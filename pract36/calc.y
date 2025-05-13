%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
extern int yylex();
%}

%token IDENTIFIER

%%

input: 
    | input line
    ;

line: 
    IDENTIFIER '\n'  { 
        printf("Valid variable: %s\n", $1);
    }
    | IDENTIFIER { 
        printf("Invalid variable: %s\n", $1); 
    }
    ;

%%

int main() {
    printf("Enter variable names:\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}
