%{
#include "calc.tab.h"
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
%}

%%

[0-9]+(\.[0-9]+)?    { yylval.dval = atof(yytext); return NUMBER; }
"sin"                { return SIN; }
"cos"                { return COS; }
"tan"                { return TAN; }
"sqrt"               { return SQRT; }
"log"                { return LOG; }
"pow"                { return POW; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }

"="                  { return '='; }
","                  { return ','; }
"^"                  { return '^'; }
"+"                  { return '+'; }
"-"                  { return '-'; }
"*"                  { return '*'; }
"/"                  { return '/'; }
"("                  { return '('; }
")"                  { return ')'; }
"\n"                 { return '\n'; }
[ \t]+               { /* Ignore whitespace */ }
.                    { printf("Unknown character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
