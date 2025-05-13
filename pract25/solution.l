%{
#include <stdio.h>
%}

%%
"the"|"The"                  { printf("%s : Article\n", yytext); }
"a"|"an"                    { printf("%s : Article\n", yytext); }

"and"|"or"|"but"            { printf("%s : Conjunction\n", yytext); }

"to"|"before"|"after"|"than" { printf("%s : Preposition\n", yytext); }

"he"|"she"|"it"|"they"|"we"|"you"|"I"|"him"|"her"|"them"|"us"|"myself"|"itself" { printf("%s : Pronoun\n", yytext); }

"imagines"|"be"|"went"      { printf("%s : Verb\n", yytext); }

"more"|"wiser"|"intelligent" { printf("%s : Adjective\n", yytext); }

"generation"|"one"          { printf("%s : Noun\n", yytext); }

[,.]                        { /* Ignore punctuation */ }

[ \t\n]+                    { /* Ignore whitespace */ }

[a-zA-Z]+                   { printf("%s : Unknown/Other\n", yytext); }

%%

int main() {
    printf("Enter the sentence:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
