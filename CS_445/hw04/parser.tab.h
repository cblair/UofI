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
     PACKAGE = 258,
     PUBLIC = 259,
     PRIVATE = 260,
     PROTECTED = 261,
     INTERNAL = 262,
     OVERRIDE = 263,
     FUNCTION = 264,
     EXTENDS = 265,
     IMPLEMENTS = 266,
     VAR = 267,
     STATIC = 268,
     IF = 269,
     IMPORT = 270,
     FOR = 271,
     EACH = 272,
     IN = 273,
     WHILE = 274,
     DO = 275,
     SWITCH = 276,
     CASE = 277,
     DEFAULT = 278,
     ELSE = 279,
     CONST = 280,
     CLASS = 281,
     INTERFACE = 282,
     TRUE = 283,
     FALSE = 284,
     DYNAMIC = 285,
     USE = 286,
     XML = 287,
     NAMESPACE = 288,
     IS = 289,
     AS = 290,
     GET = 291,
     SET = 292,
     WITH = 293,
     RETURN = 294,
     CONTINUE = 295,
     BREAK = 296,
     NULL_VAL = 297,
     NEW = 298,
     SUPER = 299,
     INSTANCEOF = 300,
     DELETE = 301,
     VOID = 302,
     TYPEOF = 303,
     TRY = 304,
     CATCH = 305,
     FINALLY = 306,
     UNDEFINED = 307,
     THROW = 308,
     FINAL = 309,
     QUESTION = 310,
     LPAREN = 311,
     RPAREN = 312,
     LBRACK = 313,
     RBRACK = 314,
     LCURLY = 315,
     RCURLY = 316,
     COLON = 317,
     DBL_COLON = 318,
     COMMA = 319,
     ASSIGN = 320,
     EQUAL = 321,
     STRICT_EQUAL = 322,
     LNOT = 323,
     BNOT = 324,
     NOT_EQUAL = 325,
     STRICT_NOT_EQUAL = 326,
     DIV = 327,
     DIV_ASSIGN = 328,
     PLUS = 329,
     PLUS_ASSIGN = 330,
     INC = 331,
     MINUS = 332,
     MINUS_ASSIGN = 333,
     DEC = 334,
     STAR = 335,
     STAR_ASSIGN = 336,
     MOD = 337,
     MOD_ASSIGN = 338,
     SR = 339,
     SR_ASSIGN = 340,
     BSR = 341,
     BSR_ASSIGN = 342,
     GE = 343,
     GT = 344,
     SL = 345,
     SL_ASSIGN = 346,
     LE = 347,
     LT = 348,
     BXOR = 349,
     BXOR_ASSIGN = 350,
     BOR = 351,
     BOR_ASSIGN = 352,
     LOR = 353,
     BAND = 354,
     BAND_ASSIGN = 355,
     LAND = 356,
     LAND_ASSIGN = 357,
     LOR_ASSIGN = 358,
     E4X_ATTRI = 359,
     SEMI = 360,
     DOT = 361,
     E4X_DESC = 362,
     REST = 363,
     AND = 364,
     INTRINSIC = 365,
     OR = 366,
     IDENT = 367,
     EOFX = 368,
     ENUMERABLE = 369,
     EXPLICIT = 370,
     FLOAT_LITERAL = 371,
     DECIMAL_LITERAL = 372,
     OCTAL_LITERAL = 373,
     STRING_LITERAL = 374,
     HEX_LITERAL = 375,
     INCLUDE = 376,
     INCLUDE_DIRECTIVE = 377
   };
#endif
/* Tokens.  */
#define PACKAGE 258
#define PUBLIC 259
#define PRIVATE 260
#define PROTECTED 261
#define INTERNAL 262
#define OVERRIDE 263
#define FUNCTION 264
#define EXTENDS 265
#define IMPLEMENTS 266
#define VAR 267
#define STATIC 268
#define IF 269
#define IMPORT 270
#define FOR 271
#define EACH 272
#define IN 273
#define WHILE 274
#define DO 275
#define SWITCH 276
#define CASE 277
#define DEFAULT 278
#define ELSE 279
#define CONST 280
#define CLASS 281
#define INTERFACE 282
#define TRUE 283
#define FALSE 284
#define DYNAMIC 285
#define USE 286
#define XML 287
#define NAMESPACE 288
#define IS 289
#define AS 290
#define GET 291
#define SET 292
#define WITH 293
#define RETURN 294
#define CONTINUE 295
#define BREAK 296
#define NULL_VAL 297
#define NEW 298
#define SUPER 299
#define INSTANCEOF 300
#define DELETE 301
#define VOID 302
#define TYPEOF 303
#define TRY 304
#define CATCH 305
#define FINALLY 306
#define UNDEFINED 307
#define THROW 308
#define FINAL 309
#define QUESTION 310
#define LPAREN 311
#define RPAREN 312
#define LBRACK 313
#define RBRACK 314
#define LCURLY 315
#define RCURLY 316
#define COLON 317
#define DBL_COLON 318
#define COMMA 319
#define ASSIGN 320
#define EQUAL 321
#define STRICT_EQUAL 322
#define LNOT 323
#define BNOT 324
#define NOT_EQUAL 325
#define STRICT_NOT_EQUAL 326
#define DIV 327
#define DIV_ASSIGN 328
#define PLUS 329
#define PLUS_ASSIGN 330
#define INC 331
#define MINUS 332
#define MINUS_ASSIGN 333
#define DEC 334
#define STAR 335
#define STAR_ASSIGN 336
#define MOD 337
#define MOD_ASSIGN 338
#define SR 339
#define SR_ASSIGN 340
#define BSR 341
#define BSR_ASSIGN 342
#define GE 343
#define GT 344
#define SL 345
#define SL_ASSIGN 346
#define LE 347
#define LT 348
#define BXOR 349
#define BXOR_ASSIGN 350
#define BOR 351
#define BOR_ASSIGN 352
#define LOR 353
#define BAND 354
#define BAND_ASSIGN 355
#define LAND 356
#define LAND_ASSIGN 357
#define LOR_ASSIGN 358
#define E4X_ATTRI 359
#define SEMI 360
#define DOT 361
#define E4X_DESC 362
#define REST 363
#define AND 364
#define INTRINSIC 365
#define OR 366
#define IDENT 367
#define EOFX 368
#define ENUMERABLE 369
#define EXPLICIT 370
#define FLOAT_LITERAL 371
#define DECIMAL_LITERAL 372
#define OCTAL_LITERAL 373
#define STRING_LITERAL 374
#define HEX_LITERAL 375
#define INCLUDE 376
#define INCLUDE_DIRECTIVE 377




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 73 "AS3.y"
{
	struct tree *t;
}
/* Line 1529 of yacc.c.  */
#line 297 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

