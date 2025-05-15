%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%left '(' ')'

%%
input:
      /* empty */
    | input line
    ;

line:
      expr '\n'     { printf("Result: %d\n", $1); }
    ;

expr:
      NUMBER        { $$ = $1; }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr {
                        if ($3 == 0) {
                            printf("Error: Division by zero\n");
                            exit(1);
                        }
                        $$ = $1 / $3;
                    }
    | '(' expr ')'  { $$ = $2; }
    ;
%%

int main() {
    printf("Enter expression:\n");
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 1;
}
