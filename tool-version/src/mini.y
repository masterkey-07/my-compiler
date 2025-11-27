%code requires {
  #include "ast.h"   /* TreeNode e NodeType visíveis para mini.tab.h */
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int yylex(void);
void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe: %s\n", s);
}

TreeNode *syntaxTreeRoot = NULL;
%}

%union {
    int      ival;    /* para NUM */
    char    *sval;    /* para ID */
    TreeNode *node;   /* para nós da AST */
}

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
        $$ = newNode(NODE_PROGRAM, "program", $1, NULL, NULL, NULL);
        syntaxTreeRoot = $$;
    }
  ;

declaracao_lista
  : declaracao_lista declaracao
    { $$ = appendSibling($1, $2); }
  | declaracao
    { $$ = newNode(NODE_DECL_LIST, NULL, $1, NULL, NULL, NULL); }
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
        $$ = newNode(NODE_VAR_DECL, $2, $1, NULL, NULL, NULL);
    }
  | tipo_especificador ID '[' NUM ']' ';'
    {
        char buf[32];
        sprintf(buf, "%d", $4);
        TreeNode *sizeNode = newNode(NODE_CONST, buf, NULL, NULL, NULL, NULL);
        $$ = newNode(NODE_VAR_DECL, $2, $1, sizeNode, NULL, NULL);
    }
  ;

tipo_especificador
  : INT
    { $$ = newNode(NODE_TYPE, "int", NULL, NULL, NULL, NULL); }
  | VOID
    { $$ = newNode(NODE_TYPE, "void", NULL, NULL, NULL, NULL); }
  ;

tipo_nao_void
  : INT
    { $$ = newNode(NODE_TYPE, "int", NULL, NULL, NULL, NULL); }
  ;

fun_declaracao
  : tipo_especificador ID '(' params ')' composto_decl
    {
        $$ = newNode(NODE_FUN_DECL, $2, $1, $4, $6, NULL);
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
  : param_lista param
    { $$ = appendSibling($1, $2); }
  | param
    { $$ = newNode(NODE_PARAM_LIST, NULL, $1, NULL, NULL, NULL); }
  ;

param
  : tipo_nao_void ID
    {
        $$ = newNode(NODE_PARAM, $2, $1, NULL, NULL, NULL);
    }
  | tipo_nao_void ID '[' ']'
    {
        TreeNode *arrayMark = newNode(NODE_VAR, "array", NULL, NULL, NULL, NULL);
        $$ = newNode(NODE_PARAM, $2, $1, arrayMark, NULL, NULL);
    }
  ;

composto_decl
  : '{' local_declaracoes statement_lista '}'
    {
        $$ = newNode(NODE_COMPOUND, "block", $2, $3, NULL, NULL);
    }
  ;

local_declaracoes
  : local_declaracoes var_declaracao
    { $$ = appendSibling($1, $2); }
  | /* empty */
    { $$ = NULL; }
  ;

statement_lista
  : statement_lista statement
    { $$ = appendSibling($1, $2); }
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
        $$ = newNode(NODE_EXPR_STMT, "expr_stmt", $1, NULL, NULL, NULL);
    }
  | ';'
    {
        $$ = newNode(NODE_EXPR_STMT, "empty_stmt", NULL, NULL, NULL, NULL);
    }
  ;

expressao
  : var '=' expressao
    {
        $$ = newNode(NODE_EXPR, "assign", $1, $3, NULL, NULL);
    }
  | simples_expressao
    { $$ = $1; }
  ;

var
  : ID
    {
        $$ = newNode(NODE_VAR, $1, NULL, NULL, NULL, NULL);
    }
  | ID '[' expressao ']'
    {
        $$ = newNode(NODE_VAR, $1, $3, NULL, NULL, NULL);
    }
  ;

selecao_decl:
        IF '(' expressao ')' statement %prec LOWER_THAN_ELSE
        {
          $$ = newNode(NODE_GENERIC, "if", $3, $5, NULL, NULL);
        }
        | IF '(' expressao ')' statement ELSE statement
        {
          $$ = newNode(NODE_GENERIC, "if_else", $3, $5, $7, NULL);
        }
        ;

iteracao_decl:
        WHILE '(' expressao ')' statement
        {
          $$ = newNode(NODE_GENERIC, "while", $3, $5, NULL, NULL);
        }
        ;

retorno_decl:
        RETURN ';'
        { $$ = newNode(NODE_GENERIC, "return", NULL, NULL, NULL, NULL); }
        | RETURN expressao ';'
        { $$ = newNode(NODE_GENERIC, "return", $2, NULL, NULL, NULL); }
        ;

var:
        ID
          { $$ = newNode(NODE_ID, $1, NULL, NULL, NULL, NULL); }
        | ID '[' expressao ']'
          {
            TreeNode *idNode    = newNode(NODE_ID, $1, NULL, NULL, NULL, NULL);
            TreeNode *indexNode = newNode(NODE_GENERIC, "index", $3, NULL, NULL, NULL);
            $$ = newNode(NODE_GENERIC, "array_var", idNode, indexNode, NULL, NULL);
          }
        ;

simples_expressao:
        soma_expressao relacional soma_expressao
          { $$ = newNode(NODE_GENERIC, "rel_expr", $2, $1, $3, NULL); }
        | soma_expressao
          { $$ = $1; }
        ;

relacional:
        '<'  
          { $$ = newNode(NODE_GENERIC, "<",  NULL, NULL, NULL, NULL); }
        | LE  
          { $$ = newNode(NODE_GENERIC, "<=",  NULL, NULL, NULL, NULL); }
        | '>'
          { $$ = newNode(NODE_GENERIC, ">",  NULL, NULL, NULL, NULL); }
        | GE
          { $$ = newNode(NODE_GENERIC, ">=",  NULL, NULL, NULL, NULL); }
        | EQ
          { $$ = newNode(NODE_GENERIC, "==",  NULL, NULL, NULL, NULL); }
        | NE
          { $$ = newNode(NODE_GENERIC, "!=",  NULL, NULL, NULL, NULL); }
        ;

soma_expressao:
        soma_expressao soma termo
          { $$ = newNode(NODE_GENERIC, "sum", $2, $1, $3, NULL); }
        | termo
          { $$ = $1; }
        ;

soma
    : '+'
      { $$ = newNode(NODE_GENERIC, "+", NULL, NULL, NULL, NULL); }
    | '-'
      { $$ = newNode(NODE_GENERIC, "-", NULL, NULL, NULL, NULL); }
    ;

termo:
        termo mult fator
          { $$ = newNode(NODE_GENERIC, "mul", $2, $1, $3, NULL); }
        | fator
          { $$ = $1; }
        ;

mult
    : '*'
      { $$ = newNode(NODE_GENERIC, "*", NULL, NULL, NULL, NULL); }
    | '/'
      { $$ = newNode(NODE_GENERIC, "/", NULL, NULL, NULL, NULL); }
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
            $$ = newNode(NODE_NUM, buf, NULL, NULL, NULL, NULL);
          }
        ;

ativacao:
        ID '(' args ')'
          {
            TreeNode *idNode = newNode(NODE_ID, $1, NULL, NULL, NULL, NULL);
            $$ = newNode(NODE_GENERIC, "call", idNode, $3, NULL, NULL );
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
          appendSibling($1, $3);
        }
        | expressao
        { $$ = $1; }
        ;

%%


int main(void) {
    if (yyparse() == 0) {
        printf("Sintaxe OK.\n");
        if (syntaxTreeRoot) {
            printf("Árvore Sintática Abstrata (AST):\n");
            printTree(syntaxTreeRoot, 0);
        }
    }
    return 0;
}
