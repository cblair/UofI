/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "gram.y"

#include "sassy.h"
#include "node.h"
#include "gram.tab.h"
#include "error.h"


void yyerror (char *s)
    {
    handle_error (ErrParse, Fatal, mylineno, filename, funcname);
    }

void fill_in_func_name (char *funcname, NODEPNT n)
    {
    int i;

    n->loc.func = funcname;

    for (i=0; i<NUM_CHILDREN; i++)
        if (n->child[i] != NULL)
	    fill_in_func_name (funcname, n->child[i]);

    if (n->link != NULL)
        fill_in_func_name (funcname, n->link);
    }

NODEPNT revlist_elifs (NODEPNT n)
    {
    NODEPNT m, tmp;

    tmp = NULL;

    while (n != NULL)
	{
	m = n->child[1]->child[2];
	n->child[1]->child[2] = tmp;
	tmp = n;
	n = m;
	}

    return tmp;
    }

NODEPNT revlist (NODEPNT n)
    {
    NODEPNT m, tmp;

    tmp = NULL;

    while (n != NULL)
	{
	m = n->link;
	n->link = tmp;
	tmp = n;
	n = m;
	}

    return tmp;
    }



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 364 "gram.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2543

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  123
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  76
/* YYNRULES -- Number of rules.  */
#define YYNRULES  241
/* YYNRULES -- Number of states.  */
#define YYNSTATES  539

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   353

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   110,     2,     2,     2,   109,   102,     2,
     114,   115,   107,   105,   113,   106,     2,   108,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    99,   112,
     103,   122,   104,    98,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   116,     2,   117,   101,   121,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   118,   100,   119,   120,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,   111
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,    11,    13,    17,    19,    22,
      30,    37,    44,    51,    57,    58,    63,    65,    69,    71,
      73,    76,    79,    81,    85,    88,    90,    94,    98,   100,
     102,   104,   106,   108,   110,   112,   115,   116,   120,   122,
     126,   128,   130,   141,   149,   150,   155,   157,   161,   163,
     165,   170,   175,   177,   180,   181,   183,   185,   188,   192,
     195,   199,   206,   213,   223,   224,   228,   230,   234,   236,
     238,   239,   244,   246,   250,   252,   254,   255,   260,   261,
     263,   268,   270,   274,   278,   280,   282,   286,   288,   291,
     293,   295,   299,   301,   303,   305,   309,   311,   315,   320,
     322,   327,   334,   343,   348,   355,   364,   373,   380,   389,
     396,   405,   412,   421,   428,   437,   444,   453,   460,   464,
     467,   470,   473,   476,   479,   482,   485,   488,   491,   494,
     497,   500,   503,   506,   508,   510,   512,   514,   516,   518,
     520,   522,   524,   526,   528,   530,   532,   534,   536,   538,
     540,   541,   544,   549,   556,   561,   569,   577,   580,   582,
     586,   588,   590,   592,   596,   601,   606,   615,   620,   629,
     633,   635,   640,   642,   644,   646,   648,   650,   652,   661,
     667,   672,   677,   681,   685,   689,   693,   697,   701,   705,
     709,   713,   717,   721,   725,   729,   733,   737,   741,   745,
     749,   752,   755,   760,   764,   766,   768,   770,   779,   780,
     783,   788,   790,   794,   796,   799,   800,   804,   809,   816,
     825,   828,   832,   836,   838,   842,   854,   863,   869,   870,
     873,   879,   887,   892,   894,   898,   900,   902,   904,   908,
     912,   913
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     124,     0,    -1,   125,   127,    -1,    -1,    88,   126,   112,
      -1,    32,    -1,   126,   113,    32,    -1,   128,    -1,   127,
     128,    -1,   129,   132,   114,   135,   115,   193,   112,    -1,
     129,   132,   114,   115,   193,   112,    -1,   129,   132,   114,
     133,   115,   112,    -1,   129,   132,   114,   135,   115,   112,
      -1,   129,   132,   114,   115,   112,    -1,    -1,    89,   114,
     130,   115,    -1,   131,    -1,   130,   113,   131,    -1,    91,
      -1,    92,    -1,    97,    29,    -1,   133,    32,    -1,   134,
      -1,   133,   113,   134,    -1,   138,   139,    -1,   136,    -1,
     135,   113,   136,    -1,   138,    32,   139,    -1,    43,    -1,
      45,    -1,    67,    -1,    44,    -1,    76,    -1,    77,    -1,
     137,    -1,    16,   137,    -1,    -1,   116,   140,   117,    -1,
     141,    -1,   140,   113,   141,    -1,    99,    -1,    33,    -1,
     143,    34,   146,   118,   174,   119,     3,   114,   167,   115,
      -1,   143,    34,   146,     3,   114,   167,   115,    -1,    -1,
      89,   114,   144,   115,    -1,   145,    -1,   144,   113,   145,
      -1,    90,    -1,    93,    -1,    94,   114,   183,   115,    -1,
      96,   114,   183,   115,    -1,    95,    -1,   150,   147,    -1,
      -1,   148,    -1,   149,    -1,     4,   150,    -1,   148,     4,
     150,    -1,     5,   150,    -1,   149,     5,   150,    -1,    32,
     151,    36,   179,   154,   158,    -1,   164,    36,   116,   160,
     117,   157,    -1,    57,    32,   116,   166,   117,    36,   179,
     157,   158,    -1,    -1,   114,   152,   115,    -1,   153,    -1,
     152,   113,   153,    -1,    99,    -1,   120,    -1,    -1,     6,
     114,   155,   115,    -1,   156,    -1,   155,   113,   156,    -1,
     121,    -1,   179,    -1,    -1,     6,   114,   178,   115,    -1,
      -1,   159,    -1,     7,   114,   164,   115,    -1,   161,    -1,
     160,   113,   161,    -1,   179,   120,   179,    -1,   179,    -1,
     163,    -1,   162,   113,   163,    -1,   136,    -1,     8,    32,
      -1,   121,    -1,   165,    -1,   164,   113,   165,    -1,   136,
      -1,   121,    -1,   179,    -1,   166,   113,   179,    -1,   168,
      -1,   167,   113,   168,    -1,    80,   114,    32,   115,    -1,
     169,    -1,   173,   114,   179,   115,    -1,   173,   114,   179,
     113,   179,   115,    -1,    72,   114,   169,   113,   179,   113,
     179,   115,    -1,   172,   114,   179,   115,    -1,   172,   114,
     179,   113,   179,   115,    -1,   172,   114,   179,   113,   179,
     113,   179,   115,    -1,    82,   114,   179,   113,   170,   113,
     179,   115,    -1,    82,   114,   179,   113,   170,   115,    -1,
      83,   114,   179,   113,   170,   113,   179,   115,    -1,    83,
     114,   179,   113,   170,   115,    -1,    84,   114,   179,   113,
     170,   113,   179,   115,    -1,    84,   114,   179,   113,   170,
     115,    -1,    85,   114,   179,   113,   170,   113,   179,   115,
      -1,    85,   114,   179,   113,   170,   115,    -1,    86,   114,
     179,   113,   170,   113,   179,   115,    -1,    86,   114,   179,
     113,   170,   115,    -1,    87,   114,   179,   113,   170,   113,
     179,   115,    -1,    87,   114,   179,   113,   170,   115,    -1,
     118,   178,   119,    -1,   143,    51,    -1,   143,    52,    -1,
     143,    53,    -1,   143,    54,    -1,   143,    55,    -1,   143,
      56,    -1,   143,    62,    -1,   143,    63,    -1,   143,    64,
      -1,   143,    68,    -1,   143,    75,    -1,   143,    69,    -1,
     143,    70,    -1,   143,    79,    -1,    17,    -1,    18,    -1,
      19,    -1,    20,    -1,    21,    -1,    22,    -1,    61,    -1,
      65,    -1,    74,    -1,    66,    -1,    71,    -1,    11,    -1,
      12,    -1,    13,    -1,    14,    -1,    23,    -1,    24,    -1,
      -1,   174,   175,    -1,   162,   122,   178,   112,    -1,   162,
     122,    81,   114,   115,   112,    -1,   162,   122,   187,   112,
      -1,    30,   114,   179,   113,   176,   115,   112,    -1,    31,
     114,   179,   113,   176,   115,   112,    -1,     1,   112,    -1,
     177,    -1,   176,   113,   177,    -1,    32,    -1,    29,    -1,
     179,    -1,   178,   113,   179,    -1,    32,   114,   178,   115,
      -1,    78,   114,   178,   115,    -1,    15,   114,   179,   113,
     179,   113,   179,   115,    -1,    25,   114,    32,   115,    -1,
     118,   175,   174,   119,     3,   114,   178,   115,    -1,    32,
     114,   115,    -1,    32,    -1,    32,   116,   194,   117,    -1,
      33,    -1,    50,    -1,    46,    -1,    47,    -1,   186,    -1,
     180,    -1,    73,   114,   186,   113,   179,   113,   179,   115,
      -1,   114,   179,   113,   179,   115,    -1,     9,   114,   179,
     115,    -1,    10,   114,   179,   115,    -1,   179,   105,   179,
      -1,   179,   106,   179,    -1,   179,   107,   179,    -1,   179,
     108,   179,    -1,   179,   109,   179,    -1,   179,   103,   179,
      -1,   179,   104,   179,    -1,   179,    41,   179,    -1,   179,
      42,   179,    -1,   179,    40,   179,    -1,   179,    39,   179,
      -1,   179,    37,   179,    -1,   179,    38,   179,    -1,   179,
     102,   179,    -1,   179,   100,   179,    -1,   179,   101,   179,
      -1,   179,    49,   179,    -1,   179,    48,   179,    -1,   110,
     179,    -1,   106,   179,    -1,   114,   134,   115,   179,    -1,
     114,   179,   115,    -1,   142,    -1,   189,    -1,   190,    -1,
      58,   114,   179,   115,   118,   181,   185,   119,    -1,    -1,
     181,   182,    -1,    59,   183,    99,   193,    -1,   184,    -1,
     183,   113,   184,    -1,    33,    -1,   106,    33,    -1,    -1,
      60,    99,   193,    -1,   171,   114,   179,   115,    -1,   171,
     114,   179,   113,   179,   115,    -1,   171,   114,   179,   113,
     179,   113,   179,   115,    -1,   118,   119,    -1,   118,   178,
     119,    -1,   118,   188,   119,    -1,   187,    -1,   188,   113,
     187,    -1,    35,   114,   179,   115,   118,   174,   119,     3,
     114,   167,   115,    -1,    26,   114,   179,   115,   193,   191,
      27,   193,    -1,   179,    98,   179,    99,   179,    -1,    -1,
     191,   192,    -1,    28,   114,   179,   115,   193,    -1,   118,
     174,   119,     3,   114,   178,   115,    -1,     3,   114,   178,
     115,    -1,   195,    -1,   194,   113,   195,    -1,   179,    -1,
     196,    -1,   197,    -1,   197,    99,   179,    -1,   198,    99,
     198,    -1,    -1,   179,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    93,    93,    98,    99,   101,   105,   110,   112,   118,
     127,   136,   145,   157,   167,   168,   171,   173,   179,   184,
     189,   196,   205,   207,   213,   228,   230,   236,   253,   255,
     257,   259,   265,   271,   278,   280,   305,   306,   309,   311,
     317,   322,   331,   340,   350,   351,   354,   356,   362,   367,
     372,   377,   382,   388,   399,   400,   407,   415,   419,   425,
     429,   435,   476,   481,   504,   505,   508,   510,   516,   521,
     527,   528,   531,   533,   539,   544,   547,   548,   551,   552,
     555,   558,   560,   566,   571,   574,   576,   582,   584,   590,
     596,   598,   604,   606,   612,   614,   620,   622,   628,   630,
     632,   637,   646,   666,   674,   683,   693,   698,   703,   708,
     713,   718,   723,   728,   733,   738,   743,   748,   754,   759,
     765,   771,   777,   783,   789,   795,   801,   807,   813,   819,
     825,   831,   837,   844,   849,   854,   859,   864,   869,   874,
     879,   884,   889,   894,   900,   905,   910,   915,   920,   925,
     931,   934,   945,   950,   958,   971,   976,   981,   984,   986,
     992,   994,   997,   999,  1005,  1010,  1015,  1020,  1025,  1032,
    1037,  1039,  1044,  1046,  1048,  1053,  1058,  1060,  1062,  1077,
    1082,  1087,  1092,  1097,  1102,  1107,  1112,  1117,  1122,  1127,
    1132,  1137,  1142,  1147,  1152,  1157,  1162,  1167,  1172,  1177,
    1182,  1187,  1204,  1209,  1211,  1213,  1215,  1218,  1228,  1229,
    1235,  1241,  1243,  1249,  1258,  1268,  1269,  1274,  1285,  1294,
    1305,  1310,  1315,  1321,  1323,  1329,  1338,  1347,  1358,  1359,
    1365,  1374,  1379,  1385,  1387,  1393,  1395,  1398,  1400,  1406,
    1412,  1413
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_RETURN", "TOK_DOT", "TOK_CROSS",
  "TOK_STEP", "TOK_AT", "TOK_NEXT", "TOK_REAL", "TOK_IMAG", "TOK_ARRAY",
  "TOK_VECTOR", "TOK_MATRIX", "TOK_CUBE", "TOK_ARR_CONPERIM",
  "TOK_COMPLEX", "TOK_SUM", "TOK_MIN", "TOK_MAX", "TOK_RED_AND",
  "TOK_RED_OR", "TOK_MEDIAN", "TOK_CONCAT", "TOK_TILE", "TOK_EXTENTS",
  "TOK_IF", "TOK_ELSE", "TOK_ELIF", "TOK_STRING", "TOK_PRINT",
  "TOK_ASSERT", "TOK_IDENT", "TOK_INTNUM", "TOK_FOR", "TOK_WHILE",
  "TOK_GEN", "TOK_AND", "TOK_OR", "TOK_EQUAL", "TOK_NE", "TOK_LE",
  "TOK_GE", "TOK_INT", "TOK_BOOL", "TOK_FIX", "TOK_TRUE", "TOK_FALSE",
  "TOK_RIGHT_SHIFT", "TOK_LEFT_SHIFT", "TOK_FLOATNUM", "TOK_ARR_SUM",
  "TOK_ARR_MIN", "TOK_ARR_MAX", "TOK_ARR_RED_AND", "TOK_ARR_RED_OR",
  "TOK_ARR_MEDIAN", "TOK_WINDOW", "TOK_SWITCH", "TOK_CASE", "TOK_DEFAULT",
  "TOK_PRODUCT", "TOK_ARR_MAX_INDICES", "TOK_ARR_MIN_INDICES",
  "TOK_ARR_PRODUCT", "TOK_MEAN", "TOK_MODE", "TOK_BITS", "TOK_ARR_MEAN",
  "TOK_ARR_MODE", "TOK_ARR_HIST", "TOK_HIST", "TOK_ACCUM", "TOK_ARR_ACCUM",
  "TOK_ST_DEV", "TOK_ARR_ST_DEV", "TOK_FLOAT", "TOK_DOUBLE",
  "TOK_INTRINSIC", "TOK_ARR_CONCAT", "TOK_FINAL", "TOK_LOOP_INDICES",
  "TOK_VAL_AT_MINS", "TOK_VAL_AT_MAXS", "TOK_VAL_AT_FIRST_MIN",
  "TOK_VAL_AT_FIRST_MAX", "TOK_VAL_AT_LAST_MIN", "TOK_VAL_AT_LAST_MAX",
  "TOK_EXPORT", "TOK_PRAGMA", "TOK_NO_UNROLL", "TOK_NO_INLINE",
  "TOK_LOOKUP", "TOK_NO_DFG", "TOK_STRIPMINE", "TOK_NO_FUSE", "TOK_BLOCK",
  "TOK_VHDL", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'!'", "UMINUS", "';'", "','", "'('", "')'",
  "'['", "']'", "'{'", "'}'", "'~'", "'_'", "'='", "$accept", "program",
  "exports", "export_list", "funcs", "func", "opt_func_prag",
  "func_pragmas", "func_pragma", "header", "types_wo_ids", "type_wo_id",
  "types_w_ids", "type_w_id", "base_type", "type", "optbrackets",
  "optsizes", "optsize", "for_loop", "opt_loop_prag", "loop_pragmas",
  "loop_pragma", "generators", "opt_cmpnds", "more_dots", "more_crosses",
  "simple_gen", "opt_extract", "colon_twids", "colon_twid",
  "opt_elegen_step", "opt_exprs", "emp_expr", "opt_expr_step", "opt_at",
  "at_spec", "cl2", "ele2", "cl9", "ele9", "cl4", "ele4", "cl7",
  "ret_exprs", "ret_expr", "loop_reduction", "value_cluster",
  "arr_reduce_op", "reduce_op", "struct_op", "stmts", "stmt",
  "pr_entities", "pr_entity", "exprs", "expr", "switch", "cases", "case",
  "const_list", "constval", "opt_default", "arr_reduce_expr", "arr_slice",
  "slices", "while_loop", "conditional", "elifs", "elif", "body_w_return",
  "array_indices", "array_index", "triple", "colon_bounds", "opt_expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,    63,    58,
     124,    94,    38,    60,    62,    43,    45,    42,    47,    37,
      33,   353,    59,    44,    40,    41,    91,    93,   123,   125,
     126,    95,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   123,   124,   125,   125,   126,   126,   127,   127,   128,
     128,   128,   128,   128,   129,   129,   130,   130,   131,   131,
     131,   132,   133,   133,   134,   135,   135,   136,   137,   137,
     137,   137,   137,   137,   138,   138,   139,   139,   140,   140,
     141,   141,   142,   142,   143,   143,   144,   144,   145,   145,
     145,   145,   145,   146,   147,   147,   147,   148,   148,   149,
     149,   150,   150,   150,   151,   151,   152,   152,   153,   153,
     154,   154,   155,   155,   156,   156,   157,   157,   158,   158,
     159,   160,   160,   161,   161,   162,   162,   163,   163,   163,
     164,   164,   165,   165,   166,   166,   167,   167,   168,   168,
     168,   168,   168,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   170,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   173,   173,   173,   173,   173,   173,
     174,   174,   175,   175,   175,   175,   175,   175,   176,   176,
     177,   177,   178,   178,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   180,   181,   181,
     182,   183,   183,   184,   184,   185,   185,   186,   186,   186,
     187,   187,   187,   188,   188,   189,   190,   190,   191,   191,
     192,   193,   193,   194,   194,   195,   195,   196,   196,   197,
     198,   198
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     3,     1,     3,     1,     2,     7,
       6,     6,     6,     5,     0,     4,     1,     3,     1,     1,
       2,     2,     1,     3,     2,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     3,     1,     3,
       1,     1,    10,     7,     0,     4,     1,     3,     1,     1,
       4,     4,     1,     2,     0,     1,     1,     2,     3,     2,
       3,     6,     6,     9,     0,     3,     1,     3,     1,     1,
       0,     4,     1,     3,     1,     1,     0,     4,     0,     1,
       4,     1,     3,     3,     1,     1,     3,     1,     2,     1,
       1,     3,     1,     1,     1,     3,     1,     3,     4,     1,
       4,     6,     8,     4,     6,     8,     8,     6,     8,     6,
       8,     6,     8,     6,     8,     6,     8,     6,     3,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     2,     4,     6,     4,     7,     7,     2,     1,     3,
       1,     1,     1,     3,     4,     4,     8,     4,     8,     3,
       1,     4,     1,     1,     1,     1,     1,     1,     8,     5,
       4,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     4,     3,     1,     1,     1,     8,     0,     2,
       4,     1,     3,     1,     2,     0,     3,     4,     6,     8,
       2,     3,     3,     1,     3,    11,     8,     5,     0,     2,
       5,     7,     4,     1,     3,     1,     1,     1,     3,     3,
       0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,    14,     5,     0,     1,     0,    14,     7,
       0,     4,     0,     0,     8,     0,    28,    31,    29,    30,
      32,    33,     0,     0,    22,    34,    36,     6,    18,    19,
       0,     0,    16,    35,     0,    21,     0,     0,    24,    20,
       0,    15,     0,     0,     0,    25,    36,    23,    41,    40,
       0,    38,    17,     0,    13,   150,     0,     0,     0,     0,
      36,     0,    37,    44,     0,    10,    11,    26,     0,    12,
       0,    27,    39,     0,     0,     0,     0,     0,   170,   172,
       0,   174,   175,   173,     0,     0,     0,     0,    44,    44,
      44,     0,   204,     0,     0,     0,   162,   177,   176,   205,
     206,     0,     0,     0,     0,     0,    89,    87,     0,    85,
     151,     9,    44,    44,    44,     0,    44,    44,    44,    44,
      44,    44,    44,     0,   201,   200,     0,     0,   150,     0,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     130,   131,   129,   132,    44,    44,   232,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,   157,    88,    44,    44,
       0,     0,    44,     0,     0,     0,     0,     0,   169,     0,
     235,     0,   233,   236,   237,     0,     0,     0,     0,     0,
       0,    48,    49,     0,    52,     0,     0,    46,    44,    44,
     203,     0,    64,     0,    93,    92,     0,    54,     0,    90,
       0,   163,   193,   194,   192,   191,   189,   190,   199,   198,
       0,   196,   197,   195,   187,   188,   182,   183,   184,   185,
     186,     0,     0,    44,    86,     0,     0,     0,     0,   180,
     181,    44,   167,     0,   164,    44,   171,    44,    44,     0,
       0,    44,   165,     0,     0,     0,    45,   202,     0,     0,
       0,     0,     0,     0,   150,     0,     0,    53,    55,    56,
       0,     0,    44,   217,    44,     0,     0,     0,     0,   220,
       0,   223,     0,   152,   154,     0,   228,   234,   238,   241,
     239,   150,   208,     0,   213,     0,     0,   211,     0,    47,
     179,     0,    68,    69,     0,    66,    44,    44,     0,     0,
      57,    59,     0,     0,    44,    91,     0,   227,   161,   160,
       0,   158,     0,   231,     0,   221,     0,   222,    44,     0,
       0,   215,    44,   214,     0,    50,    51,    44,     0,    65,
      70,     0,    94,   144,   145,   146,   147,   133,   134,   135,
     136,   137,   138,   148,   149,   139,   140,   142,   143,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      99,     0,     0,     0,    58,    60,     0,    81,    84,    44,
     218,     0,     0,     0,   153,    44,   224,     0,     0,     0,
     229,     0,     0,     0,   209,     0,     0,   212,     0,    67,
       0,    78,    44,     0,     0,     0,    44,    44,    44,    44,
      44,    44,     0,    43,    44,    44,     0,    44,    76,    44,
       0,   159,   155,   156,   166,   226,    44,     0,     0,     0,
     207,   178,   168,    44,     0,    61,    79,    95,    44,     0,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
       0,    82,     0,    62,    83,   219,     0,     0,     0,   216,
      74,     0,    72,    75,     0,    76,    44,    98,     0,     0,
       0,     0,     0,     0,    44,   103,    44,   100,     0,    44,
       0,     0,   210,    44,    71,     0,    78,     0,    44,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,   230,
     225,    73,    80,    63,    44,     0,    44,   107,    44,   109,
      44,   111,    44,   113,    44,   115,    44,   117,    44,   104,
     101,    77,     0,   118,     0,     0,     0,     0,     0,     0,
       0,   102,   106,   108,   110,   112,   114,   116,   105
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     5,     8,     9,    10,    31,    32,    22,
      23,    24,    44,   107,    25,    68,    38,    50,    51,    92,
      93,   196,   197,   206,   267,   268,   269,   207,   261,   304,
     305,   401,   461,   462,   453,   435,   436,   376,   377,   108,
     109,   208,   209,   341,   368,   369,   370,   489,    94,   371,
     372,    64,   110,   320,   321,   280,    96,    97,   331,   394,
     296,   297,   395,    98,   281,   282,    99,   100,   329,   390,
      56,   181,   182,   183,   184,   185
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -431
static const yytype_int16 yypact[] =
{
     -66,    46,    97,    17,  -431,    77,  -431,    51,    13,  -431,
     356,  -431,   138,   -42,  -431,   187,  -431,  -431,  -431,  -431,
    -431,  -431,    59,   -17,  -431,  -431,    66,  -431,  -431,  -431,
     149,   -62,  -431,  -431,   338,  -431,   356,   -15,  -431,  -431,
     -42,  -431,     6,    28,    37,  -431,   -18,  -431,  -431,  -431,
     -59,  -431,  -431,    90,  -431,  -431,   105,   134,   356,     7,
      66,   -15,  -431,   923,    78,  -431,  -431,  -431,   217,  -431,
     147,  -431,  -431,   148,   151,   168,   175,   185,    22,  -431,
     186,  -431,  -431,  -431,   192,   196,   198,   199,   923,   923,
     656,   272,  -431,  2404,   211,    43,  2309,  -431,  -431,  -431,
    -431,   174,   269,   214,   216,   301,  -431,  -431,   -74,  -431,
    -431,  -431,   923,   923,   923,   300,   923,   762,   817,   923,
     923,   245,   923,   -52,  -431,  -431,   226,  1051,  -431,   308,
    -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,   923,   923,  -431,   923,   923,   923,
     923,   923,   923,   923,   923,   923,   923,   923,   923,   923,
     923,   923,   923,   923,   923,   923,  -431,  -431,   923,   923,
     228,   184,   872,  1221,  1300,  1926,   229,  1316,  -431,    49,
    2283,   -14,  -431,  -431,   244,   247,  1332,  1348,  2464,   234,
      61,  -431,  -431,   241,  -431,   242,    62,  -431,   923,   923,
    -431,   104,   243,   326,  -431,  -431,     8,   208,   -20,  -431,
    1078,  2309,  2386,   529,   604,   604,  2405,  2405,    23,    23,
    2296,   485,   645,   805,  2405,  2405,    60,    60,  -431,  -431,
    -431,  1940,  1954,   923,  -431,   246,   430,   181,   249,  -431,
    -431,   923,  -431,     9,  -431,   817,  -431,   923,   711,   248,
     252,   923,  -431,   -16,   -16,   -52,  -431,  -431,  1427,   359,
     -80,   327,   251,   250,  -431,   308,   308,  -431,   367,   368,
     260,   292,   923,  -431,   923,    75,    75,    70,   262,  -431,
     -67,  -431,   -28,  -431,  -431,  1968,  -431,  -431,  2309,  2309,
    -431,  -431,  -431,  2045,  -431,   346,    80,  -431,    81,  -431,
    -431,   266,  -431,  -431,    92,  -431,   923,   923,  1875,   171,
    -431,  -431,   308,   308,   923,  -431,  1094,  2309,  -431,  -431,
      95,  -431,    96,  -431,   275,  -431,   271,  -431,   923,   268,
     190,   238,   923,  -431,   -16,  -431,  -431,   923,   -80,  -431,
     965,    20,  2309,  -431,  -431,  -431,  -431,  -431,  -431,  -431,
    -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431,   276,
    -431,   277,   278,   280,   282,   283,   284,   288,   109,  -431,
    -431,   290,   293,   403,  -431,  -431,    27,  -431,  1005,   923,
    -431,    75,   296,   297,  -431,   550,  -431,  1443,     9,   298,
    -431,   407,   -16,   312,  -431,   302,  1459,  -431,   126,  -431,
     303,   412,   923,   391,  2358,   402,   923,   923,   923,   923,
     923,   923,  1875,  -431,   923,   923,   322,   923,   431,   923,
    1475,  -431,  -431,  -431,  -431,  -431,   923,   328,   -76,     9,
    -431,  -431,  -431,   506,   329,  -431,  -431,  2309,   923,   331,
     332,  2059,  2073,  2087,  2164,  2178,  2192,  -431,  1173,  1189,
    1875,  -431,   334,  -431,  2309,  -431,  1554,  1875,     9,  -431,
    -431,   130,  -431,  2309,   292,   978,   923,  -431,   323,   323,
     323,   323,   323,   323,   923,  -431,   923,  -431,   137,   923,
       9,   140,  -431,   506,  -431,   143,   412,  2206,   923,   155,
     156,   159,   162,   163,   166,  1205,  1570,  -431,   172,  -431,
    -431,  -431,  -431,  -431,   923,   -24,   923,  -431,   923,  -431,
     923,  -431,   923,  -431,   923,  -431,   923,  -431,   923,  -431,
    -431,  -431,  1586,  -431,  1602,  1681,  1697,  1713,  1729,  1808,
    1824,  -431,  -431,  -431,  -431,  -431,  -431,  -431,  -431
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -431,  -431,  -431,  -431,  -431,   442,  -431,  -431,   411,  -431,
     418,    -2,  -431,   -29,   439,    11,   397,  -431,   398,  -431,
     345,  -431,   212,  -431,  -431,  -431,  -431,  -230,  -431,  -431,
     131,  -431,  -431,   -13,     3,    -8,  -431,  -431,    54,  -431,
     316,    15,   201,  -431,  -430,    79,    85,  -356,  -431,  -431,
    -431,  -125,   -85,   219,   115,   -30,   -88,  -431,  -431,  -431,
    -246,   167,  -431,   369,  -165,  -431,  -431,  -431,  -431,  -431,
     -55,  -431,   257,  -431,  -431,   256
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -242
static const yytype_int16 yytable[] =
{
     124,   125,   127,   201,    70,    45,   128,   238,   298,    53,
      53,   263,    53,    -2,    60,    35,   270,   294,    48,   302,
     478,    26,     1,   458,   173,   174,   175,   481,   177,    67,
     180,   186,   187,    95,    47,   310,   311,   334,   191,   171,
     303,   192,   193,   194,   195,    46,   145,    26,   172,    28,
      29,    40,   325,    41,    61,    30,   210,   211,    62,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,     4,   101,
     231,   232,   374,   375,    49,   326,   102,   179,   126,   145,
     295,   327,   190,   271,    15,   523,    36,     6,    37,   245,
     205,    26,     7,   246,   318,   101,     7,   319,   103,   104,
     257,   258,   102,   490,   491,   492,   493,   494,    54,    69,
      15,    16,    17,    18,    55,    55,   264,    55,   161,   162,
     163,   164,   165,   402,   103,   104,   117,   403,   118,   309,
     417,    36,   237,    57,   418,    19,   428,    16,    17,    18,
      58,   128,    59,   285,    20,    21,   145,   180,   146,   288,
     289,   386,   145,   293,   244,    13,   330,   163,   164,   165,
      27,    19,   101,    34,   145,   255,   252,   256,    39,   102,
      20,    21,    37,   145,   316,   323,   317,    15,   286,    11,
      12,   101,   102,   334,   334,   335,   336,   105,   102,   106,
      15,   103,   104,   277,    63,   338,    15,   339,   381,   381,
     382,   383,   265,   266,    16,    17,    18,    65,   340,   342,
     103,   104,   412,   259,   413,   106,   378,    16,    17,    18,
      16,    17,    18,    16,    17,    18,   205,   205,    19,   145,
     387,   432,   205,   483,   396,   484,    66,    20,    21,    60,
     412,    19,   497,   412,    19,   500,   271,    19,   502,   111,
      20,    21,   112,    20,    21,   113,    20,    21,   506,   508,
     507,   509,   510,   101,   511,   512,   514,   513,   515,   516,
     102,   517,   114,   205,   205,   145,   166,   521,    15,   115,
     373,   420,   106,   283,   145,   388,   389,   392,   393,   116,
     119,   167,   103,   104,   170,   106,   120,   398,    15,   391,
     121,   106,   122,   123,   437,    16,    17,    18,   441,   442,
     443,   444,   445,   446,    15,   144,   448,   449,   168,   378,
     169,   454,   176,   425,    87,    16,    17,    18,   456,    19,
     202,   198,   233,   247,   242,   463,   248,   251,    20,    21,
     465,    16,    17,    18,    15,   253,   254,   260,   262,    19,
     278,   284,   301,   306,   308,   203,   291,   307,    20,    21,
     292,   312,    15,   313,   459,    19,   314,   324,   487,   333,
     337,    16,    17,    18,    20,    21,   495,   384,   496,   385,
     404,   405,   406,   106,   407,   463,   408,   409,   410,    16,
      17,    18,   411,   482,   414,    19,   416,   415,   422,   423,
     427,   429,   426,   204,    20,    21,   522,   433,   524,   434,
     525,   430,   526,    19,   527,   499,   528,   438,   529,   204,
     530,   101,    20,    21,   440,   205,   450,   452,   102,    73,
      74,   488,   457,   464,   466,    75,    15,   467,   479,   498,
      14,    52,    43,    42,    33,    76,    77,    71,   505,    72,
     103,   104,    78,    79,   -44,    80,   188,   299,   486,   399,
     501,   451,   315,    16,    17,    18,    81,    82,   503,   485,
      83,   -44,   -44,   -44,   -44,   -44,   -44,   234,    84,   439,
     189,   447,   -44,   -44,   -44,   322,   421,    19,   -44,   -44,
     -44,   397,   287,    85,   290,   -44,    20,    21,    86,   -44,
       0,     0,     0,     0,     0,    73,    74,     0,     0,    87,
       0,    75,     0,     0,   149,   150,   151,   152,     0,     0,
       0,    76,    77,   153,   154,     0,    88,     0,    78,    79,
      89,    80,     0,     0,    90,     0,     0,     0,   236,   279,
       0,   106,    81,    82,     0,     0,    83,     0,     0,    73,
      74,     0,     0,     0,    84,    75,   147,     0,   149,   150,
     151,   152,     0,     0,     0,    76,    77,   153,   154,    85,
       0,     0,    78,    79,    86,    80,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    87,    81,    82,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,    88,     0,     0,     0,    89,     0,     0,     0,
      90,     0,     0,    85,    91,     0,     0,   460,    86,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    87,
       0,     0,     0,  -242,  -242,   151,   152,     0,     0,     0,
       0,     0,   153,   154,     0,     0,    88,     0,     0,     0,
      89,     0,     0,     0,    90,    73,    74,     0,   236,   279,
       0,    75,    15,     0,     0,     0,     0,     0,     0,     0,
       0,    76,    77,     0,   149,   150,   151,   152,    78,    79,
       0,    80,     0,   153,   154,     0,     0,     0,     0,    16,
      17,    18,    81,    82,     0,     0,    83,   159,   160,   161,
     162,   163,   164,   165,    84,     0,     0,     0,     0,     0,
      73,    74,     0,    19,     0,     0,    75,     0,     0,    85,
       0,     0,    20,    21,    86,     0,    76,    77,     0,     0,
       0,     0,     0,    78,    79,    87,    80,   158,   159,   160,
     161,   162,   163,   164,   165,     0,     0,    81,    82,     0,
       0,    83,    88,     0,     0,     0,    89,     0,     0,    84,
      90,    73,    74,     0,    91,     0,     0,    75,     0,     0,
       0,     0,     0,     0,    85,     0,     0,    76,    77,    86,
       0,     0,     0,     0,    78,    79,     0,    80,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    81,    82,
    -240,     0,    83,     0,     0,     0,     0,    88,     0,     0,
      84,    89,     0,     0,  -240,    90,    73,    74,  -240,    91,
       0,     0,    75,     0,     0,    85,     0,     0,     0,     0,
      86,     0,    76,    77,   149,   150,   151,   152,     0,    78,
      79,    87,    80,   153,   154,     0,     0,     0,     0,     0,
       0,     0,     0,    81,    82,     0,     0,    83,    88,     0,
       0,     0,    89,     0,     0,    84,    90,   178,     0,     0,
      91,    73,    74,     0,     0,     0,     0,    75,     0,     0,
      85,     0,     0,     0,     0,    86,     0,    76,    77,     0,
       0,     0,     0,     0,    78,    79,    87,    80,   159,   160,
     161,   162,   163,   164,   165,     0,  -240,     0,    81,    82,
       0,     0,    83,    88,     0,     0,     0,    89,     0,     0,
      84,    90,    73,    74,     0,    91,     0,     0,    75,     0,
       0,     0,     0,     0,     0,    85,     0,     0,    76,    77,
      86,     0,     0,   235,     0,    78,    79,     0,    80,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    81,
      82,   400,     0,    83,     0,     0,     0,     0,    88,     0,
       0,    84,    89,     0,   452,     0,    90,     0,     0,     0,
     236,     0,     0,     0,     0,     0,    85,     0,     0,     0,
       0,    86,   147,   148,   149,   150,   151,   152,     0,     0,
       0,     0,    87,   153,   154,   147,   148,   149,   150,   151,
     152,     0,     0,     0,     0,     0,   153,   154,     0,    88,
       0,     0,     0,    89,     0,     0,     0,    90,     0,     0,
       0,    91,   147,   148,   149,   150,   151,   152,     0,     0,
       0,     0,     0,   153,   154,     0,     0,     0,     0,     0,
       0,     0,     0,   155,     0,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,   155,     0,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   147,   148,
     149,   150,   151,   152,     0,     0,     0,     0,     0,   153,
     154,     0,     0,   155,     0,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   147,   148,   149,   150,   151,
     152,     0,     0,     0,     0,   419,   153,   154,     0,     0,
       0,   147,   148,   149,   150,   151,   152,     0,     0,     0,
       0,     0,   153,   154,     0,     0,     0,     0,     0,   155,
       0,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,     0,     0,     0,   199,     0,   200,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   155,     0,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,     0,
       0,   272,   155,   273,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,     0,     0,   379,     0,   380,
     147,   148,   149,   150,   151,   152,     0,     0,     0,     0,
       0,   153,   154,     0,     0,     0,   147,   148,   149,   150,
     151,   152,     0,     0,     0,     0,     0,   153,   154,     0,
       0,     0,   147,   148,   149,   150,   151,   152,     0,     0,
       0,     0,     0,   153,   154,     0,     0,     0,   147,   148,
     149,   150,   151,   152,     0,     0,     0,     0,     0,   153,
     154,   155,     0,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,     0,     0,     0,   474,   155,   475,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,     0,
       0,     0,   476,   155,   477,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,     0,     0,   518,   155,
     519,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,     0,     0,     0,     0,     0,   239,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,   153,   154,
       0,     0,     0,   147,   148,   149,   150,   151,   152,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,   147,
     148,   149,   150,   151,   152,     0,     0,     0,     0,     0,
     153,   154,     0,     0,     0,   147,   148,   149,   150,   151,
     152,     0,     0,     0,     0,     0,   153,   154,   155,     0,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
       0,     0,     0,     0,   155,   240,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,     0,     0,
     155,   243,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,     0,     0,     0,     0,   155,   249,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,     0,
       0,     0,     0,   250,   147,   148,   149,   150,   151,   152,
       0,     0,     0,     0,     0,   153,   154,     0,     0,     0,
     147,   148,   149,   150,   151,   152,     0,     0,     0,     0,
       0,   153,   154,     0,     0,     0,   147,   148,   149,   150,
     151,   152,     0,     0,     0,     0,     0,   153,   154,     0,
       0,     0,   147,   148,   149,   150,   151,   152,     0,     0,
       0,     0,     0,   153,   154,   155,     0,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,     0,     0,     0,
       0,   155,   300,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,     0,     0,     0,     0,   155,   424,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,     0,
       0,     0,     0,   155,   431,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,     0,     0,     0,     0,
     455,   147,   148,   149,   150,   151,   152,     0,     0,     0,
       0,     0,   153,   154,     0,     0,     0,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,   153,   154,
       0,     0,     0,   147,   148,   149,   150,   151,   152,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,   147,
     148,   149,   150,   151,   152,     0,     0,     0,     0,     0,
     153,   154,   155,     0,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,     0,     0,     0,   155,   480,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
       0,     0,     0,     0,   155,   520,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,     0,     0,
     155,   531,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,     0,     0,     0,     0,     0,   532,   147,   148,
     149,   150,   151,   152,     0,     0,     0,     0,     0,   153,
     154,     0,     0,     0,   147,   148,   149,   150,   151,   152,
       0,     0,     0,     0,     0,   153,   154,     0,     0,     0,
     147,   148,   149,   150,   151,   152,     0,     0,     0,     0,
       0,   153,   154,     0,     0,     0,   147,   148,   149,   150,
     151,   152,     0,     0,     0,     0,     0,   153,   154,   155,
       0,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,     0,     0,     0,     0,   155,   533,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,     0,     0,     0,
       0,   155,   534,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,     0,     0,     0,     0,   155,   535,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,     0,
       0,     0,     0,     0,   536,   147,   148,   149,   150,   151,
     152,     0,     0,     0,     0,     0,   153,   154,     0,     0,
       0,   147,   148,   149,   150,   151,   152,     0,     0,     0,
       0,     0,   153,   154,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   344,   345,   346,
       0,     0,   347,   348,   349,   350,   351,   352,   353,   354,
       0,     0,     0,     0,     0,     0,   155,     0,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,     0,
       0,     0,   155,   537,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,     0,   355,     0,     0,   538,
     356,   357,     0,     0,     0,     0,   358,   359,     0,   360,
       0,     0,     0,     0,     0,   361,     0,   362,   363,   364,
     365,   366,   367,   147,   148,   149,   150,   151,   152,     0,
       0,     0,     0,     0,   153,   154,     0,   147,   148,   149,
     150,   151,   152,     0,     0,     0,     0,     0,   153,   154,
       0,   147,   148,   149,   150,   151,   152,     0,     0,     0,
       0,     0,   153,   154,     0,   147,   148,   149,   150,   151,
     152,     0,     0,     0,     0,     0,   153,   154,     0,     0,
       0,     0,     0,     0,   155,     0,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,   155,   241,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
       0,     0,   155,   275,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,     0,   155,   276,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,     0,
       0,   328,   147,   148,   149,   150,   151,   152,     0,     0,
       0,     0,     0,   153,   154,     0,   147,   148,   149,   150,
     151,   152,     0,     0,     0,     0,     0,   153,   154,     0,
     147,   148,   149,   150,   151,   152,     0,     0,     0,     0,
       0,   153,   154,     0,   147,   148,   149,   150,   151,   152,
       0,     0,     0,     0,     0,   153,   154,     0,     0,     0,
       0,     0,     0,   155,     0,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,     0,   155,   332,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,     0,
       0,   155,   468,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,     0,     0,   155,   469,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,     0,     0,     0,
     470,   147,   148,   149,   150,   151,   152,     0,     0,     0,
       0,     0,   153,   154,     0,   147,   148,   149,   150,   151,
     152,     0,     0,     0,     0,     0,   153,   154,     0,   147,
     148,   149,   150,   151,   152,     0,     0,     0,     0,     0,
     153,   154,     0,   147,   148,   149,   150,   151,   152,     0,
       0,     0,     0,     0,   153,   154,     0,     0,     0,     0,
       0,     0,   155,     0,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,     0,   155,   471,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,     0,
     155,   472,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,     0,     0,   155,   473,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,     0,   504,
     147,   148,   149,   150,   151,   152,     0,     0,     0,     0,
       0,   153,   154,   147,   148,   149,   150,   151,   152,     0,
       0,     0,     0,     0,   153,   154,   147,   148,   149,   150,
     151,   152,     0,     0,     0,     0,     0,   153,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   348,   349,   350,   351,
     352,   155,  -241,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,     0,   155,   274,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,   155,     0,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   355,
       0,     0,     0,   356,   357,   149,   150,   151,   152,   358,
       0,     0,   360,     0,   153,   154,     0,     0,   129,     0,
     362,   363,   364,   365,   366,   367,  -242,  -242,     0,     0,
       0,     0,     0,   153,   154,   130,   131,   132,   133,   134,
     135,     0,     0,     0,     0,     0,   136,   137,   138,     0,
       0,     0,   139,   140,   141,     0,     0,     0,     0,   142,
       0,     0,     0,   143,     0,     0,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -242,  -242,
     161,   162,   163,   164,   165,   130,   131,   132,   133,   134,
     135,     0,     0,     0,     0,     0,   136,   137,   138,     0,
       0,     0,   139,   140,   141,     0,     0,     0,     0,   142,
       0,     0,     0,   143
};

static const yytype_int16 yycheck[] =
{
      88,    89,    90,   128,    59,    34,    91,   172,   254,     3,
       3,     3,     3,     0,    32,    32,    36,    33,    33,    99,
     450,    10,    88,    99,   112,   113,   114,   457,   116,    58,
     118,   119,   120,    63,    36,   265,   266,   113,    90,   113,
     120,    93,    94,    95,    96,    34,   113,    36,   122,    91,
      92,   113,   119,   115,   113,    97,   144,   145,   117,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    32,     1,
     168,   169,   312,   313,    99,   113,     8,   117,    90,   113,
     106,   119,   122,   113,    16,   119,   113,     0,   116,   113,
     129,    90,    89,   117,    29,     1,    89,    32,    30,    31,
     198,   199,     8,   469,   470,   471,   472,   473,   112,   112,
      16,    43,    44,    45,   118,   118,   118,   118,   105,   106,
     107,   108,   109,   113,    30,    31,   114,   117,   116,   264,
     113,   113,   172,   115,   117,    67,   392,    43,    44,    45,
     113,   236,   115,   241,    76,    77,   113,   245,   115,   247,
     248,   326,   113,   251,   115,   114,   291,   107,   108,   109,
      32,    67,     1,   114,   113,   113,   115,   115,    29,     8,
      76,    77,   116,   113,   272,   115,   274,    16,   243,   112,
     113,     1,     8,   113,   113,   115,   115,   119,     8,   121,
      16,    30,    31,   233,   114,   113,    16,   115,   113,   113,
     115,   115,     4,     5,    43,    44,    45,   112,   306,   307,
      30,    31,   113,   119,   115,   121,   314,    43,    44,    45,
      43,    44,    45,    43,    44,    45,   265,   266,    67,   113,
     328,   115,   271,   113,   332,   115,   112,    76,    77,    32,
     113,    67,   115,   113,    67,   115,   113,    67,   115,   112,
      76,    77,   114,    76,    77,   114,    76,    77,   113,   113,
     115,   115,   113,     1,   115,   113,   113,   115,   115,   113,
       8,   115,   114,   312,   313,   113,   112,   115,    16,   114,
     119,   379,   121,   112,   113,    27,    28,    59,    60,   114,
     114,    32,    30,    31,     3,   121,   114,   337,    16,   119,
     114,   121,   114,   114,   402,    43,    44,    45,   406,   407,
     408,   409,   410,   411,    16,   114,   414,   415,   114,   417,
     114,   419,    32,   388,    89,    43,    44,    45,   426,    67,
      32,   115,   114,    99,   115,   433,    99,   113,    76,    77,
     438,    43,    44,    45,    16,   114,   114,   114,    32,    67,
     114,   112,     3,    36,   114,    57,   118,   116,    76,    77,
     118,     4,    16,     5,   429,    67,   116,   115,   466,    33,
     114,    43,    44,    45,    76,    77,   474,   112,   476,   118,
     114,   114,   114,   121,   114,   483,   114,   114,   114,    43,
      44,    45,   114,   458,   114,    67,     3,   114,   112,   112,
       3,    99,   114,   121,    76,    77,   504,   114,   506,     7,
     508,   119,   510,    67,   512,   480,   514,    36,   516,   121,
     518,     1,    76,    77,    32,   464,   114,     6,     8,     9,
      10,   118,   114,   114,   113,    15,    16,   115,   114,   479,
       8,    40,    34,   115,    15,    25,    26,    60,   488,    61,
      30,    31,    32,    33,    34,    35,   121,   255,   465,   338,
     483,   417,   271,    43,    44,    45,    46,    47,   486,   464,
      50,    51,    52,    53,    54,    55,    56,   171,    58,   404,
     121,   412,    62,    63,    64,   276,   381,    67,    68,    69,
      70,   334,   245,    73,   248,    75,    76,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,     9,    10,    -1,    -1,    89,
      -1,    15,    -1,    -1,    39,    40,    41,    42,    -1,    -1,
      -1,    25,    26,    48,    49,    -1,   106,    -1,    32,    33,
     110,    35,    -1,    -1,   114,    -1,    -1,    -1,   118,   119,
      -1,   121,    46,    47,    -1,    -1,    50,    -1,    -1,     9,
      10,    -1,    -1,    -1,    58,    15,    37,    -1,    39,    40,
      41,    42,    -1,    -1,    -1,    25,    26,    48,    49,    73,
      -1,    -1,    32,    33,    78,    35,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    89,    46,    47,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
     114,    -1,    -1,    73,   118,    -1,    -1,   121,    78,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    89,
      -1,    -1,    -1,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   114,     9,    10,    -1,   118,   119,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    39,    40,    41,    42,    32,    33,
      -1,    35,    -1,    48,    49,    -1,    -1,    -1,    -1,    43,
      44,    45,    46,    47,    -1,    -1,    50,   103,   104,   105,
     106,   107,   108,   109,    58,    -1,    -1,    -1,    -1,    -1,
       9,    10,    -1,    67,    -1,    -1,    15,    -1,    -1,    73,
      -1,    -1,    76,    77,    78,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    32,    33,    89,    35,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    -1,    46,    47,    -1,
      -1,    50,   106,    -1,    -1,    -1,   110,    -1,    -1,    58,
     114,     9,    10,    -1,   118,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    25,    26,    78,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      99,    -1,    50,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      58,   110,    -1,    -1,   113,   114,     9,    10,   117,   118,
      -1,    -1,    15,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      78,    -1,    25,    26,    39,    40,    41,    42,    -1,    32,
      33,    89,    35,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,    -1,    -1,    50,   106,    -1,
      -1,    -1,   110,    -1,    -1,    58,   114,   115,    -1,    -1,
     118,     9,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    78,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    32,    33,    89,    35,   103,   104,
     105,   106,   107,   108,   109,    -1,    99,    -1,    46,    47,
      -1,    -1,    50,   106,    -1,    -1,    -1,   110,    -1,    -1,
      58,   114,     9,    10,    -1,   118,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    25,    26,
      78,    -1,    -1,    81,    -1,    32,    33,    -1,    35,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,     6,    -1,    50,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    58,   110,    -1,     6,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    78,    37,    38,    39,    40,    41,    42,    -1,    -1,
      -1,    -1,    89,    48,    49,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   118,    37,    38,    39,    40,    41,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    98,    -1,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    37,    38,
      39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,   120,    48,    49,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,    -1,    -1,    -1,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    -1,
      -1,   113,    98,   115,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    -1,    -1,    -1,   113,    -1,   115,
      37,    38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    98,    -1,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,    -1,    -1,    -1,   113,    98,   115,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    -1,
      -1,    -1,   113,    98,   115,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    -1,    -1,   113,    98,
     115,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,    -1,    -1,    -1,    -1,    -1,   115,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      -1,    -1,    -1,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    98,    -1,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      -1,    -1,    -1,    -1,    98,   115,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
      98,   115,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,    98,   115,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,    -1,   115,    37,    38,    39,    40,    41,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    98,    -1,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,    98,   115,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,    98,   115,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,    98,   115,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    -1,    -1,    -1,    -1,
     115,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    -1,    -1,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      -1,    -1,    -1,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    98,    -1,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,    98,   115,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      -1,    -1,    -1,    -1,    98,   115,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
      98,   115,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,    -1,    -1,    -1,    -1,    -1,   115,    37,    38,
      39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    -1,    -1,    37,    38,    39,    40,    41,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    98,
      -1,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,    -1,    -1,    -1,    -1,    98,   115,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    -1,    -1,    -1,
      -1,    98,   115,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,    -1,    -1,    -1,    -1,    98,   115,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    -1,
      -1,    -1,    -1,    -1,   115,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    -1,
      -1,    -1,    98,   115,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    -1,    -1,    61,    -1,    -1,   115,
      65,    66,    -1,    -1,    -1,    -1,    71,    72,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    82,    83,    84,
      85,    86,    87,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    -1,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      -1,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    -1,    98,   113,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      -1,    -1,    98,   113,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    -1,    -1,    98,   113,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    -1,
      -1,   113,    37,    38,    39,    40,    41,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    37,    38,    39,    40,
      41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      37,    38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    37,    38,    39,    40,    41,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    -1,    98,   113,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    -1,
      -1,    98,   113,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,    -1,    -1,    98,   113,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    -1,    -1,    -1,
     113,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    -1,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    -1,    -1,    98,   113,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    -1,
      98,   113,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,    -1,    -1,    98,   113,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    -1,    -1,   113,
      37,    38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    37,    38,    39,    40,
      41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    98,    -1,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    61,
      -1,    -1,    -1,    65,    66,    39,    40,    41,    42,    71,
      -1,    -1,    74,    -1,    48,    49,    -1,    -1,    34,    -1,
      82,    83,    84,    85,    86,    87,    41,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    51,    52,    53,    54,    55,
      56,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      -1,    -1,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79,    -1,    -1,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,   108,   109,    51,    52,    53,    54,    55,
      56,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      -1,    -1,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    79
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    88,   124,   125,    32,   126,     0,    89,   127,   128,
     129,   112,   113,   114,   128,    16,    43,    44,    45,    67,
      76,    77,   132,   133,   134,   137,   138,    32,    91,    92,
      97,   130,   131,   137,   114,    32,   113,   116,   139,    29,
     113,   115,   115,   133,   135,   136,   138,   134,    33,    99,
     140,   141,   131,     3,   112,   118,   193,   115,   113,   115,
      32,   113,   117,   114,   174,   112,   112,   136,   138,   112,
     193,   139,   141,     9,    10,    15,    25,    26,    32,    33,
      35,    46,    47,    50,    58,    73,    78,    89,   106,   110,
     114,   118,   142,   143,   171,   178,   179,   180,   186,   189,
     190,     1,     8,    30,    31,   119,   121,   136,   162,   163,
     175,   112,   114,   114,   114,   114,   114,   114,   116,   114,
     114,   114,   114,   114,   179,   179,   134,   179,   175,    34,
      51,    52,    53,    54,    55,    56,    62,    63,    64,    68,
      69,    70,    75,    79,   114,   113,   115,    37,    38,    39,
      40,    41,    42,    48,    49,    98,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   112,    32,   114,   114,
       3,   113,   122,   179,   179,   179,    32,   179,   115,   178,
     179,   194,   195,   196,   197,   198,   179,   179,   143,   186,
     178,    90,    93,    94,    95,    96,   144,   145,   115,   113,
     115,   174,    32,    57,   121,   136,   146,   150,   164,   165,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   114,   163,    81,   118,   178,   187,   115,
     115,   113,   115,   115,   115,   113,   117,    99,    99,   115,
     115,   113,   115,   114,   114,   113,   115,   179,   179,   119,
     114,   151,    32,     3,   118,     4,     5,   147,   148,   149,
      36,   113,   113,   115,    99,   113,   113,   178,   114,   119,
     178,   187,   188,   112,   112,   179,   193,   195,   179,   179,
     198,   118,   118,   179,    33,   106,   183,   184,   183,   145,
     115,     3,    99,   120,   152,   153,    36,   116,   114,   174,
     150,   150,     4,     5,   116,   165,   179,   179,    29,    32,
     176,   177,   176,   115,   115,   119,   113,   119,   113,   191,
     174,   181,   113,    33,   113,   115,   115,   114,   113,   115,
     179,   166,   179,    11,    12,    13,    14,    17,    18,    19,
      20,    21,    22,    23,    24,    61,    65,    66,    71,    72,
      74,    80,    82,    83,    84,    85,    86,    87,   167,   168,
     169,   172,   173,   119,   150,   150,   160,   161,   179,   113,
     115,   113,   115,   115,   112,   118,   187,   179,    27,    28,
     192,   119,    59,    60,   182,   185,   179,   184,   178,   153,
       6,   154,   113,   117,   114,   114,   114,   114,   114,   114,
     114,   114,   113,   115,   114,   114,     3,   113,   117,   120,
     179,   177,   112,   112,   115,   193,   114,     3,   183,    99,
     119,   115,   115,   114,     7,   158,   159,   179,    36,   169,
      32,   179,   179,   179,   179,   179,   179,   168,   179,   179,
     114,   161,     6,   157,   179,   115,   179,   114,    99,   193,
     121,   155,   156,   179,   114,   179,   113,   115,   113,   113,
     113,   113,   113,   113,   113,   115,   113,   115,   167,   114,
     115,   167,   193,   113,   115,   164,   157,   179,   118,   170,
     170,   170,   170,   170,   170,   179,   179,   115,   178,   193,
     115,   156,   115,   158,   113,   178,   113,   115,   113,   115,
     113,   115,   113,   115,   113,   115,   113,   115,   113,   115,
     115,   115,   179,   119,   179,   179,   179,   179,   179,   179,
     179,   115,   115,   115,   115,   115,   115,   115,   115
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 94 "gram.y"
    {
			prog_tree = revlist ((yyvsp[(2) - (2)]));
			;}
    break;

  case 5:
#line 102 "gram.y"
    {
			add_to_export_list ((yyvsp[(1) - (1)])->Symval);
			;}
    break;

  case 6:
#line 106 "gram.y"
    {
			add_to_export_list ((yyvsp[(3) - (3)])->Symval);
			;}
    break;

  case 7:
#line 111 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 8:
#line 113 "gram.y"
    {
			(yyvsp[(2) - (2)])->link = (yyvsp[(1) - (2)]);
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 9:
#line 119 "gram.y"
    {
			NODEPNT typs = (yyvsp[(2) - (7)])->child[3];
			(yyvsp[(2) - (7)])->child[3] = NULL;
			(yyval) = make_node (FUNC, typs, (yyvsp[(2) - (7)]), revlist ((yyvsp[(4) - (7)])),
				(yyvsp[(6) - (7)]), NULL, NULL, (yyvsp[(2) - (7)])->Lineno, (yyvsp[(2) - (7)])->Srcfile, (yyvsp[(2) - (7)])->Srcfunc);
			(yyval)->pragma = (yyvsp[(1) - (7)]);
			funcname = NULL;
			;}
    break;

  case 10:
#line 128 "gram.y"
    {
			NODEPNT typs = (yyvsp[(2) - (6)])->child[3];
			(yyvsp[(2) - (6)])->child[3] = NULL;
			(yyval) = make_node (FUNC, typs, (yyvsp[(2) - (6)]), NULL, (yyvsp[(5) - (6)]), NULL,
				NULL, (yyvsp[(2) - (6)])->Lineno, (yyvsp[(2) - (6)])->Srcfile, (yyvsp[(2) - (6)])->Srcfunc);
			(yyval)->pragma = (yyvsp[(1) - (6)]);
			funcname = NULL;
			;}
    break;

  case 11:
#line 137 "gram.y"
    {
			NODEPNT typs = (yyvsp[(2) - (6)])->child[3];
			(yyvsp[(2) - (6)])->child[3] = NULL;
			(yyval) = make_node (FUNCDECL, typs, (yyvsp[(2) - (6)]), revlist ((yyvsp[(4) - (6)])),
				NULL, NULL, NULL, (yyvsp[(2) - (6)])->Lineno, (yyvsp[(2) - (6)])->Srcfile, (yyvsp[(2) - (6)])->Srcfunc);
			(yyval)->pragma = (yyvsp[(1) - (6)]);
			funcname = NULL;
			;}
    break;

  case 12:
#line 146 "gram.y"
    {
			NODEPNT nn;
			NODEPNT typs = (yyvsp[(2) - (6)])->child[3];
			(yyvsp[(2) - (6)])->child[3] = NULL;
			(yyval) = make_node (FUNCDECL, typs, (yyvsp[(2) - (6)]), revlist ((yyvsp[(4) - (6)])),
				NULL, NULL, NULL, (yyvsp[(2) - (6)])->Lineno, (yyvsp[(2) - (6)])->Srcfile, (yyvsp[(2) - (6)])->Srcfunc);
			(yyval)->pragma = (yyvsp[(1) - (6)]);
			funcname = NULL;
			for (nn=(yyval)->child[2]; nn!=NULL; nn=nn->link)
			    nn->nodetype = ATYPE;
			;}
    break;

  case 13:
#line 158 "gram.y"
    {
			NODEPNT typs = (yyvsp[(2) - (5)])->child[3];
			(yyvsp[(2) - (5)])->child[3] = NULL;
			(yyval) = make_node (FUNCDECL, typs, (yyvsp[(2) - (5)]), NULL,
				NULL, NULL, NULL, (yyvsp[(2) - (5)])->Lineno, (yyvsp[(2) - (5)])->Srcfile, (yyvsp[(2) - (5)])->Srcfunc);
			(yyval)->pragma = (yyvsp[(1) - (5)]);
			funcname = NULL;
			;}
    break;

  case 14:
#line 167 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 15:
#line 169 "gram.y"
    { (yyval) = (yyvsp[(3) - (4)]); ;}
    break;

  case 16:
#line 172 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 17:
#line 174 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 18:
#line 180 "gram.y"
    {
			(yyval) = make_node (NO_INLINE, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 19:
#line 185 "gram.y"
    {
			(yyval) = make_node (LOOKUP, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 20:
#line 190 "gram.y"
    {
			(yyval) = make_node (VHDL, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			(yyval)->child[0] = (yyvsp[(2) - (2)]);
			;}
    break;

  case 21:
#line 197 "gram.y"
    {
			funcname = find_sym ((yyvsp[(2) - (2)])->Symval);
			fill_in_func_name (funcname, (yyvsp[(1) - (2)]));
			(yyvsp[(2) - (2)])->loc.func = funcname;
			(yyvsp[(2) - (2)])->child[3] = revlist ((yyvsp[(1) - (2)]));
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 22:
#line 206 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 23:
#line 208 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 24:
#line 214 "gram.y"
    {
			NODEPNT tmp;
			if ((yyvsp[(2) - (2)]) != NULL)
			    {
			    tmp = make_node (ICONST, NULL, NULL, NULL,
					NULL, NULL, NULL, (yyvsp[(1) - (2)])->Lineno, (yyvsp[(1) - (2)])->Srcfile, (yyvsp[(1) - (2)])->Srcfunc);
			    tmp->Iconst = list_length ((yyvsp[(2) - (2)]));
			    }
			else
			    tmp = NULL;
			(yyval) = make_node (ATYPE, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]), tmp, NULL, NULL,
				NULL, (yyvsp[(1) - (2)])->Lineno, (yyvsp[(1) - (2)])->Srcfile, (yyvsp[(1) - (2)])->Srcfunc);
			;}
    break;

  case 25:
#line 229 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 26:
#line 231 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 27:
#line 237 "gram.y"
    {
			NODEPNT tmp;
			if ((yyvsp[(3) - (3)]) != NULL)
			    {
			    tmp = make_node (ICONST, NULL, NULL, NULL,
					NULL, NULL, NULL, (yyvsp[(2) - (3)])->Lineno, (yyvsp[(2) - (3)])->Srcfile, (yyvsp[(2) - (3)])->Srcfunc);
			    tmp->Iconst = list_length ((yyvsp[(3) - (3)]));
			    }
			else
			    tmp = NULL;
			(yyvsp[(2) - (3)])->child[0] = (yyvsp[(1) - (3)]);
			(yyvsp[(2) - (3)])->child[1] = (yyvsp[(3) - (3)]);
			(yyvsp[(2) - (3)])->child[2] = tmp;
			(yyval) = (yyvsp[(2) - (3)]);
			;}
    break;

  case 28:
#line 254 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 29:
#line 256 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 30:
#line 258 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 31:
#line 260 "gram.y"
    {
			(yyval) = make_node (BOOLTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->Totsize = 1;
			;}
    break;

  case 32:
#line 266 "gram.y"
    {
			(yyval) = make_node (FLOATTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->Totsize = 32;
			;}
    break;

  case 33:
#line 272 "gram.y"
    {
			(yyval) = make_node (DOUBLETYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->Totsize = 64;
			;}
    break;

  case 34:
#line 279 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 35:
#line 281 "gram.y"
    {
			switch ((yyvsp[(2) - (2)])->nodetype)
			    {
			    case INTTYPE :
			        (yyvsp[(2) - (2)])->nodetype = CXINTTYPE;
				break;
			    case FIXTYPE :
			        (yyvsp[(2) - (2)])->nodetype = CXFIXTYPE;
				break;
			    case FLOATTYPE :
			        (yyvsp[(2) - (2)])->nodetype = CXFLOATTYPE;
				(yyvsp[(2) - (2)])->Totsize = 32;
				break;
			    case DOUBLETYPE :
			        (yyvsp[(2) - (2)])->nodetype = CXDOUBLETYPE;
				(yyvsp[(2) - (2)])->Totsize = 64;
				break;
			    default :
			        handle_error (ErrParseComplexType, Fatal,
				    (yyvsp[(2) - (2)])->Lineno, (yyvsp[(2) - (2)])->Srcfile, (yyvsp[(2) - (2)])->Srcfunc, (yyvsp[(2) - (2)])->nodetype);
			    }
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 36:
#line 305 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 37:
#line 307 "gram.y"
    { (yyval) = revlist ((yyvsp[(2) - (3)])); ;}
    break;

  case 38:
#line 310 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 39:
#line 312 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 40:
#line 318 "gram.y"
    {
			(yyval) = make_node (COLON, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 41:
#line 323 "gram.y"
    {
			char *tmp = (yyvsp[(1) - (1)])->Strval;
			(yyvsp[(1) - (1)])->nodetype = ICONST;
			(yyvsp[(1) - (1)])->Iconst = int_from_string (tmp);
			free (tmp);
			(yyval) = (yyvsp[(1) - (1)]);
			;}
    break;

  case 42:
#line 332 "gram.y"
    {
			NODEPNT tmp;
			tmp = make_node (BODYRET, revlist ((yyvsp[(5) - (10)])), revlist ((yyvsp[(9) - (10)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval) = make_node (FOR, (yyvsp[(3) - (10)]), tmp, NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (10)])->Lineno, (yyvsp[(3) - (10)])->Srcfile, (yyvsp[(3) - (10)])->Srcfunc);
			(yyval)->pragma = (yyvsp[(1) - (10)]);
			;}
    break;

  case 43:
#line 341 "gram.y"
    {
			NODEPNT tmp;
			tmp = make_node (BODYRET, NULL, revlist ((yyvsp[(6) - (7)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval) = make_node (FOR, (yyvsp[(3) - (7)]), tmp, NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (7)])->Lineno, (yyvsp[(3) - (7)])->Srcfile, (yyvsp[(3) - (7)])->Srcfunc);
			(yyval)->pragma = (yyvsp[(1) - (7)]);
			;}
    break;

  case 44:
#line 350 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 45:
#line 352 "gram.y"
    { (yyval) = (yyvsp[(3) - (4)]); ;}
    break;

  case 46:
#line 355 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 47:
#line 357 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 48:
#line 363 "gram.y"
    {
			(yyval) = make_node (NO_UNROLL, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 49:
#line 368 "gram.y"
    {
			(yyval) = make_node (NO_DFG, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 50:
#line 373 "gram.y"
    {
			(yyval) = make_node (STRIPMINE, revlist ((yyvsp[(3) - (4)])), NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 51:
#line 378 "gram.y"
    {
			(yyval) = make_node (BLOCK, revlist ((yyvsp[(3) - (4)])), NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 52:
#line 383 "gram.y"
    {
			(yyval) = make_node (NO_FUSE, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 53:
#line 389 "gram.y"
    {
			if ((yyvsp[(2) - (2)]) == NULL)
			    (yyval) = (yyvsp[(1) - (2)]);
			else
			    {
			    (yyvsp[(1) - (2)])->link = (yyvsp[(2) - (2)])->child[0];
			    (yyvsp[(2) - (2)])->child[0] = (yyvsp[(1) - (2)]);
			    (yyval) = (yyvsp[(2) - (2)]);
			    }
			;}
    break;

  case 54:
#line 399 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 55:
#line 401 "gram.y"
    {
			NODEPNT tmp;
			tmp = revlist ((yyvsp[(1) - (1)]));
			(yyval) = make_node (DOT_PROD, tmp, NULL, NULL,
				NULL, NULL, NULL, tmp->Lineno, tmp->Srcfile, tmp->Srcfunc);
			;}
    break;

  case 56:
#line 408 "gram.y"
    {
			NODEPNT tmp;
			tmp = revlist ((yyvsp[(1) - (1)]));
			(yyval) = make_node (CROSS_PROD, tmp, NULL, NULL,
				NULL, NULL, NULL, tmp->Lineno, tmp->Srcfile, tmp->Srcfunc);
			;}
    break;

  case 57:
#line 416 "gram.y"
    {
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 58:
#line 420 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 59:
#line 426 "gram.y"
    {
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 60:
#line 430 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 61:
#line 436 "gram.y"
    {
			(yyvsp[(1) - (6)])->child[0] = make_node (UNKNOWNTYPE, NULL,
				NULL, NULL, NULL, NULL, NULL, (yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile, (yyvsp[(1) - (6)])->Srcfunc);
			if ((yyvsp[(2) - (6)]) != NULL)
			    {
			    NODEPNT n, *p;
			    int i, cnt;

			    /* count the COLONS */
			    for (cnt=0, n=(yyvsp[(2) - (6)]); n!=NULL; n=n->link)
			        if (n->nodetype == COLON)
				    cnt++;

			    /* build a string of COLONs */
			    for (p=&((yyvsp[(1) - (6)])->child[1]), i=0; i<cnt; i++)
			        {
				n = make_node (COLON, NULL, NULL, NULL,
					NULL, NULL, NULL, (yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile, (yyvsp[(1) - (6)])->Srcfunc);
				*p = n;
				p = &(n->link);
				}

			    if (cnt > 0)
			        {
			        (yyvsp[(1) - (6)])->child[2] = make_node (ICONST, NULL, NULL,
			    		NULL, NULL, NULL, NULL, (yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile, (yyvsp[(1) - (6)])->Srcfunc);
			        (yyvsp[(1) - (6)])->child[2]->Iconst = cnt;
			        (yyval) = make_node (SLICE_GEN, (yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)]),
					    NULL, (yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile, (yyvsp[(1) - (6)])->Srcfunc);
				}
			    else
			        (yyval) = make_node (ELE_GEN, (yyvsp[(1) - (6)]), NULL, (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)]),
						NULL, (yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile,
						(yyvsp[(1) - (6)])->Srcfunc);

			    }
			else
			  (yyval) = make_node (ELE_GEN, (yyvsp[(1) - (6)]), NULL, (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)]),
					  NULL, (yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile, (yyvsp[(1) - (6)])->Srcfunc);
			;}
    break;

  case 62:
#line 477 "gram.y"
    {
			(yyval) = make_node (SCALAR_GEN, revlist ((yyvsp[(1) - (6)])), revlist ((yyvsp[(4) - (6)])),
				(yyvsp[(6) - (6)]), NULL, NULL, NULL, (yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile, (yyvsp[(1) - (6)])->Srcfunc);
			;}
    break;

  case 63:
#line 482 "gram.y"
    {
			NODEPNT n;
			(yyvsp[(2) - (9)])->child[0] = make_node (UNKNOWNTYPE, NULL,
				NULL, NULL, NULL, NULL, NULL, (yyvsp[(2) - (9)])->Lineno, (yyvsp[(2) - (9)])->Srcfile, (yyvsp[(2) - (9)])->Srcfunc);
			(yyvsp[(2) - (9)])->child[1] = revlist ((yyvsp[(4) - (9)]));

			for (n=(yyvsp[(2) - (9)])->child[1]; n!=NULL; n=n->link)
			    if (n->nodetype == INTNUM)
			        {
				char *tmp = n->Strval;
				n->nodetype = ICONST;
				n->Iconst = int_from_string (tmp);
				free (tmp);
				}

			(yyvsp[(2) - (9)])->child[2] = make_node (ICONST, NULL,
				NULL, NULL, NULL, NULL, NULL, (yyvsp[(2) - (9)])->Lineno, (yyvsp[(2) - (9)])->Srcfile, (yyvsp[(2) - (9)])->Srcfunc);
			(yyvsp[(2) - (9)])->child[2]->Iconst = list_length ((yyvsp[(2) - (9)])->child[1]);
			(yyval) = make_node (WINDOW, (yyvsp[(2) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)]),
				NULL, NULL, (yyvsp[(2) - (9)])->Lineno, (yyvsp[(2) - (9)])->Srcfile, (yyvsp[(2) - (9)])->Srcfunc);
			;}
    break;

  case 64:
#line 504 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 65:
#line 506 "gram.y"
    { (yyval) = revlist ((yyvsp[(2) - (3)])); ;}
    break;

  case 66:
#line 509 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 67:
#line 511 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 68:
#line 517 "gram.y"
    {
			(yyval) = make_node (COLON, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 69:
#line 522 "gram.y"
    {
			(yyval) = make_node (TILDE, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 70:
#line 527 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 71:
#line 529 "gram.y"
    { (yyval) = revlist ((yyvsp[(3) - (4)])); ;}
    break;

  case 72:
#line 532 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 73:
#line 534 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 74:
#line 540 "gram.y"
    {
			(yyval) = make_node (EMPTY, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 75:
#line 545 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 76:
#line 547 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 77:
#line 549 "gram.y"
    { (yyval) = revlist ((yyvsp[(3) - (4)])); ;}
    break;

  case 78:
#line 551 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 79:
#line 553 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 80:
#line 556 "gram.y"
    { (yyval) = revlist ((yyvsp[(3) - (4)])); ;}
    break;

  case 81:
#line 559 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 82:
#line 561 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 83:
#line 567 "gram.y"
    {
			(yyval) = make_node (TILDE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 84:
#line 572 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 85:
#line 575 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 86:
#line 577 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 87:
#line 583 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 88:
#line 585 "gram.y"
    {
			(yyvsp[(2) - (2)])->child[0] = make_node (NEXT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 89:
#line 591 "gram.y"
    {
			(yyval) = make_node (EMPTY, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 90:
#line 597 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 91:
#line 599 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 92:
#line 605 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 93:
#line 607 "gram.y"
    {
			(yyval) = make_node (EMPTY, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 94:
#line 613 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 95:
#line 615 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 96:
#line 621 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 97:
#line 623 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 98:
#line 629 "gram.y"
    { (yyval) = (yyvsp[(3) - (4)]); ;}
    break;

  case 99:
#line 631 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 100:
#line 633 "gram.y"
    {
			(yyvsp[(1) - (4)])->child[0] = (yyvsp[(3) - (4)]);
			(yyval) = (yyvsp[(1) - (4)]);
			;}
    break;

  case 101:
#line 638 "gram.y"
    {
			if ((yyvsp[(1) - (6)])->nodetype != CONSTRUCT_CONCAT)
			    handle_error (ErrParseMaskOnStructOp, Fatal,
			    	(yyvsp[(1) - (6)])->Lineno, (yyvsp[(1) - (6)])->Srcfile, (yyvsp[(1) - (6)])->Srcfunc);
			(yyvsp[(1) - (6)])->child[0] = (yyvsp[(3) - (6)]);
			(yyvsp[(1) - (6)])->child[1] = (yyvsp[(5) - (6)]);
			(yyval) = (yyvsp[(1) - (6)]);
			;}
    break;

  case 102:
#line 647 "gram.y"
    {
			(yyval) = make_node (ACCUM, (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(7) - (8)]),
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			switch ((yyvsp[(3) - (8)])->nodetype)
			    {
			    case REDUCE_VAL_AT_MINS :
			    case REDUCE_VAL_AT_MAXS :
			    case REDUCE_VAL_AT_FIRST_MIN :
			    case REDUCE_VAL_AT_FIRST_MAX :
			    case REDUCE_VAL_AT_LAST_MIN :
			    case REDUCE_VAL_AT_LAST_MAX :
			    case REDUCE_MODE :
			        handle_error (ErrParseBadReduceInAccum, Fatal,
			            (yyvsp[(3) - (8)])->Lineno, (yyvsp[(3) - (8)])->Srcfile, (yyvsp[(3) - (8)])->Srcfunc);
			    default :
                                break;
			    }
			;}
    break;

  case 103:
#line 667 "gram.y"
    {
			if ((yyvsp[(1) - (4)])->nodetype == REDUCE_HIST)
			    handle_error (ErrParseHistNeedsRange, Fatal,
			        (yyvsp[(1) - (4)])->Lineno, (yyvsp[(1) - (4)])->Srcfile, (yyvsp[(1) - (4)])->Srcfunc);
			(yyvsp[(1) - (4)])->child[0] = (yyvsp[(3) - (4)]);
			(yyval) = (yyvsp[(1) - (4)]);
			;}
    break;

  case 104:
#line 675 "gram.y"
    {
			if ((yyvsp[(1) - (6)])->nodetype == REDUCE_HIST)
			    (yyvsp[(1) - (6)])->child[2] = (yyvsp[(5) - (6)]); /* range */
			else
			    (yyvsp[(1) - (6)])->child[1] = (yyvsp[(5) - (6)]); /* mask */
			(yyvsp[(1) - (6)])->child[0] = (yyvsp[(3) - (6)]);
			(yyval) = (yyvsp[(1) - (6)]);
			;}
    break;

  case 105:
#line 684 "gram.y"
    {
			if ((yyvsp[(1) - (8)])->nodetype != REDUCE_HIST)
			    handle_error (ErrParseRangeOnlyForHist, Fatal,
			        (yyvsp[(1) - (8)])->Lineno, (yyvsp[(1) - (8)])->Srcfile, (yyvsp[(1) - (8)])->Srcfunc);
			(yyvsp[(1) - (8)])->child[0] = (yyvsp[(3) - (8)]);
			(yyvsp[(1) - (8)])->child[1] = (yyvsp[(7) - (8)]);
			(yyvsp[(1) - (8)])->child[2] = (yyvsp[(5) - (8)]);
			(yyval) = (yyvsp[(1) - (8)]);
			;}
    break;

  case 106:
#line 694 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_MINS, (yyvsp[(3) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(5) - (8)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 107:
#line 699 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_MINS, (yyvsp[(3) - (6)]), NULL, (yyvsp[(5) - (6)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 108:
#line 704 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_MAXS, (yyvsp[(3) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(5) - (8)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 109:
#line 709 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_MAXS, (yyvsp[(3) - (6)]), NULL, (yyvsp[(5) - (6)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 110:
#line 714 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_FIRST_MIN, (yyvsp[(3) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(5) - (8)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 111:
#line 719 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_FIRST_MIN, (yyvsp[(3) - (6)]), NULL, (yyvsp[(5) - (6)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 112:
#line 724 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_FIRST_MAX, (yyvsp[(3) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(5) - (8)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 113:
#line 729 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_FIRST_MAX, (yyvsp[(3) - (6)]), NULL, (yyvsp[(5) - (6)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 114:
#line 734 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_LAST_MIN, (yyvsp[(3) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(5) - (8)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 115:
#line 739 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_LAST_MIN, (yyvsp[(3) - (6)]), NULL, (yyvsp[(5) - (6)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 116:
#line 744 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_LAST_MAX, (yyvsp[(3) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(5) - (8)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 117:
#line 749 "gram.y"
    {
			(yyval) = make_node (REDUCE_VAL_AT_LAST_MAX, (yyvsp[(3) - (6)]), NULL, (yyvsp[(5) - (6)]), NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 118:
#line 755 "gram.y"
    {
			(yyval) = revlist ((yyvsp[(2) - (3)]));
			;}
    break;

  case 119:
#line 760 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_SUM, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 120:
#line 766 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_MIN, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 121:
#line 772 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_MAX, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 122:
#line 778 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_AND, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 123:
#line 784 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_OR, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 124:
#line 790 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_MEDIAN, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 125:
#line 796 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_MAX_INDICES, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 126:
#line 802 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_MIN_INDICES, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 127:
#line 808 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_PRODUCT, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 128:
#line 814 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_MEAN, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 129:
#line 820 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_ST_DEV, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 130:
#line 826 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_MODE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 131:
#line 832 "gram.y"
    {
			(yyval) = make_node (ARR_REDUCE_HIST, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 132:
#line 838 "gram.y"
    {
			(yyval) = make_node (ARR_CONCAT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval)->pragma = (yyvsp[(1) - (2)]);
			;}
    break;

  case 133:
#line 845 "gram.y"
    {
			(yyval) = make_node (REDUCE_SUM, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 134:
#line 850 "gram.y"
    {
			(yyval) = make_node (REDUCE_MIN, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 135:
#line 855 "gram.y"
    {
			(yyval) = make_node (REDUCE_MAX, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 136:
#line 860 "gram.y"
    {
			(yyval) = make_node (REDUCE_AND, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 137:
#line 865 "gram.y"
    {
			(yyval) = make_node (REDUCE_OR, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 138:
#line 870 "gram.y"
    {
			(yyval) = make_node (REDUCE_MEDIAN, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 139:
#line 875 "gram.y"
    {
			(yyval) = make_node (REDUCE_PRODUCT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 140:
#line 880 "gram.y"
    {
			(yyval) = make_node (REDUCE_MEAN, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 141:
#line 885 "gram.y"
    {
			(yyval) = make_node (REDUCE_ST_DEV, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 142:
#line 890 "gram.y"
    {
			(yyval) = make_node (REDUCE_MODE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 143:
#line 895 "gram.y"
    {
			(yyval) = make_node (REDUCE_HIST, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 144:
#line 901 "gram.y"
    {
			(yyval) = make_node (CONSTRUCT_ARRAY, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 145:
#line 906 "gram.y"
    {
			(yyval) = make_node (CONSTRUCT_VECTOR, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 146:
#line 911 "gram.y"
    {
			(yyval) = make_node (CONSTRUCT_MATRIX, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 147:
#line 916 "gram.y"
    {
			(yyval) = make_node (CONSTRUCT_CUBE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 148:
#line 921 "gram.y"
    {
			(yyval) = make_node (CONSTRUCT_CONCAT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 149:
#line 926 "gram.y"
    {
			(yyval) = make_node (CONSTRUCT_TILE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 150:
#line 931 "gram.y"
    {
			(yyval) = NULL;
			;}
    break;

  case 151:
#line 935 "gram.y"
    {
			if ((yyvsp[(2) - (2)]) != NULL)
			    {
			    (yyvsp[(2) - (2)])->link = (yyvsp[(1) - (2)]);
			    (yyval) = (yyvsp[(2) - (2)]);
			    }
			else
			    (yyval) = (yyvsp[(1) - (2)]);
			;}
    break;

  case 152:
#line 946 "gram.y"
    {
			(yyval) = make_node (ASSIGN, revlist ((yyvsp[(1) - (4)])), revlist ((yyvsp[(3) - (4)])), NULL,
				NULL, NULL, NULL, (yyvsp[(2) - (4)])->Lineno, (yyvsp[(2) - (4)])->Srcfile, (yyvsp[(2) - (4)])->Srcfunc);
			;}
    break;

  case 153:
#line 951 "gram.y"
    {
			NODEPNT m = make_node (LOOP_INDICES, NULL,
				NULL, NULL, NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			(yyval) = make_node (ASSIGN, revlist ((yyvsp[(1) - (6)])), m, NULL,
				NULL, NULL, NULL, (yyvsp[(2) - (6)])->Lineno, (yyvsp[(2) - (6)])->Srcfile, (yyvsp[(2) - (6)])->Srcfunc);
			;}
    break;

  case 154:
#line 959 "gram.y"
    {
			NODEPNT m;
			for (m=(yyvsp[(1) - (4)])->child[1]; m!=NULL; m=m->link)
			    if (m->nodetype != ICONST)
			        {
			        handle_error (ErrParseArrayDefNeedsSize,
			    		Fatal, (yyvsp[(1) - (4)])->Lineno, (yyvsp[(1) - (4)])->Srcfile, (yyvsp[(1) - (4)])->Srcfunc);
				break;
				}
			(yyval) = make_node (ASSIGN, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)]), NULL,
				NULL, NULL, NULL, (yyvsp[(1) - (4)])->Lineno, (yyvsp[(1) - (4)])->Srcfile, (yyvsp[(1) - (4)])->Srcfunc);
			;}
    break;

  case 155:
#line 972 "gram.y"
    {
			(yyval) = make_node (PRINT, (yyvsp[(3) - (7)]), revlist ((yyvsp[(5) - (7)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 156:
#line 977 "gram.y"
    {
			(yyval) = make_node (ASSERT, (yyvsp[(3) - (7)]), revlist ((yyvsp[(5) - (7)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 157:
#line 982 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 158:
#line 985 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 159:
#line 987 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 160:
#line 993 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 161:
#line 995 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 162:
#line 998 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 163:
#line 1000 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 164:
#line 1006 "gram.y"
    {
			(yyval) = make_node (FCALL, (yyvsp[(1) - (4)]), revlist ((yyvsp[(3) - (4)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 165:
#line 1011 "gram.y"
    {
			(yyval) = make_node (INTRINCALL, (yyvsp[(1) - (4)]), revlist ((yyvsp[(3) - (4)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 166:
#line 1016 "gram.y"
    {
			(yyval) = make_node (ARR_CONPERIM, (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(7) - (8)]),
				NULL, NULL, NULL, (yyvsp[(3) - (8)])->Lineno, (yyvsp[(3) - (8)])->Srcfile, (yyvsp[(3) - (8)])->Srcfunc);
			;}
    break;

  case 167:
#line 1021 "gram.y"
    {
			(yyval) = make_node (EXTENTS, (yyvsp[(3) - (4)]), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 168:
#line 1026 "gram.y"
    {
			if ((yyvsp[(2) - (8)]) != NULL)
			    (yyvsp[(2) - (8)])->link = revlist ((yyvsp[(3) - (8)]));
			(yyval) = make_node (BODYRET, (yyvsp[(2) - (8)]), revlist ((yyvsp[(7) - (8)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 169:
#line 1033 "gram.y"
    {
			(yyval) = make_node (FCALL, (yyvsp[(1) - (3)]), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 170:
#line 1038 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 171:
#line 1040 "gram.y"
    {
			(yyval) = make_node (ARRAYREF, (yyvsp[(1) - (4)]), revlist ((yyvsp[(3) - (4)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 172:
#line 1045 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 173:
#line 1047 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 174:
#line 1049 "gram.y"
    {
			(yyval) = make_node (TRUE_ND, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 175:
#line 1054 "gram.y"
    {
			(yyval) = make_node (FALSE_ND, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 176:
#line 1059 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 177:
#line 1061 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 178:
#line 1063 "gram.y"
    {
			(yyval) = make_node (ARR_ACCUM, (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(7) - (8)]),
						NULL, NULL, NULL, (yyvsp[(3) - (8)])->Lineno, (yyvsp[(3) - (8)])->Srcfile, (yyvsp[(3) - (8)])->Srcfunc);
			switch ((yyvsp[(3) - (8)])->nodetype)
			    {
			    case ARR_REDUCE_MAX_INDICES :
			    case ARR_REDUCE_MIN_INDICES :
			    case ARR_REDUCE_MODE :
			        handle_error (ErrParseBadReduceInAccum,
			    		Fatal, (yyvsp[(3) - (8)])->Lineno, (yyvsp[(3) - (8)])->Srcfile, (yyvsp[(3) - (8)])->Srcfunc);
			    default :
                                break;
			    }
			;}
    break;

  case 179:
#line 1078 "gram.y"
    {
			(yyval) = make_node (COMPLEX, (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), NULL,
				NULL, NULL, NULL, (yyvsp[(2) - (5)])->Lineno, (yyvsp[(2) - (5)])->Srcfile, (yyvsp[(2) - (5)])->Srcfunc);
			;}
    break;

  case 180:
#line 1083 "gram.y"
    {
			(yyval) = make_node (REAL, (yyvsp[(3) - (4)]), NULL, NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (4)])->Lineno, (yyvsp[(3) - (4)])->Srcfile, (yyvsp[(3) - (4)])->Srcfunc);
			;}
    break;

  case 181:
#line 1088 "gram.y"
    {
			(yyval) = make_node (IMAG, (yyvsp[(3) - (4)]), NULL, NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (4)])->Lineno, (yyvsp[(3) - (4)])->Srcfile, (yyvsp[(3) - (4)])->Srcfunc);
			;}
    break;

  case 182:
#line 1093 "gram.y"
    {
			(yyval) = make_node (ADD, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 183:
#line 1098 "gram.y"
    {
			(yyval) = make_node (SUB, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 184:
#line 1103 "gram.y"
    {
			(yyval) = make_node (MUL, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 185:
#line 1108 "gram.y"
    {
			(yyval) = make_node (DIV, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 186:
#line 1113 "gram.y"
    {
			(yyval) = make_node (MOD, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 187:
#line 1118 "gram.y"
    {
			(yyval) = make_node (LT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 188:
#line 1123 "gram.y"
    {
			(yyval) = make_node (GT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,	
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 189:
#line 1128 "gram.y"
    {
			(yyval) = make_node (LE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 190:
#line 1133 "gram.y"
    {
			(yyval) = make_node (GE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 191:
#line 1138 "gram.y"
    {
			(yyval) = make_node (NEQ, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 192:
#line 1143 "gram.y"
    {
			(yyval) = make_node (EQ, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 193:
#line 1148 "gram.y"
    {
			(yyval) = make_node (AND, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 194:
#line 1153 "gram.y"
    {
			(yyval) = make_node (OR, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 195:
#line 1158 "gram.y"
    {
			(yyval) = make_node (BIT_AND, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 196:
#line 1163 "gram.y"
    {
			(yyval) = make_node (BIT_OR, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 197:
#line 1168 "gram.y"
    {
			(yyval) = make_node (BIT_EOR, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 198:
#line 1173 "gram.y"
    {
			(yyval) = make_node (LEFT_SHIFT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 199:
#line 1178 "gram.y"
    {
			(yyval) = make_node (RIGHT_SHIFT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 200:
#line 1183 "gram.y"
    {
			(yyval) = make_node (NOT, (yyvsp[(2) - (2)]), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 201:
#line 1188 "gram.y"
    {
			if ((((yyvsp[(2) - (2)])->nodetype==INTNUM) && (! is_hex ((yyvsp[(2) - (2)])->Strval)) &&
				(! is_binary ((yyvsp[(2) - (2)])->Strval))) || ((yyvsp[(2) - (2)])->nodetype==FLOATNUM))
			    {
			    char *str;
			    SacMalloc (str, char*, 2 + strlen ((yyvsp[(2) - (2)])->Strval))
			    strcpy (str, "-");
			    strcat (str, (yyvsp[(2) - (2)])->Strval);
			    free ((yyvsp[(2) - (2)])->Strval);
			    (yyvsp[(2) - (2)])->Strval = str;
			    (yyval) = (yyvsp[(2) - (2)]);
			    }
			else
			    (yyval) = make_node (NEG, (yyvsp[(2) - (2)]), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 202:
#line 1205 "gram.y"
    {
			(yyval) = make_node (CAST, (yyvsp[(2) - (4)]), (yyvsp[(4) - (4)]), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 203:
#line 1210 "gram.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 204:
#line 1212 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 205:
#line 1214 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 206:
#line 1216 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 207:
#line 1219 "gram.y"
    {
			if (((yyvsp[(6) - (8)])==NULL) && ((yyvsp[(7) - (8)])==NULL))
			    handle_error (ErrParseNoCaseOrDefault, 
			    		Fatal, (yyvsp[(3) - (8)])->Lineno, (yyvsp[(3) - (8)])->Srcfile, (yyvsp[(3) - (8)])->Srcfunc);

			(yyval) = make_node (SWITCH, (yyvsp[(3) - (8)]), revlist ((yyvsp[(6) - (8)])), (yyvsp[(7) - (8)]),
				NULL, NULL, NULL, (yyvsp[(3) - (8)])->Lineno, (yyvsp[(3) - (8)])->Srcfile, (yyvsp[(3) - (8)])->Srcfunc);
			;}
    break;

  case 208:
#line 1228 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 209:
#line 1230 "gram.y"
    {
			(yyvsp[(2) - (2)])->link = (yyvsp[(1) - (2)]);
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 210:
#line 1236 "gram.y"
    {
			(yyval) = make_node (CASE, revlist ((yyvsp[(2) - (4)])), (yyvsp[(4) - (4)]), NULL,
				NULL, NULL, NULL, (yyvsp[(2) - (4)])->Lineno, (yyvsp[(2) - (4)])->Srcfile, (yyvsp[(2) - (4)])->Srcfunc);
			;}
    break;

  case 211:
#line 1242 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 212:
#line 1244 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 213:
#line 1250 "gram.y"
    {
			char *tmp = (yyvsp[(1) - (1)])->Strval;
			int val = int_from_string (tmp);
			free (tmp);
			(yyvsp[(1) - (1)])->nodetype = ICONST;
			(yyvsp[(1) - (1)])->Iconst = val;
			(yyval) = (yyvsp[(1) - (1)]);
			;}
    break;

  case 214:
#line 1259 "gram.y"
    {
			char *tmp = (yyvsp[(2) - (2)])->Strval;
			int val = int_from_string (tmp);
			free (tmp);
			(yyvsp[(2) - (2)])->nodetype = ICONST;
			(yyvsp[(2) - (2)])->Iconst = -val;
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 215:
#line 1268 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 216:
#line 1270 "gram.y"
    {
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 217:
#line 1275 "gram.y"
    {
			if ((yyvsp[(1) - (4)])->nodetype == ARR_REDUCE_HIST)
			    handle_error (ErrParseHistNeedsRange,
			    		Fatal, (yyvsp[(1) - (4)])->Lineno, (yyvsp[(1) - (4)])->Srcfile, (yyvsp[(1) - (4)])->Srcfunc);
			else if ((yyvsp[(1) - (4)])->nodetype == ARR_CONCAT)
			    handle_error (ErrParseConcatNeedsTwoArrays,
			    		Fatal, (yyvsp[(1) - (4)])->Lineno, (yyvsp[(1) - (4)])->Srcfile, (yyvsp[(1) - (4)])->Srcfunc);
			(yyvsp[(1) - (4)])->child[0] = (yyvsp[(3) - (4)]);
			(yyval) = (yyvsp[(1) - (4)]);
			;}
    break;

  case 218:
#line 1286 "gram.y"
    {
			if ((yyvsp[(1) - (6)])->nodetype == ARR_REDUCE_HIST)
			    (yyvsp[(1) - (6)])->child[2] = (yyvsp[(5) - (6)]);
			else
			    (yyvsp[(1) - (6)])->child[1] = (yyvsp[(5) - (6)]);
			(yyvsp[(1) - (6)])->child[0] = (yyvsp[(3) - (6)]);
			(yyval) = (yyvsp[(1) - (6)]);
			;}
    break;

  case 219:
#line 1295 "gram.y"
    {
			if ((yyvsp[(1) - (8)])->nodetype != ARR_REDUCE_HIST)
			    handle_error (ErrParseRangeOnlyForHist,
			    		Fatal, (yyvsp[(1) - (8)])->Lineno, (yyvsp[(1) - (8)])->Srcfile, (yyvsp[(1) - (8)])->Srcfunc);
			(yyvsp[(1) - (8)])->child[0] = (yyvsp[(3) - (8)]);
			(yyvsp[(1) - (8)])->child[1] = (yyvsp[(7) - (8)]);
			(yyvsp[(1) - (8)])->child[2] = (yyvsp[(5) - (8)]);
			(yyval) = (yyvsp[(1) - (8)]);
			;}
    break;

  case 220:
#line 1306 "gram.y"
    {
			(yyval) = make_node (ARR_SLICE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 221:
#line 1311 "gram.y"
    {
			(yyval) = make_node (ARR_SLICE, revlist ((yyvsp[(2) - (3)])), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 222:
#line 1316 "gram.y"
    {
			(yyval) = make_node (ARR_SLICE, revlist ((yyvsp[(2) - (3)])), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 223:
#line 1322 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 224:
#line 1324 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 225:
#line 1330 "gram.y"
    {
			NODEPNT tmp;
			tmp = make_node (BODYRET, revlist ((yyvsp[(6) - (11)])), revlist ((yyvsp[(10) - (11)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			(yyval) = make_node (WHILE, (yyvsp[(3) - (11)]), tmp, NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (11)])->Lineno, (yyvsp[(3) - (11)])->Srcfile, (yyvsp[(3) - (11)])->Srcfunc);
			;}
    break;

  case 226:
#line 1339 "gram.y"
    {
			NODEPNT tmp, *p;
			tmp = revlist_elifs ((yyvsp[(6) - (8)]));
			for (p=&tmp; (*p)!=NULL; p=&((*p)->child[1]->child[2]));
			*p = (yyvsp[(8) - (8)]);
			(yyval) = make_node (CONDITIONAL, (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), tmp,
				NULL, NULL, NULL, (yyvsp[(3) - (8)])->Lineno, (yyvsp[(3) - (8)])->Srcfile, (yyvsp[(3) - (8)])->Srcfunc);
			;}
    break;

  case 227:
#line 1348 "gram.y"
    {
			NODEPNT t1, t2;
			t1 = make_node (BODYRET, NULL, (yyvsp[(3) - (5)]), NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (5)])->Lineno, (yyvsp[(3) - (5)])->Srcfile, (yyvsp[(3) - (5)])->Srcfunc);
			t2 = make_node (BODYRET, NULL, (yyvsp[(5) - (5)]), NULL,
				NULL, NULL, NULL, (yyvsp[(5) - (5)])->Lineno, (yyvsp[(5) - (5)])->Srcfile, (yyvsp[(5) - (5)])->Srcfunc);
			(yyval) = make_node (CONDITIONAL, (yyvsp[(1) - (5)]), t1, t2,
				NULL, NULL, NULL, (yyvsp[(1) - (5)])->Lineno, (yyvsp[(1) - (5)])->Srcfile, (yyvsp[(1) - (5)])->Srcfunc);
			;}
    break;

  case 228:
#line 1358 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 229:
#line 1360 "gram.y"
    {
			(yyvsp[(2) - (2)])->child[1]->child[2] = (yyvsp[(1) - (2)]);
			(yyval) = (yyvsp[(2) - (2)]);
			;}
    break;

  case 230:
#line 1366 "gram.y"
    {
			NODEPNT tmp;
			tmp = make_node (CONDITIONAL, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)]), NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (5)])->Lineno, (yyvsp[(3) - (5)])->Srcfile, (yyvsp[(3) - (5)])->Srcfunc);
			(yyval) = make_node (BODYRET, NULL, tmp, NULL,
				NULL, NULL, NULL, (yyvsp[(3) - (5)])->Lineno, (yyvsp[(3) - (5)])->Srcfile, (yyvsp[(3) - (5)])->Srcfunc);
			;}
    break;

  case 231:
#line 1375 "gram.y"
    {
			(yyval) = make_node (BODYRET, revlist ((yyvsp[(2) - (7)])), revlist ((yyvsp[(6) - (7)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 232:
#line 1380 "gram.y"
    {
			(yyval) = make_node (BODYRET, NULL, revlist ((yyvsp[(3) - (4)])), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 233:
#line 1386 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 234:
#line 1388 "gram.y"
    {
			(yyvsp[(3) - (3)])->link = (yyvsp[(1) - (3)]);
			(yyval) = (yyvsp[(3) - (3)]);
			;}
    break;

  case 235:
#line 1394 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 236:
#line 1396 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 237:
#line 1399 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 238:
#line 1401 "gram.y"
    {
			(yyvsp[(1) - (3)])->child[2] = (yyvsp[(3) - (3)]);
			(yyval) = (yyvsp[(1) - (3)]);
			;}
    break;

  case 239:
#line 1407 "gram.y"
    {
			(yyval) = make_node (TRIPLE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			;}
    break;

  case 240:
#line 1412 "gram.y"
    { (yyval) = NULL; ;}
    break;

  case 241:
#line 1414 "gram.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 4443 "gram.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1416 "gram.y"


