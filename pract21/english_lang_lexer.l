%{
#include <stdio.h>
%}

%%

[ \t\n]           

car|house|man|tent|ocean|country|city       { printf("%s - Noun\n", yytext); }
she|he|we|they|it    { printf("%s - Pronoun\n", yytext); }
come|go|walk|did|have|read|write       { printf("%s - Verb\n", yytext); }
pretty|old|blue|smart      { printf("%s - Adjective\n", yytext); }
abnormally|accidentlly|actually|beautifully|bitterly|brightly|calmly   { printf("%s - Adverb\n", yytext); }
by|with|about|until        { printf("%s - Preposition\n", yytext); }
and|but|or|while|because        { printf("%s - Conjunction\n", yytext); }
oh!|wow!|oops!        { printf("%s - Interjection\n", yytext); }

.               { printf("%s - Unknown Token\n", yytext); } /* Catch all other characters */

%%

int main() {
    printf("Enter a sentence: ");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
