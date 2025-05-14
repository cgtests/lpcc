%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
%}

digit   [0-9]+
id      [a-zA-Z_][a-zA-Z0-9_]*

%%

{digit}               { yylval.num = atoi(yytext); return NUMBER; }
"sin"                 { return SIN; }
"cos"                 { return COS; }
{id}                  { yylval.id = strdup(yytext); return ID; }
"="                   { return '='; }
"("                   { return '('; }
")"                   { return ')'; }
"+"                   { return '+'; }
"-"                   { return '-'; }
"*"                   { return '*'; }
"/"                   { return '/'; }
\n                    { return '\n'; }
[ \t]                 ; // skip whitespace
.                     { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
