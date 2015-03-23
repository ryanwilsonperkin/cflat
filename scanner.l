%{
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int col_num = 1;
int next_col_num = 1;
int line_num = 1;
int next_line_num = 1;
char *line_buf = NULL;
char **custom_types = NULL;
int n_custom_types = 0;

char get_c_val();
int check_custom_type();
void add_custom_type(char *);
void reset_line_buf();
void append_line_buf();
void increment_col_num();
void increment_line_num();
void reset_col_num();
%}

NATURAL [1-9]
DIGIT   [0-9]
LETTER  [a-zA-Z_]

ID      {LETTER}({LETTER}|{DIGIT})*
CHAR    '(\\n|\\0|.)'
FLOAT   -?0|({NATURAL}{DIGIT}*)\.{DIGIT}+([eE][-+]?{NATURAL}{DIGIT}+)?
INT     -?0|({NATURAL}{DIGIT}*)

%x COMMENT

%%

"/*"    { BEGIN(COMMENT); }
<COMMENT>"*/"    { BEGIN(INITIAL); }
<COMMENT>[^*\n]+ { append_line_buf(); increment_col_num(); }
<COMMENT>"*"     { append_line_buf(); increment_col_num(); }
<COMMENT>\n      { reset_line_buf(); reset_col_num(); increment_line_num(); }

{CHAR}     { append_line_buf(); increment_col_num(); yylval.cval = get_c_val(yytext); return CONST_CHAR; }
{FLOAT}    { append_line_buf(); increment_col_num(); yylval.fval = atof(yytext); return CONST_FLOAT; }
{INT}      { append_line_buf(); increment_col_num(); yylval.ival = atoi(yytext); return CONST_INT; }
"char"     { append_line_buf(); increment_col_num(); return CHAR; }
"float"    { append_line_buf(); increment_col_num(); return FLOAT; }
"int"      { append_line_buf(); increment_col_num(); return INT;}
"else"     { append_line_buf(); increment_col_num(); return ELSE; }
"for"      { append_line_buf(); increment_col_num(); return FOR; }
"if"       { append_line_buf(); increment_col_num(); return IF; }
"return"   { append_line_buf(); increment_col_num(); return RETURN; }
"struct"   { append_line_buf(); increment_col_num(); return STRUCT; }
"typedef"  { append_line_buf(); increment_col_num(); return TYPEDEF; }
"void"     { append_line_buf(); increment_col_num(); return VOID; }
"while"    { append_line_buf(); increment_col_num(); return WHILE; }
"sizeof"   { append_line_buf(); increment_col_num(); return SIZEOF; }
"+"        { append_line_buf(); increment_col_num(); return PLUS; }
"-"        { append_line_buf(); increment_col_num(); return MINUS; }
"*"        { append_line_buf(); increment_col_num(); return ASTERISK; }
"/"        { append_line_buf(); increment_col_num(); return SLASH; }
"%"        { append_line_buf(); increment_col_num(); return PERCENT; }
"=="       { append_line_buf(); increment_col_num(); return EQ; }
"!="       { append_line_buf(); increment_col_num(); return NE; }
"<"        { append_line_buf(); increment_col_num(); return LT; }
">"        { append_line_buf(); increment_col_num(); return GT; }
"<="       { append_line_buf(); increment_col_num(); return LE; }
">="       { append_line_buf(); increment_col_num(); return GE; }
"&&"       { append_line_buf(); increment_col_num(); return AND; }
"||"       { append_line_buf(); increment_col_num(); return OR; }
"!"        { append_line_buf(); increment_col_num(); return NOT; }
"="        { append_line_buf(); increment_col_num(); return ASSIGN; }
"++"       { append_line_buf(); increment_col_num(); return INCREMENT; }
"--"       { append_line_buf(); increment_col_num(); return DECREMENT; }
"{"        { append_line_buf(); increment_col_num(); return LBRACE; }
"}"        { append_line_buf(); increment_col_num(); return RBRACE; }
"["        { append_line_buf(); increment_col_num(); return LBRACKET; }
"]"        { append_line_buf(); increment_col_num(); return RBRACKET; }
"("        { append_line_buf(); increment_col_num(); return LPAREN; }
")"        { append_line_buf(); increment_col_num(); return RPAREN; }
";"        { append_line_buf(); increment_col_num(); return SEMICOLON; }
","        { append_line_buf(); increment_col_num(); return COMMA; }
"."        { append_line_buf(); increment_col_num(); return PERIOD; }
{ID}       { append_line_buf(); increment_col_num(); yylval.sval = strdup(yytext); return check_custom_type(); }
[ \t]+     { append_line_buf(); increment_col_num(); /* Ignore whitespace */ }
\n         { reset_line_buf(); reset_col_num(); increment_line_num(); }
.          { append_line_buf(); increment_col_num(); return UNRECOGNIZED; }

%%

char get_c_val(char *s)
{
        if (s[1] == '\\') {
                switch (s[2]) {
                case 'n': return '\n';
                case '0': return '\0';
                }
        }
        return s[1];
}

void add_custom_type(char *type_name)
{
        if (custom_types) {
            custom_types = realloc(custom_types, sizeof(char *) * (n_custom_types+1));
        } else {
            custom_types = malloc(sizeof(char *));
        }
        custom_types[n_custom_types++] = strdup(type_name);
}

int check_custom_type()
{
        int i;
        for (i = 0; i < n_custom_types; i++) {
                if (strcmp(yytext, custom_types[i]) == 0) {
                        return TYPE_NAME;
                }
        }
        return ID;
}

void reset_line_buf()
{
        free(line_buf);
        line_buf = malloc(1);
        line_buf[0] = 0;
}

void append_line_buf()
{
        if (line_buf) {
                line_buf = realloc(line_buf, strlen(line_buf) + yyleng + 1);
        } else {
                line_buf = malloc(yyleng + 1);
        }
        line_buf = strcat(line_buf, yytext);
}

void increment_col_num()
{
        col_num = next_col_num;
        next_col_num += yyleng;
}

void increment_line_num()
{
        line_num = next_line_num;
        next_line_num++;
}

void reset_col_num()
{
        col_num = next_col_num;
        next_col_num = 1;
}
