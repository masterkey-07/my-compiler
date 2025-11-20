%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s) {
    fprintf(stderr, "parse error: %s\n", s);
}
%}

/* One token: NUM */
%token NUM

/* Precedence and associativity */
%left '+' '-'
%left '*' '/'

/* Start symbol */
%start input

%%
/* Grammar */

input
    : /* empty */
    | input line
    ;

line
    : '\n'
    | expr '\n'   { printf("= %d\n", $1); }
    ;

expr
    : NUM                         { $$ = $1; }
    | expr '+' expr               { $$ = $1 + $3; }
    | expr '-' expr               { $$ = $1 - $3; }
    | expr '*' expr               { $$ = $1 * $3; }
    | expr '/' expr               { $$ = $1 / $3; }
    | '(' expr ')'                { $$ = $2; }
    ;

%%

int main(void) {
    return yyparse();
}
