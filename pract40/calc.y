%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
%}

%union {
    float fval;
    char* id;
}

%token <fval> NUMBER
%token <id> ID
%token POW LOG
%left '+' '-'
%left '*' '/'
%type <fval> expr

%%

input:
      /* empty */
    | input stmt
    ;

stmt:
      ID '=' expr term   { printf("%s = %.4f\n", $1, $3); free($1); }
    ;

term:
      '\n'
    | ';'
    ;

expr:
      expr '+' expr               { $$ = $1 + $3; }
    | expr '-' expr               { $$ = $1 - $3; }
    | expr '*' expr               { $$ = $1 * $3; }
    | expr '/' expr               { $$ = ($3 != 0) ? $1 / $3 : 0; }
    | POW '(' expr ',' expr ')'   { $$ = pow($3, $5); }
    | LOG '(' expr ')'            { $$ = log($3); }
    | NUMBER                      { $$ = $1; }
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
