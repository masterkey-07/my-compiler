%{
#include "parser.tab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%option yylineno

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
[a-zA-Z]+  {
                      yylval.sval = strdup(yytext);
                      return ID;
                    }

[ \t\r\n]+      { /* ignora espaços/brancos */ }
"//".*          { /* skip comment */ }
"/*"([^*]|\*+[^*/])*\*+"/"   { /* skip comment */ }

[(){}\[\],;+\-*/<>]   { return yytext[0]; }
"="                   { return '='; }

.           { fprintf(stderr, "ERRO LÉXICO: \"%s\" LINHA: %d\n", yytext, yylineno); exit(0); }

%%

int yywrap(void) {
    return 1;
}