%{
#include "calc.tab.h"
#include <stdio.h>
%}

%%

[A-Za-z_][A-Za-z0-9_]*   { return IDENTIFIER; }  // Matches valid identifiers
.                         { return yytext[0]; }   // Return individual characters for invalid tokens

%%

int yywrap() {
    return 1;
}
