
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 73 "AS3.y"

	struct tree *t;



/* Line 1676 of yacc.c  */
#line 180 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


