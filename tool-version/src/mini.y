/* mini.y — Gramática estilo C em PT-BR, pronta para Bison */

/* ===== Cabeçalho C ===== */
%code requires {
  /* Coloque aqui headers compartilhados entre parser e scanner, se precisar */
}

%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyerror(const char *s) {
  fprintf(stderr, "Erro de sintaxe: %s\n", s);
  return 0;
}
%}

/* ===== Configurações do Bison ===== */
%define parse.error detailed
%define api.value.type {int}   /* não usamos valores semânticos por enquanto */

/* ===== Tokens ===== */
%token INT VOID IF ELSE WHILE RETURN
%token ID NUM
%token LE GE EQ NE             /* <= >= == != */

/* ===== Precedência / Associatividade ===== */
%right '='
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc EQ NE LE GE '<' '>'  /* relacionais */
%left '+' '-'
%left '*' '/'

/* Símbolo inicial */
%start programa

%% /* ===== Gramática ===== */

/* 1 */ programa
    : declaracao_lista
    ;

/* 2 */ declaracao_lista
    : declaracao_lista declaracao
    | declaracao
    ;

/* 3 */ declaracao
    : var_declaracao
    | fun_declaracao
    ;

/* 4 */ var_declaracao
    : tipo_especificador ID ';'
    | tipo_especificador ID '[' NUM ']' ';'
    ;

/* 5 */ tipo_especificador
    : INT
    | VOID
    ;

/* 6 */ fun_declaracao
    : tipo_especificador ID '(' params ')' composto_decl
    ;

/* 7 */ params
    : param_lista
    | VOID
    ;

/* 8 */ param_lista
    : param_lista ',' param
    | param
    ;

/* 9 */ param
    : tipo_nao_void ID
    | tipo_nao_void ID '[' ']'
    ;

/* auxiliar para evitar "void ID" em parâmetros */
tipo_nao_void
    : INT
    ;

/* 10 */ composto_decl
    : '{' local_declaracoes statement_lista '}'
    ;

/* 11 */ local_declaracoes
    : local_declaracoes var_declaracao
    | /* empty */
    ;

/* 12 */ statement_lista
    : statement_lista statement
    | /* empty */
    ;

/* 13 */ statement
    : expressao_decl
    | composto_decl
    | selecao_decl
    | iteracao_decl
    | retorno_decl
    ;

/* 14 */ expressao_decl
    : expressao ';'
    | ';'
    ;

/* 15 */ selecao_decl
    : IF '(' expressao ')' statement %prec LOWER_THAN_ELSE
    | IF '(' expressao ')' statement ELSE statement
    ;

/* 16 */ iteracao_decl
    : WHILE '(' expressao ')' statement
    ;

/* 17 */ retorno_decl
    : RETURN ';'
    | RETURN expressao ';'
    ;

/* 18 */ expressao
    : var '=' expressao
    | simples_expressao
    ;

/* 19 */ var
    : ID
    | ID '[' expressao ']'
    ;

/* 20 */ simples_expressao
    : soma_expressao relacional soma_expressao
    | soma_expressao
    ;

/* 21 */ relacional
    : '<'
    | LE
    | '>'
    | GE
    | EQ
    | NE
    ;

/* 22 */ soma_expressao
    : soma_expressao soma termo
    | termo
    ;

/* 23 */ soma
    : '+'
    | '-'
    ;

/* 24 */ termo
    : termo mult fator
    | fator
    ;

/* 25 */ mult
    : '*'
    | '/'
    ;

/* 26 */ fator
    : '(' expressao ')'
    | var
    | ativacao
    | NUM
    ;

/* 27 */ ativacao
    : ID '(' args ')'
    ;

/* 28 */ args
    : arg_lista
    | /* empty */
    ;

/* 29 */ arg_lista
    : arg_lista ',' expressao
    | expressao
    ;

%% /* ===== Código de apoio (main) ===== */

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Erro abrindo arquivo");
            return 1;
        }
    }
    return yyparse();
}