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
     NUMBER = 258,
     ID = 259,
     TYPE = 260,
     PRINT_KEYWORD = 261,
     PRINT = 262,
     IF_KEYWORD = 263,
     IF = 264,
     ELSE_KEYWORD = 265,
     ELSE = 266,
     WHILE_KEYWORD = 267,
     WHILE = 268,
     RETURN_KEYWORD = 269,
     RETURN = 270,
     OPERATOR = 271,
     ADD = 272,
     SUB = 273,
     MUL = 274,
     DIV = 275,
     SYMBOL = 276,
     PARENTHESIS = 277,
     SEMICOLON = 278,
     EQ = 279,
     LBRACE = 280,
     RBRACE = 281
   };
#endif
/* Tokens.  */
#define NUMBER 258
#define ID 259
#define TYPE 260
#define PRINT_KEYWORD 261
#define PRINT 262
#define IF_KEYWORD 263
#define IF 264
#define ELSE_KEYWORD 265
#define ELSE 266
#define WHILE_KEYWORD 267
#define WHILE 268
#define RETURN_KEYWORD 269
#define RETURN 270
#define OPERATOR 271
#define ADD 272
#define SUB 273
#define MUL 274
#define DIV 275
#define SYMBOL 276
#define PARENTHESIS 277
#define SEMICOLON 278
#define EQ 279
#define LBRACE 280
#define RBRACE 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 15 "parser.y"
{
    int number;
    char character;
    char* string;
    char* keyword;
    char* operator;
}
/* Line 1529 of yacc.c.  */
#line 109 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

