%{
#include "parser.tab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%

"int"           { return INT; }
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }

"<="            { return LE; }
">="            { return GE; }
"=="            { return EQ; }
"!="            { return NE; }

[0-9]+          { yylval.ival = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  {
                      yylval.sval = strdup(yytext);
                      return ID;
                    }

[ \t\r\n]+      { /* ignora espaços/brancos */ }
"//".*          { /* skip comment */ }

[(){}\[\],;+\-*/<>]   { return yytext[0]; }
"="                   { return '='; }

.           { fprintf(stderr, "Caracter inesperado: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}