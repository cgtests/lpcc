%{
#include <stdio.h>
%}

%%  <-- Token matching rules start here

"int"         { printf("%s : Keyword\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("%s : Identifier\n", yytext); }
[0-9]+        { printf("%s : Number\n", yytext); }
"="           { printf("%s : Assignment Operator\n", yytext); }
"+"|"-"       { printf("%s : Arithmetic Operator\n", yytext); }
";"           { printf("%s : Semicolon\n", yytext); }
","           { printf("%s : Comma\n", yytext); }
"{"           { printf("%s : Opening Brace\n", yytext); }
"}"           { printf("%s : Closing Brace\n", yytext); }
[ \t\n]+      { /* skip whitespace */ }

%%  <-- C code like main() goes here

int main() {
    printf("Enter the C code:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
