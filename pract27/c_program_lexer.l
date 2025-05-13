%{
#include <stdio.h>
%}

%%

"int"                   { printf("%s - Keyword\n", yytext); }
[0-9]+                  { printf("%s - Constant\n", yytext); }
"="|"+"|"-"|"*"|"/"     { printf("%s - Operator\n", yytext); }
"{"|"}"|";"             { printf("%s - Symbol\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("%s - Identifier\n", yytext); }
[ \t\n]                 ;   // Ignore whitespace
.                       { printf("%s - Unknown\n", yytext); }

%%

int main() {
    printf("Lexical Analysis Output:\n\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
