%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
%}

%union {
    int num;
    char* id;
}

%token <num> NUMBER
%token <id> ID
%token SIN COS

%left '+' '-'  // define operator precedence
%left '*' '/'

%type <num> expr

%%

input:
      /* empty */
    | input stmt
    ;

stmt:
      ID '=' expr '\n'  { printf("%s = %d\n", $1, $3); free($1); }
    ;

expr:
      expr '+' expr     { $$ = $1 + $3; }
    | expr '-' expr     { $$ = $1 - $3; }
    | expr '*' expr     { $$ = $1 * $3; }
    | expr '/' expr     { $$ = $3 != 0 ? $1 / $3 : 0; }
    | SIN '(' expr ')'  { $$ = (int)sin($3); }
    | COS '(' expr ')'  { $$ = (int)cos($3); }
    | NUMBER            { $$ = $1; }
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
