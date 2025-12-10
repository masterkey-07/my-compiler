%code requires {
  #include "symbol.h"   /* TreeNode e NodeType visíveis para mini.tab.h */
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

extern int yylineno;
extern char* yytext;

int yylex(void);

void yyerror() {
  fprintf(stderr, "ERRO SINTÁTICO: \"%s\" LINHA: %d\n", yytext, yylineno);
}

TreeNode *syntax_tree_root = NULL;
%}

%union {
    int      ival;    /* para NUM */
    char    *sval;    /* para ID */
    TreeNode *node;   /* para nós da AST */
}

%locations

%token INT VOID IF ELSE WHILE RETURN
%token LE GE EQ NE

%token <sval> ID
%token <ival> NUM

%type <node> programa declaracao_lista declaracao
%type <node> var_declaracao tipo_especificador tipo_nao_void
%type <node> fun_declaracao params param_lista param
%type <node> composto_decl local_declaracoes statement_lista
%type <node> statement expressao_decl selecao_decl iteracao_decl retorno_decl
%type <node> expressao var simples_expressao relacional
%type <node> soma_expressao termo soma fator mult
%type <node> ativacao args arg_lista

%right '='
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc EQ NE LE GE '<' '>'
%left '+' '-'
%left '*' '/'

%start programa

%%

programa
  : declaracao_lista
    {
        $$ = create_node(NODE_PROGRAM, "program", $1, NULL, NULL, NULL);
        syntax_tree_root = $$;
    }
  ;

declaracao_lista
  : declaracao_lista declaracao
    { $$ = append_sibling_node($1, $2); }
  | declaracao
    { $$ = create_node(NODE_DECL_LIST, NULL, $1, NULL, NULL, NULL); }
  ;

declaracao
  : var_declaracao
    { $$ = $1; }
  | fun_declaracao
    { $$ = $1; }
  ;

var_declaracao
  : tipo_especificador ID ';'
    {
        /* nó de variável: texto = nome do identificador */
        $$ = create_node(NODE_VAR_DECL, $2, $1, NULL, NULL, NULL);
    }
  | tipo_especificador ID '[' NUM ']' ';'
    {
        char buf[32];
        sprintf(buf, "%d", $4);
        TreeNode *sizeNode = create_node(NODE_CONST, buf, NULL, NULL, NULL, NULL);
        $$ = create_node(NODE_VAR_DECL, $2, $1, sizeNode, NULL, NULL);
    }
  ;

tipo_especificador
  : INT
    { $$ = create_node(NODE_TYPE, "int", NULL, NULL, NULL, NULL); }
  | VOID
    { $$ = create_node(NODE_TYPE, "void", NULL, NULL, NULL, NULL); }
  ;

tipo_nao_void
  : INT
    { $$ = create_node(NODE_TYPE, "int", NULL, NULL, NULL, NULL); }
  ;

fun_declaracao
  : tipo_especificador ID '(' params ')' composto_decl
    {
        $$ = create_node(NODE_FUN_DECL, $2, $1, $4, $6, NULL);
    }
  ;

params
  : param_lista
    { $$ = $1; }
  | VOID
    {
        /* função sem parâmetros: podemos usar NULL ou um nó especial */
        $$ = NULL;
    }
  ;

param_lista
  : param_lista ',' param
    { $$ = append_sibling_node($1, $3); }
  | param
    { $$ = create_node(NODE_PARAM_LIST, NULL, $1, NULL, NULL, NULL); }
  ;

param
  : tipo_nao_void ID
    {
        $$ = create_node(NODE_PARAM, $2, $1, NULL, NULL, NULL);
    }
  | tipo_nao_void ID '[' ']'
    {
        TreeNode *array_mark = create_node(NODE_VAR, "array", NULL, NULL, NULL, NULL);
        $$ = create_node(NODE_PARAM, $2, $1, array_mark, NULL, NULL);
    }
  ;

composto_decl
  : '{' local_declaracoes statement_lista '}'
    {
        $$ = create_node(NODE_COMPOUND, "block", $2, $3, NULL, NULL);
    }
  ;

local_declaracoes
  : local_declaracoes var_declaracao
    { $$ = append_sibling_node($1, $2); }
  | /* empty */
    { $$ = NULL; }
  ;

statement_lista
  : statement_lista statement
    { $$ = append_sibling_node($1, $2); }
  | /* empty */
    { $$ = NULL; }
  ;

statement
  : expressao_decl       { $$ = $1; }
  | composto_decl        { $$ = $1; }
  | selecao_decl         { $$ = $1; }
  | iteracao_decl        { $$ = $1; }
  | retorno_decl         { $$ = $1; }
  ;

expressao_decl
  : expressao ';'
    {
        $$ = create_node(NODE_EXPR_STMT, "expr_stmt", $1, NULL, NULL, NULL);
    }
  | ';'
    {
        $$ = create_node(NODE_EXPR_STMT, "empty_stmt", NULL, NULL, NULL, NULL);
    }
  ;

expressao
  : var '=' expressao
    {
        $$ = create_node(NODE_EXPR, "assign", $1, $3, NULL, NULL);
    }
  | simples_expressao
    { $$ = $1; }
  ;

var
  : ID
    {
        $$ = create_node(NODE_VAR, $1, NULL, NULL, NULL, NULL);
    }
  | ID '[' expressao ']'
    {
        $$ = create_node(NODE_VAR, $1, $3, NULL, NULL, NULL);
    }
  ;

selecao_decl:
        IF '(' expressao ')' statement %prec LOWER_THAN_ELSE
        {
          $$ = create_node(NODE_GENERIC, "if", $3, $5, NULL, NULL);
        }
        | IF '(' expressao ')' statement ELSE statement
        {
          $$ = create_node(NODE_GENERIC, "if_else", $3, $5, $7, NULL);
        }
        ;

iteracao_decl:
        WHILE '(' expressao ')' statement
        {
          $$ = create_node(NODE_GENERIC, "while", $3, $5, NULL, NULL);
        }
        ;

retorno_decl:
        RETURN ';'
        { $$ = create_node(NODE_GENERIC, "return", NULL, NULL, NULL, NULL); }
        | RETURN expressao ';'
        { $$ = create_node(NODE_GENERIC, "return", $2, NULL, NULL, NULL); }
        ;

/*
Essa gramática está duplicada, me parece que a anterior é "melhor"
var:
        ID
          { $$ = create_node(NODE_ID, $1, NULL, NULL, NULL, NULL); }
        | ID '[' expressao ']'
          {
            TreeNode *node_id    = create_node(NODE_ID, $1, NULL, NULL, NULL, NULL);
            TreeNode *indexNode = create_node(NODE_GENERIC, "index", $3, NULL, NULL, NULL);
            $$ = create_node(NODE_GENERIC, "array_var", node_id, indexNode, NULL, NULL);
          }
        ;
*/


simples_expressao:
        soma_expressao relacional soma_expressao
          { $$ = create_node(NODE_GENERIC, "rel_expr", $2, $1, $3, NULL); }
        | soma_expressao
          { $$ = $1; }
        ;

relacional:
        '<'  
          { $$ = create_node(NODE_GENERIC, "<",  NULL, NULL, NULL, NULL); }
        | LE  
          { $$ = create_node(NODE_GENERIC, "<=",  NULL, NULL, NULL, NULL); }
        | '>'
          { $$ = create_node(NODE_GENERIC, ">",  NULL, NULL, NULL, NULL); }
        | GE
          { $$ = create_node(NODE_GENERIC, ">=",  NULL, NULL, NULL, NULL); }
        | EQ
          { $$ = create_node(NODE_GENERIC, "==",  NULL, NULL, NULL, NULL); }
        | NE
          { $$ = create_node(NODE_GENERIC, "!=",  NULL, NULL, NULL, NULL); }
        ;

soma_expressao:
        soma_expressao soma termo
          { $$ = create_node(NODE_GENERIC, "sum", $2, $1, $3, NULL); }
        | termo
          { $$ = $1; }
        ;

soma
    : '+'
      { $$ = create_node(NODE_GENERIC, "+", NULL, NULL, NULL, NULL); }
    | '-'
      { $$ = create_node(NODE_GENERIC, "-", NULL, NULL, NULL, NULL); }
    ;

termo:
        termo mult fator
          { $$ = create_node(NODE_GENERIC, "mul", $2, $1, $3, NULL); }
        | fator
          { $$ = $1; }
        ;

mult
    : '*'
      { $$ = create_node(NODE_GENERIC, "*", NULL, NULL, NULL, NULL); }
    | '/'
      { $$ = create_node(NODE_GENERIC, "/", NULL, NULL, NULL, NULL); }
    ;

fator:
        '(' expressao ')'
          { $$ = $2; }
        | var
          { $$ = $1; }
        | ativacao
          { $$ = $1;}
        | NUM
          { 
            char buf[32];
            sprintf(buf, "%d", $1);
            $$ = create_node(NODE_NUM, buf, NULL, NULL, NULL, NULL);
          }
        ;

ativacao:
        ID '(' args ')'
          {
            TreeNode *node_id = create_node(NODE_ID, $1, NULL, NULL, NULL, NULL);
            $$ = create_node(NODE_GENERIC, "call", node_id, $3, NULL, NULL );
          }
        ;

args:
        arg_lista
        {
          $$ = $1;
        }
        |
        {
          $$ = NULL;
        }
        ;

arg_lista:
        arg_lista ',' expressao
        {
          append_sibling_node($1, $3);
        }
        | expressao
        { $$ = $1; }
        ;

%%

TreeNode *parse_syntax_tree(void) {
    if (yyparse() == 0)
        return syntax_tree_root;

    return NULL;
}