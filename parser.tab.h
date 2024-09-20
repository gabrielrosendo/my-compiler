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
     IF_KEYWORD = 262,
     ELSE_KEYWORD = 263,
     WHILE_KEYWORD = 264,
     RETURN_KEYWORD = 265,
     OPERATOR = 266,
     SYMBOL = 267,
     PARENTHESIS = 268,
     SEMICOLON = 269,
     EQ = 270,
     LBRACE = 271,
     RBRACE = 272
   };
#endif
/* Tokens.  */
#define NUMBER 258
#define ID 259
#define TYPE 260
#define PRINT_KEYWORD 261
#define IF_KEYWORD 262
#define ELSE_KEYWORD 263
#define WHILE_KEYWORD 264
#define RETURN_KEYWORD 265
#define OPERATOR 266
#define SYMBOL 267
#define PARENTHESIS 268
#define SEMICOLON 269
#define EQ 270
#define LBRACE 271
#define RBRACE 272




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "parser.y"
{
    int number;
    char character;
    char* string;
    char* operator;
    // struct ASTNode* ast;
}
/* Line 1529 of yacc.c.  */
#line 91 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

