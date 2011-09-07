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
     TOK_RETURN = 258,
     TOK_DOT = 259,
     TOK_CROSS = 260,
     TOK_STEP = 261,
     TOK_AT = 262,
     TOK_NEXT = 263,
     TOK_REAL = 264,
     TOK_IMAG = 265,
     TOK_ARRAY = 266,
     TOK_VECTOR = 267,
     TOK_MATRIX = 268,
     TOK_CUBE = 269,
     TOK_ARR_CONPERIM = 270,
     TOK_COMPLEX = 271,
     TOK_SUM = 272,
     TOK_MIN = 273,
     TOK_MAX = 274,
     TOK_RED_AND = 275,
     TOK_RED_OR = 276,
     TOK_MEDIAN = 277,
     TOK_CONCAT = 278,
     TOK_TILE = 279,
     TOK_EXTENTS = 280,
     TOK_IF = 281,
     TOK_ELSE = 282,
     TOK_ELIF = 283,
     TOK_STRING = 284,
     TOK_PRINT = 285,
     TOK_ASSERT = 286,
     TOK_IDENT = 287,
     TOK_INTNUM = 288,
     TOK_FOR = 289,
     TOK_WHILE = 290,
     TOK_GEN = 291,
     TOK_AND = 292,
     TOK_OR = 293,
     TOK_EQUAL = 294,
     TOK_NE = 295,
     TOK_LE = 296,
     TOK_GE = 297,
     TOK_INT = 298,
     TOK_BOOL = 299,
     TOK_FIX = 300,
     TOK_TRUE = 301,
     TOK_FALSE = 302,
     TOK_RIGHT_SHIFT = 303,
     TOK_LEFT_SHIFT = 304,
     TOK_FLOATNUM = 305,
     TOK_ARR_SUM = 306,
     TOK_ARR_MIN = 307,
     TOK_ARR_MAX = 308,
     TOK_ARR_RED_AND = 309,
     TOK_ARR_RED_OR = 310,
     TOK_ARR_MEDIAN = 311,
     TOK_WINDOW = 312,
     TOK_SWITCH = 313,
     TOK_CASE = 314,
     TOK_DEFAULT = 315,
     TOK_PRODUCT = 316,
     TOK_ARR_MAX_INDICES = 317,
     TOK_ARR_MIN_INDICES = 318,
     TOK_ARR_PRODUCT = 319,
     TOK_MEAN = 320,
     TOK_MODE = 321,
     TOK_BITS = 322,
     TOK_ARR_MEAN = 323,
     TOK_ARR_MODE = 324,
     TOK_ARR_HIST = 325,
     TOK_HIST = 326,
     TOK_ACCUM = 327,
     TOK_ARR_ACCUM = 328,
     TOK_ST_DEV = 329,
     TOK_ARR_ST_DEV = 330,
     TOK_FLOAT = 331,
     TOK_DOUBLE = 332,
     TOK_INTRINSIC = 333,
     TOK_ARR_CONCAT = 334,
     TOK_FINAL = 335,
     TOK_LOOP_INDICES = 336,
     TOK_VAL_AT_MINS = 337,
     TOK_VAL_AT_MAXS = 338,
     TOK_VAL_AT_FIRST_MIN = 339,
     TOK_VAL_AT_FIRST_MAX = 340,
     TOK_VAL_AT_LAST_MIN = 341,
     TOK_VAL_AT_LAST_MAX = 342,
     TOK_EXPORT = 343,
     TOK_PRAGMA = 344,
     TOK_NO_UNROLL = 345,
     TOK_NO_INLINE = 346,
     TOK_LOOKUP = 347,
     TOK_NO_DFG = 348,
     TOK_STRIPMINE = 349,
     TOK_NO_FUSE = 350,
     TOK_BLOCK = 351,
     TOK_VHDL = 352,
     UMINUS = 353
   };
#endif
/* Tokens.  */
#define TOK_RETURN 258
#define TOK_DOT 259
#define TOK_CROSS 260
#define TOK_STEP 261
#define TOK_AT 262
#define TOK_NEXT 263
#define TOK_REAL 264
#define TOK_IMAG 265
#define TOK_ARRAY 266
#define TOK_VECTOR 267
#define TOK_MATRIX 268
#define TOK_CUBE 269
#define TOK_ARR_CONPERIM 270
#define TOK_COMPLEX 271
#define TOK_SUM 272
#define TOK_MIN 273
#define TOK_MAX 274
#define TOK_RED_AND 275
#define TOK_RED_OR 276
#define TOK_MEDIAN 277
#define TOK_CONCAT 278
#define TOK_TILE 279
#define TOK_EXTENTS 280
#define TOK_IF 281
#define TOK_ELSE 282
#define TOK_ELIF 283
#define TOK_STRING 284
#define TOK_PRINT 285
#define TOK_ASSERT 286
#define TOK_IDENT 287
#define TOK_INTNUM 288
#define TOK_FOR 289
#define TOK_WHILE 290
#define TOK_GEN 291
#define TOK_AND 292
#define TOK_OR 293
#define TOK_EQUAL 294
#define TOK_NE 295
#define TOK_LE 296
#define TOK_GE 297
#define TOK_INT 298
#define TOK_BOOL 299
#define TOK_FIX 300
#define TOK_TRUE 301
#define TOK_FALSE 302
#define TOK_RIGHT_SHIFT 303
#define TOK_LEFT_SHIFT 304
#define TOK_FLOATNUM 305
#define TOK_ARR_SUM 306
#define TOK_ARR_MIN 307
#define TOK_ARR_MAX 308
#define TOK_ARR_RED_AND 309
#define TOK_ARR_RED_OR 310
#define TOK_ARR_MEDIAN 311
#define TOK_WINDOW 312
#define TOK_SWITCH 313
#define TOK_CASE 314
#define TOK_DEFAULT 315
#define TOK_PRODUCT 316
#define TOK_ARR_MAX_INDICES 317
#define TOK_ARR_MIN_INDICES 318
#define TOK_ARR_PRODUCT 319
#define TOK_MEAN 320
#define TOK_MODE 321
#define TOK_BITS 322
#define TOK_ARR_MEAN 323
#define TOK_ARR_MODE 324
#define TOK_ARR_HIST 325
#define TOK_HIST 326
#define TOK_ACCUM 327
#define TOK_ARR_ACCUM 328
#define TOK_ST_DEV 329
#define TOK_ARR_ST_DEV 330
#define TOK_FLOAT 331
#define TOK_DOUBLE 332
#define TOK_INTRINSIC 333
#define TOK_ARR_CONCAT 334
#define TOK_FINAL 335
#define TOK_LOOP_INDICES 336
#define TOK_VAL_AT_MINS 337
#define TOK_VAL_AT_MAXS 338
#define TOK_VAL_AT_FIRST_MIN 339
#define TOK_VAL_AT_FIRST_MAX 340
#define TOK_VAL_AT_LAST_MIN 341
#define TOK_VAL_AT_LAST_MAX 342
#define TOK_EXPORT 343
#define TOK_PRAGMA 344
#define TOK_NO_UNROLL 345
#define TOK_NO_INLINE 346
#define TOK_LOOKUP 347
#define TOK_NO_DFG 348
#define TOK_STRIPMINE 349
#define TOK_NO_FUSE 350
#define TOK_BLOCK 351
#define TOK_VHDL 352
#define UMINUS 353




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

