%{
#include "parser.tab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YY_USER_ACTION                                             \
  yylloc.first_line = yylloc.last_line;                            \
  yylloc.first_column = yylloc.last_column;                        \
  if (yylloc.last_line == yylineno)                                \
    yylloc.last_column += yyleng;                                  \
  else {                                                           \
    yylloc.last_line = yylineno;                                   \
    yylloc.last_column = yytext + yyleng - strrchr(yytext, '\n');  \
  }
%}

%option yylineno
%option noinput
%option nounput

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