%{
#include "mini.tab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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


[0-9]+          { yylval = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  { return ID; }


[ \t\r\n]+      { /* ignore */ }


[(){}\[\],;+\-*/<>]   { return yytext[0]; }
"="                   { return '='; }


.               { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
