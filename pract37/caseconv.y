%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void yyerror(const char *s);
extern int yylex();
%}

%token LOWERCASE UPPERCASE

%union {
    char cval;
}

%%

input: 
    | input line
    ;

line:
    LOWERCASE  { printf("%c", toupper($1)); }  // Convert lowercase to uppercase
    | UPPERCASE  { printf("%c", tolower($1)); }  // Convert uppercase to lowercase
    ;

%%

int main() {
    printf("Enter text to convert (uppercase <-> lowercase): ");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}
