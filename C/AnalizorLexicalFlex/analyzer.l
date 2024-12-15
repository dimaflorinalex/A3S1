/*
    How to compile and run:

    flex analyzer.l
    gcc -o analyzer lex.yy.c
    ./analyzer snippet_1.c
*/

/* Flag for flex to autogenerate yywrap() function which returns 1 (true) when first EOF (end of file) is reached, so flex stops: https://silcnitc.github.io/lex.html */
%option noyywrap

%{
/* Definitions Section */
#include <stdio.h>
#include <stdlib.h>

int current_line_index = 1; /* Current line number */

int count_newlines(const char *str); /* Counts newline characters in string */
%}

/* Tokens */
DIGIT           [0-9]
LETTER          [a-zA-Z_]
IDENTIFIER      {LETTER}({LETTER}|{DIGIT})*
HEX_NUMBER      0[xX][0-9a-fA-F]+
OCT_NUMBER      0[0-7]+
BIN_NUMBER      0[bB][01]+
DEC_NUMBER      {DIGIT}+(\.[0-9]+)?([eE][+-]?[0-9]+)?
CHAR_LITERAL    \'([^'\\]|\\.)\'
STRING_LITERAL  \"([^"\\]|\\.)*(\\\n([^"\\]|\\.)*)*\"

INVALID_IDENTIFIER          {DIGIT}+{IDENTIFIER}
UNTERMINATED_CHAR_LITERAL   \'([^'\\]|\\.)*$
UNTERMINATED_STRING_LITERAL \"([^"\\]|\\.)*(\\\n([^"\\]|\\.)*)*$

KEYWORDS        auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while

PREPROCESSOR    \#(include|define|undef|if|ifdef|ifndef|else|elif|endif|pragma)
OPERATORS       \+|\-|\*|\/|%|\+\+|\-\-|=|==|!=|<|>|<=|>=|\||&&|!|&|\^|~|\|\||<<|>>|\+=|\-=|\*=|\/=|%=|<<=|>>=|&=|\^=|\|=|\->
DELIMITERS      \(|\)|\{|\}|\[|\]|\;|\,|\.|\:

SINGLE_COMMENT  \/\/.*
MULTI_COMMENT   \/\*([^*]|\*+[^\/])*\*+\/

/* Rules Section */
%%
{KEYWORDS}       { printf("Type: KEYWORD - Line: %d - Value: %s\n", current_line_index, yytext); }
{IDENTIFIER}     { printf("Type: IDENTIFIER - Line: %d - Value: %s\n", current_line_index, yytext); }
{HEX_NUMBER}     { printf("Type: HEX_NUMBER - Line: %d - Value: %s\n", current_line_index, yytext); }
{OCT_NUMBER}     { printf("Type: OCT_NUMBER - Line: %d - Value: %s\n", current_line_index, yytext); }
{BIN_NUMBER}     { printf("Type: BIN_NUMBER - Line: %d - Value: %s\n", current_line_index, yytext); }
{DEC_NUMBER}     { printf("Type: DEC_NUMBER - Line: %d - Value: %s\n", current_line_index, yytext); }
{CHAR_LITERAL}   { printf("Type: CHAR_LITERAL - Line: %d - Value: %s\n", current_line_index, yytext); }
{STRING_LITERAL} { printf("Type: STRING_LITERAL - Line: %d - Value: %s\n", current_line_index, yytext); current_line_index += count_newlines(yytext); }
{PREPROCESSOR}   { printf("Type: PREPROCESSOR - Line: %d - Value: %s\n", current_line_index, yytext); }
{OPERATORS}      { printf("Type: OPERATOR - Line: %d - Value: %s\n", current_line_index, yytext); }
{DELIMITERS}     { printf("Type: DELIMITER - Line: %d - Value: %s\n", current_line_index, yytext); }

{SINGLE_COMMENT} { printf("Type: COMMENT - Line: %d - Value: %s\n", current_line_index, yytext); }
{MULTI_COMMENT}  { printf("Type: COMMENT - Line: %d - Value: %s\n", current_line_index, yytext); current_line_index += count_newlines(yytext); }

[ \t]+           ; /* Ignore space and tab characters */
\n               { current_line_index++; } /* New line characters */

{INVALID_IDENTIFIER}          { printf("Type: LEXICAL_ERROR - Reason: INVALID_IDENTIFIER - Line: %d - Value: %s\n", current_line_index, yytext); }
{UNTERMINATED_CHAR_LITERAL}   { printf("Type: LEXICAL_ERROR - Reason: UNTERMINATED_CHAR_LITERAL - Line: %d - Value: %s\n", current_line_index, yytext); }
{UNTERMINATED_STRING_LITERAL} { printf("Type: LEXICAL_ERROR - Reason: UNTERMINATED_STRING_LITERAL Line: %d - Value: %s\n", current_line_index, yytext); }
.                             { printf("Type: LEXICAL_ERROR - Reason: UNKNOWN - Line: %d - Value: %s\n", current_line_index, yytext); }

%%
/* Code Section */
int count_newlines(const char *str) {
    int count = 0;

    while (*str) {
        if (*str == '\n') {
            count++;
        }

        str++;
    }

    return count;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <C source code file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("File could not be opened");
        return 1;
    }

    yyin = input_file;
    yylex();

    fclose(input_file);
    return 0;
}