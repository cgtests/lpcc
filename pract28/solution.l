%{
#include <stdio.h>
%}

%%

"char"                      { printf("%s : Keyword\n", yytext); }
"printf"                    { printf("%s : Function\n", yytext); }
\"[^\"]*\"                  { printf("%s : String Literal\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*      { printf("%s : Identifier\n", yytext); }
"="                         { printf("%s : Assignment Operator\n", yytext); }
";"                         { printf("%s : Semicolon\n", yytext); }
"("                         { printf("%s : Opening Parenthesis\n", yytext); }
")"                         { printf("%s : Closing Parenthesis\n", yytext); }
"{"                         { printf("%s : Opening Brace\n", yytext); }
"}"                         { printf("%s : Closing Brace\n", yytext); }
[ \t\n]+                    { /* Skip whitespace */ }
.                           { printf("%s : Unknown Token\n", yytext); }

%%

int main() {
    printf("Enter the C code:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
