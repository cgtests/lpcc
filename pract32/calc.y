%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX_VARS 100

typedef struct {
    char name[50];
    double value;
} variable;

variable vars[MAX_VARS];
int var_count = 0;


int is_valid_identifier(const char* name)
{
    if(!isalpha(name[0]) && name[0] != '_')
        return 0;
    for(int i = 1; name[i] != '\0'; i++)
    {
        if(!isalnum(name[i]) && name[i] != '_')
        {
            return 0;
        }
    }

    return 1;
}

double get_var_value(char* name) {
    if(!is_valid_identifier(name))
    {
        printf("Invalid variable name\n");
        return 0;
    }

    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }
    printf("Valid variable name\n", name);
    return 0;
}

void set_var_value(char* name, double value) {
    if(!is_valid_identifier(name))
    {
        printf("invalid variable name\n");
        return;
    }
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    strcpy(vars[var_count].name, name);
    vars[var_count].value = value;
    var_count++;
}

void yyerror(const char *s) {
    printf("Syntax error: %s\n", s);
}

extern int yylex();
%}

%union {
    double dval;
    char* sval;
}


%token <dval> NUMBER
%token <sval> IDENTIFIER
%token SIN COS TAN SQRT LOG POW
%left '+' '-'
%left '*' '/'
%right '^'
%left '(' ')'
%right '='
%right ','
%type <dval> expr

%%

input: 
    | input line
    ;

line: 
    expr '\n'  { printf("Result: %lf\n", $1); }
    | IDENTIFIER '=' expr '\n' { set_var_value($1, $3); printf("Assigned: %s = %lf\n", $1, $3); }
    ;

expr:
    NUMBER          { $$ = $1; }
    | IDENTIFIER    { $$ = get_var_value($1); }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if ($3 == 0) {
            printf("Error: Division by zero\n");
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
    | expr '^' expr { $$ = pow($1, $3); }
    | POW '(' expr ',' expr ')' { $$ = pow($3, $5); }
    | '(' expr ')'  { $$ = $2; }
    | SIN '(' expr ')' { $$ = sin($3); }
    | COS '(' expr ')' { $$ = cos($3); }
    | TAN '(' expr ')' { $$ = tan($3); }
    | SQRT '(' expr ')' { $$ = sqrt($3); }
    | LOG '(' expr ')' { $$ = log($3); }
    ;

%%

int main() {
    printf("Enter expressions:\n");
    yyparse();
    printf("Valid variable");

    
    return 0;
}
