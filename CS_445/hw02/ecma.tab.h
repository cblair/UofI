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
     EXIT = 258,
     AS = 259,
     BREAK = 260,
     CASE = 261,
     CATCH = 262,
     CLASS = 263,
     CONST = 264,
     CONTINUE = 265,
     DEFAULT = 266,
     DELETE = 267,
     DO = 268,
     ELSE = 269,
     EXTENDS = 270,
     FALSE = 271,
     FINALLY = 272,
     FOR = 273,
     FUNCTION = 274,
     IF = 275,
     IMPLEMENTS = 276,
     IMPORT = 277,
     IN = 278,
     INSTANCEOF = 279,
     INTERFACE = 280,
     INTERNAL = 281,
     IS = 282,
     NEW = 283,
     NULL_VAL = 284,
     PACKAGE = 285,
     PRIVATE = 286,
     PROTECTED = 287,
     PUBLIC = 288,
     RETURN = 289,
     SUPER = 290,
     SWITCH = 291,
     THIS = 292,
     THROW = 293,
     TO = 294,
     TRUE = 295,
     TRY = 296,
     TYPEOF = 297,
     USE = 298,
     VAR = 299,
     VOID = 300,
     WHILE = 301,
     WITH = 302,
     EACH = 303,
     GET = 304,
     SET = 305,
     NAMESPACE = 306,
     INCLUDE = 307,
     DYNAMIC = 308,
     FINAL = 309,
     NATIVE = 310,
     OVERRIDE = 311,
     PROTOTYPE = 312,
     STATIC = 313,
     ARRAY = 314,
     INT = 315,
     UINT = 316,
     NUM = 317,
     STRING = 318,
     BOOL = 319,
     OOBJECT = 320,
     OB = 321,
     CB = 322,
     OP = 323,
     CP = 324,
     SQ = 325,
     DQ = 326,
     SUM = 327,
     DIV = 328,
     DIF = 329,
     MUL = 330,
     POW = 331,
     ASSIGN = 332,
     EQUIV = 333,
     NOTEQ = 334,
     LT = 335,
     GT = 336,
     LTEQ = 337,
     GTEQ = 338,
     COLON = 339,
     SEMICOLON = 340,
     COMMA = 341,
     IDENT = 342,
     INTEGER = 343,
     ID = 344,
     NAME = 345,
     PUNC = 346,
     MIX = 347
   };
#endif
/* Tokens.  */
#define EXIT 258
#define AS 259
#define BREAK 260
#define CASE 261
#define CATCH 262
#define CLASS 263
#define CONST 264
#define CONTINUE 265
#define DEFAULT 266
#define DELETE 267
#define DO 268
#define ELSE 269
#define EXTENDS 270
#define FALSE 271
#define FINALLY 272
#define FOR 273
#define FUNCTION 274
#define IF 275
#define IMPLEMENTS 276
#define IMPORT 277
#define IN 278
#define INSTANCEOF 279
#define INTERFACE 280
#define INTERNAL 281
#define IS 282
#define NEW 283
#define NULL_VAL 284
#define PACKAGE 285
#define PRIVATE 286
#define PROTECTED 287
#define PUBLIC 288
#define RETURN 289
#define SUPER 290
#define SWITCH 291
#define THIS 292
#define THROW 293
#define TO 294
#define TRUE 295
#define TRY 296
#define TYPEOF 297
#define USE 298
#define VAR 299
#define VOID 300
#define WHILE 301
#define WITH 302
#define EACH 303
#define GET 304
#define SET 305
#define NAMESPACE 306
#define INCLUDE 307
#define DYNAMIC 308
#define FINAL 309
#define NATIVE 310
#define OVERRIDE 311
#define PROTOTYPE 312
#define STATIC 313
#define ARRAY 314
#define INT 315
#define UINT 316
#define NUM 317
#define STRING 318
#define BOOL 319
#define OOBJECT 320
#define OB 321
#define CB 322
#define OP 323
#define CP 324
#define SQ 325
#define DQ 326
#define SUM 327
#define DIV 328
#define DIF 329
#define MUL 330
#define POW 331
#define ASSIGN 332
#define EQUIV 333
#define NOTEQ 334
#define LT 335
#define GT 336
#define LTEQ 337
#define GTEQ 338
#define COLON 339
#define SEMICOLON 340
#define COMMA 341
#define IDENT 342
#define INTEGER 343
#define ID 344
#define NAME 345
#define PUNC 346
#define MIX 347




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "ecma.y"
{
        double dval;
        int varindex;
        int ival;
        char *sval;
}
/* Line 1529 of yacc.c.  */
#line 240 "ecma.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

