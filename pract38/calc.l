%{
#include "y.tab.h"
#include <string.h>
#include <stdlib.h>
%}

digit   [0-9]
id      [a-zA-Z_][a-zA-Z0-9_]*
string  \"[^\"]*\"

%%

{digit}+              { yylval.num = atoi(yytext); return NUMBER; }
"sqrt"                { return SQRT; }
"strlen"              { return STRLEN; }
{id}                  { yylval.id = strdup(yytext); return ID; }
{string}              { yylval.str = strdup(yytext); return STRING; }
"="                   { return '='; }
"("                   { return '('; }
")"                   { return ')'; }
\n                    { return '\n'; }
[ \t]+                ; // ignore whitespace
.                     { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
