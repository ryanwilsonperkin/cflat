/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CHAR = 258,
     ELSE = 259,
     FLOAT = 260,
     FOR = 261,
     IF = 262,
     INT = 263,
     RETURN = 264,
     STRUCT = 265,
     TYPEDEF = 266,
     VOID = 267,
     WHILE = 268,
     LBRACE = 269,
     RBRACE = 270,
     LBRACKET = 271,
     RBRACKET = 272,
     LPAREN = 273,
     RPAREN = 274,
     SEMICOLON = 275,
     ASSIGN = 276,
     OR = 277,
     AND = 278,
     NOT = 279,
     EQ = 280,
     NE = 281,
     LT = 282,
     LE = 283,
     GT = 284,
     GE = 285,
     PLUS = 286,
     MINUS = 287,
     ASTERISK = 288,
     SLASH = 289,
     PERCENT = 290,
     SIZEOF = 291,
     INCREMENT = 292,
     DECREMENT = 293,
     PERIOD = 294,
     CONST_CHAR = 295,
     CONST_FLOAT = 296,
     CONST_INT = 297,
     ID = 298
   };
#endif
/* Tokens.  */
#define CHAR 258
#define ELSE 259
#define FLOAT 260
#define FOR 261
#define IF 262
#define INT 263
#define RETURN 264
#define STRUCT 265
#define TYPEDEF 266
#define VOID 267
#define WHILE 268
#define LBRACE 269
#define RBRACE 270
#define LBRACKET 271
#define RBRACKET 272
#define LPAREN 273
#define RPAREN 274
#define SEMICOLON 275
#define ASSIGN 276
#define OR 277
#define AND 278
#define NOT 279
#define EQ 280
#define NE 281
#define LT 282
#define LE 283
#define GT 284
#define GE 285
#define PLUS 286
#define MINUS 287
#define ASTERISK 288
#define SLASH 289
#define PERCENT 290
#define SIZEOF 291
#define INCREMENT 292
#define DECREMENT 293
#define PERIOD 294
#define CONST_CHAR 295
#define CONST_FLOAT 296
#define CONST_INT 297
#define ID 298




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 8 "parser.y"
{
    char cval;
    float fval;
    int ival;
    char *sval;
}
/* Line 1529 of yacc.c.  */
#line 142 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

