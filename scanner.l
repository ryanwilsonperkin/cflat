%{
#include <string.h>
#include "parser.h"
%}

NATURAL	[1-9]
DIGIT	[0-9]
LETTER	[a-zA-Z_]

ID	{LETTER}({LETTER}|{DIGIT})*
CHAR	'(\\n|\\0|.)'
FLOAT	{NATURAL}{DIGIT}*\.{DIGIT}+([eE][-+]?{NATURAL}{DIGIT}+)?
INT	{NATURAL}{DIGIT}*

%%

{CHAR}		{ return CONST_CHAR; }
{FLOAT}		{ return CONST_FLOAT; }
{INT}		{ return CONST_INT; }
"char"		{ return CHAR; }
"float"		{ return FLOAT; }
"int"		{ return INT;}
"else"		{ return ELSE; }
"for"		{ return FOR; }
"if"		{ return IF; }
"return"	{ return RETURN; }
"struct"	{ return STRUCT; }
"typedef"	{ return TYPEDEF; }
"void"		{ return VOID; }
"while"		{ return WHILE; }
"sizeof"	{ return SIZEOF; }
"+"		{ return PLUS; }
"-"		{ return MINUS; }
"*"		{ return ASTERISK; }
"/"		{ return SLASH; }
"%"		{ return PERCENT; }
"=="		{ return EQ; }
"!="		{ return NE; }
"<"		{ return LT; }
">"		{ return GT; }
"<="		{ return LE; }
">="		{ return GE; }
"&&"		{ return AND; }
"||"		{ return OR; }
"!"		{ return NOT; }
"="		{ return ASSIGN; }
"++"		{ return INCREMENT; }
"--"		{ return DECREMENT; }
"{"		{ return LBRACE; }
"}"		{ return RBRACE; }
"["		{ return LBRACKET; }
"]"		{ return RBRACKET; }
"("		{ return LPAREN; }
")"		{ return RPAREN; }
";"		{ return SEMICOLON; }
"."		{ return PERIOD; }
{ID}		{ yylval.sval = strdup(yytext); return ID; }
[ \t\n]+	{ /* Ignore whitespace */ }
