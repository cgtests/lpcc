%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Convert matched word to lowercase for comparison
void to_lower(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = tolower(s[i]);
    }
}
%}

%%
[,.]              { /* Ignore punctuation */ }

[ \t\n]+          { /* Ignore whitespace */ }

[a-zA-Z]+ {
    to_lower(yytext); // normalize case

    if (strcmp(yytext, "human") == 0 ||
        strcmp(yytext, "beings") == 0 ||
        strcmp(yytext, "instinct") == 0 ||
        strcmp(yytext, "thinking") == 0 ||
        strcmp(yytext, "urge") == 0 ||
        strcmp(yytext, "things") == 0 ||
        strcmp(yytext, "groups") == 0 ||
        strcmp(yytext, "gap") == 0)
    {
        printf("%s : Noun\n", yytext);
    }
    else if (strcmp(yytext, "have") == 0 ||
             strcmp(yytext, "divide") == 0)
    {
        printf("%s : Verb\n", yytext);
    }
    else if (strcmp(yytext, "strong") == 0 ||
             strcmp(yytext, "dramatic") == 0 ||
             strcmp(yytext, "basic") == 0 ||
             strcmp(yytext, "binary") == 0 ||
             strcmp(yytext, "distinct") == 0 ||
             strcmp(yytext, "empty") == 0)
    {
        printf("%s : Adjective\n", yytext);
    }
    else if (strcmp(yytext, "toward") == 0 ||
             strcmp(yytext, "into") == 0 ||
             strcmp(yytext, "to") == 0)
    {
        printf("%s : Preposition\n", yytext);
    }
    else if (strcmp(yytext, "the") == 0 ||
             strcmp(yytext, "a") == 0 ||
             strcmp(yytext, "an") == 0)
    {
        printf("%s : Article\n", yytext);
    }
    else if (strcmp(yytext, "and") == 0 ||
             strcmp(yytext, "or") == 0 ||
             strcmp(yytext, "but") == 0 ||
             strcmp(yytext, "with") == 0 ||
             strcmp(yytext, "in") == 0 ||
             strcmp(yytext, "between") == 0)
    {
        printf("%s : Conjunction\n", yytext);
    }
    else {
        printf("%s : Unknown/Other\n", yytext);
    }
}
%%

int main() {
    printf("Enter the sentence:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
