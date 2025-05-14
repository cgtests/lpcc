%{
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int yylex();
void yyerror(const char *s);
%}

%union {
    int num;
    char* str;
    char* id;
}

%token <num> NUMBER
%token <str> STRING
%token <id> ID
%token SQRT STRLEN

%type <num> expr

%%

input:
      /* empty */
    | input stmt
    ;

stmt:
      ID '=' expr '\n'   { printf("%s = %d\n", $1, $3); free($1); }
    ;

expr:
      SQRT '(' NUMBER ')'    { $$ = (int)sqrt($3); }
    | STRLEN '(' STRING ')'  { $$ = strlen($3) - 2; free($3); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter input (Ctrl+D to exit):\n");
    yyparse();
    return 0;
}
