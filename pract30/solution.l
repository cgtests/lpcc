%{
#include <stdio.h>
%}

%%
// Keywords
"int"|"float"|"char"|"double" { printf("%s : Keyword\n", yytext); }

// Identifiers
[a-zA-Z_][a-zA-Z0-9_]*        { printf("%s : Identifier\n", yytext); }

// Numbers
[0-9]+(\.[0-9]+)?             { printf("%s : Number\n", yytext); }

// Operators
"="                           { printf("%s : Assignment Operator\n", yytext); }
"*"                           { printf("%s : Multiplication Operator\n", yytext); }
"+"                           { printf("%s : Addition Operator\n", yytext); }
"-"                           { printf("%s : Subtraction Operator\n", yytext); }
"/"                           { printf("%s : Division Operator\n", yytext); }

// Symbols
";"                           { printf("%s : Semicolon\n", yytext); }
","                           { printf("%s : Comma\n", yytext); }
"("                           { printf("%s : Opening Parenthesis\n", yytext); }
")"                           { printf("%s : Closing Parenthesis\n", yytext); }
"{"                           { printf("%s : Opening Brace\n", yytext); }
"}"                           { printf("%s : Closing Brace\n", yytext); }

// Whitespace
[ \t\n]+                      { /* Ignore whitespace */ }

// Unknown tokens
.                             { printf("%s : Unknown Token\n", yytext); }
%%

int main() {
    printf("Enter the C code:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
