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
     ARRAY = 260,
     BOOLEAN = 261,
     BREAK = 262,
     CASE = 263,
     CATCH = 264,
     CLASS = 265,
     CONST = 266,
     CONTINUE = 267,
     DEFAULT = 268,
     DELETE = 269,
     DO = 270,
     ELSE = 271,
     EXTENDS = 272,
     FALSE = 273,
     FINALLY = 274,
     FOR = 275,
     FUNCTION = 276,
     IF = 277,
     IMPLEMENTS = 278,
     IMPORT = 279,
     IN = 280,
     INSTANCEOF = 281,
     INT = 282,
     INTERFACE = 283,
     INTERNAL = 284,
     IS = 285,
     NEW = 286,
     NULL_TYPE = 287,
     NUMBER = 288,
     OBJECT = 289,
     PACKAGE = 290,
     PRIVATE = 291,
     PROTECTED = 292,
     PUBLIC = 293,
     RETURN = 294,
     STRING = 295,
     SUPER = 296,
     SWITCH = 297,
     THIS = 298,
     THROW = 299,
     TO = 300,
     TRUE = 301,
     TRY = 302,
     TYPEOF = 303,
     UINT = 304,
     USE = 305,
     VAR = 306,
     VOID = 307,
     WHILE = 308,
     WITH = 309,
     EACH = 310,
     GET = 311,
     SET = 312,
     NAMESPACE = 313,
     INCLUDE = 314,
     DYNAMIC = 315,
     FINAL = 316,
     NATIVE = 317,
     OVERRIDE = 318,
     PROTOTYPE = 319,
     STATIC = 320,
     PERIOD = 321,
     COMMA = 322,
     SEMICOLON = 323,
     COLON = 324,
     LC = 325,
     RC = 326,
     LP = 327,
     RP = 328,
     LB = 329,
     RB = 330,
     ASSIGN = 331,
     LT = 332,
     GT = 333,
     LTE = 334,
     GTE = 335,
     DLT = 336,
     DGT = 337,
     SUM = 338,
     DIF = 339,
     DIV = 340,
     MUL = 341,
     POW = 342,
     INCREMENT = 343,
     DECREMENT = 344,
     AMP = 345,
     DAMP = 346,
     PIPE = 347,
     DPIPE = 348,
     AT = 349,
     IDENT = 350,
     INTEGER = 351,
     ID = 352,
     INT_LIT = 353,
     NAME = 354,
     PUNC = 355,
     REAL_LIT = 356,
     STRING_LIT = 357
   };
#endif
/* Tokens.  */
#define EXIT 258
#define AS 259
#define ARRAY 260
#define BOOLEAN 261
#define BREAK 262
#define CASE 263
#define CATCH 264
#define CLASS 265
#define CONST 266
#define CONTINUE 267
#define DEFAULT 268
#define DELETE 269
#define DO 270
#define ELSE 271
#define EXTENDS 272
#define FALSE 273
#define FINALLY 274
#define FOR 275
#define FUNCTION 276
#define IF 277
#define IMPLEMENTS 278
#define IMPORT 279
#define IN 280
#define INSTANCEOF 281
#define INT 282
#define INTERFACE 283
#define INTERNAL 284
#define IS 285
#define NEW 286
#define NULL_TYPE 287
#define NUMBER 288
#define OBJECT 289
#define PACKAGE 290
#define PRIVATE 291
#define PROTECTED 292
#define PUBLIC 293
#define RETURN 294
#define STRING 295
#define SUPER 296
#define SWITCH 297
#define THIS 298
#define THROW 299
#define TO 300
#define TRUE 301
#define TRY 302
#define TYPEOF 303
#define UINT 304
#define USE 305
#define VAR 306
#define VOID 307
#define WHILE 308
#define WITH 309
#define EACH 310
#define GET 311
#define SET 312
#define NAMESPACE 313
#define INCLUDE 314
#define DYNAMIC 315
#define FINAL 316
#define NATIVE 317
#define OVERRIDE 318
#define PROTOTYPE 319
#define STATIC 320
#define PERIOD 321
#define COMMA 322
#define SEMICOLON 323
#define COLON 324
#define LC 325
#define RC 326
#define LP 327
#define RP 328
#define LB 329
#define RB 330
#define ASSIGN 331
#define LT 332
#define GT 333
#define LTE 334
#define GTE 335
#define DLT 336
#define DGT 337
#define SUM 338
#define DIF 339
#define DIV 340
#define MUL 341
#define POW 342
#define INCREMENT 343
#define DECREMENT 344
#define AMP 345
#define DAMP 346
#define PIPE 347
#define DPIPE 348
#define AT 349
#define IDENT 350
#define INTEGER 351
#define ID 352
#define INT_LIT 353
#define NAME 354
#define PUNC 355
#define REAL_LIT 356
#define STRING_LIT 357




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
#line 260 "ecma.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

