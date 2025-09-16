/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode *statement_sequence(void);
static TreeNode *statement(void);
static TreeNode *if_stmt(void);
static TreeNode *repeat_stmt(void);
static TreeNode *assign_stmt(void);
static TreeNode *read_stmt(void);
static TreeNode *write_stmt(void);
static TreeNode *exp(void);
static TreeNode *simple_exp(void);
static TreeNode *term(void);
static TreeNode *factor(void);

static void syntaxError(char *message)
{
  fprintf(listing, "\n>>> ");
  fprintf(listing, "Syntax error at line %d: %s", lineno, message);
  Error = TRUE;
}

static void match(TokenType expected)
{
  if (token == expected)
    token = getToken();
  else
  {
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    fprintf(listing, "      ");
  }
}

TreeNode *statement_sequence(void)
{
  TreeNode *node = statement();

  TreeNode *help_node = node;

  while ((token != ENDFILE) && (token != END) &&
         (token != ELSE) && (token != UNTIL))
  {
    TreeNode *another_help_node;

    match(SEMI);

    another_help_node = statement();

    if (another_help_node != NULL)
    {
      if (node == NULL)
        node = help_node = another_help_node;
      else /* now p cannot be NULL either */
      {
        help_node->sibling = another_help_node;
        help_node = another_help_node;
      }
    }
  }

  return node;
}

TreeNode *statement(void)
{
  TreeNode *node = NULL;

  switch (token)
  {
  case IF:
    node = if_stmt();
    break;
  case REPEAT:
    node = repeat_stmt();
    break;
  case ID:
    node = assign_stmt();
    break;
  case READ:
    node = read_stmt();
    break;
  case WRITE:
    node = write_stmt();
    break;
  default:
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    token = getToken();
    break;
  } /* end case */
  return node;
}

TreeNode *if_stmt(void)
{
  TreeNode *node = newStatementNode(IfK);

  match(IF);

  if (node != NULL)
    node->child[0] = exp();

  match(THEN);

  if (node != NULL)
    node->child[1] = statement_sequence();

  if (token == ELSE)
  {
    match(ELSE);
    if (node != NULL)
      node->child[2] = statement_sequence();
  }

  match(END);

  return node;
}

TreeNode *repeat_stmt(void)
{
  TreeNode *t = newStatementNode(RepeatK);

  match(REPEAT);

  if (t != NULL)
    t->child[0] = statement_sequence();

  match(UNTIL);

  if (t != NULL)
    t->child[1] = exp();

  return t;
}

TreeNode *assign_stmt(void)
{
  TreeNode *t = newStatementNode(AssignK);

  if ((t != NULL) && (token == ID))
    t->attr.name = copyString(tokenString);

  match(ID);

  match(ASSIGN);

  if (t != NULL)
    t->child[0] = exp();

  return t;
}

TreeNode *read_stmt(void)
{
  TreeNode *t = newStatementNode(ReadK);
  match(READ);
  if ((t != NULL) && (token == ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  return t;
}

TreeNode *write_stmt(void)
{
  TreeNode *t = newStatementNode(WriteK);
  match(WRITE);
  if (t != NULL)
    t->child[0] = exp();
  return t;
}

TreeNode *exp(void)
{
  TreeNode *t = simple_exp();

  if ((token == LT) || (token == EQ))
  {
    TreeNode *p = newExpNode(OpK);
    if (p != NULL)
    {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
    }
    match(token);
    if (t != NULL)
      t->child[1] = simple_exp();
  }
  return t;
}

TreeNode *simple_exp(void)
{
  TreeNode *t = term();
  while ((token == PLUS) || (token == MINUS))
  {
    TreeNode *p = newExpNode(OpK);
    if (p != NULL)
    {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      t->child[1] = term();
    }
  }
  return t;
}

TreeNode *term(void)
{
  TreeNode *t = factor();
  while ((token == TIMES) || (token == OVER))
  {
    TreeNode *p = newExpNode(OpK);
    if (p != NULL)
    {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      p->child[1] = factor();
    }
  }
  return t;
}

TreeNode *factor(void)
{
  TreeNode *t = NULL;
  switch (token)
  {
  case NUM:
    t = newExpNode(ConstK);
    if ((t != NULL) && (token == NUM))
      t->attr.val = atoi(tokenString);
    match(NUM);
    break;
  case ID:
    t = newExpNode(IdK);
    if ((t != NULL) && (token == ID))
      t->attr.name = copyString(tokenString);
    match(ID);
    break;
  case LPAREN:
    match(LPAREN);
    t = exp();
    match(RPAREN);
    break;
  default:
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    token = getToken();
    break;
  }
  return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode *parse(void)
{
  TreeNode *tree;

  token = getToken();

  tree = statement_sequence();

  if (token != ENDFILE)
    syntaxError("Code ends before file\n");

  return tree;
}
