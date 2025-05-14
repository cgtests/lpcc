%{
#include <stdio.h>
%}

%%
"int"            { printf("%s : Keyword\n", yytext); }
"char"|"float"|"double"|"void" { printf("%s : Keyword\n", yytext); }
"="              { printf("%s : Assignment Operator\n", yytext); }
";"              { printf("%s : Semicolon\n", yytext); }
"{"              { printf("%s : Opening Brace\n", yytext); }
"}"              { printf("%s : Closing Brace\n", yytext); }
"("              { printf("%s : Opening Parenthesis\n", yytext); }
")"              { printf("%s : Closing Parenthesis\n", yytext); }
"/"              { printf("%s : Division Operator\n", yytext); }
"*"              { printf("%s : Multiplication Operator\n", yytext); }
"+"              { printf("%s : Addition Operator\n", yytext); }
"-"              { printf("%s : Subtraction Operator\n", yytext); }
[0-9]+           { printf("%s : Number\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("%s : Identifier\n", yytext); }
[ \t\n]          ;  // Ignore whitespace
.                { printf("%s : Unknown Token\n", yytext); }
%%

int main() {
    printf("Enter the C code:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
