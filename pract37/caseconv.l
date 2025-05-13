%{
#include "caseconv.tab.h"
#include <stdio.h>
%}

%%

[a-z]   { yylval.cval = yytext[0]; return LOWERCASE; }  // Matches lowercase letters
[A-Z]   { yylval.cval = yytext[0]; return UPPERCASE; }  // Matches uppercase letters
[ \t\n] { /* Ignore whitespace */ }
.       { /* Ignore any other character */ }

%%

int yywrap() {
    return 1;
}
