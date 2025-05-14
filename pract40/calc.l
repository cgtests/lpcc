%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
%}

digit   [0-9]+(\.[0-9]+)?
id      [a-zA-Z_][a-zA-Z0-9_]*

%%

"pow"                 { return POW; }
"log"                 { return LOG; }
{digit}               { yylval.fval = atof(yytext); return NUMBER; }
{id}                  { yylval.id = strdup(yytext); return ID; }
"="                   { return '='; }
","                   { return ','; }
"("                   { return '('; }
")"                   { return ')'; }
"+"                   { return '+'; }
"-"                   { return '-'; }
"*"                   { return '*'; }
"/"                   { return '/'; }
";"                   { return ';'; }
\n                    { return '\n'; }
[ \t]+                ;  // skip whitespace
.                     { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
