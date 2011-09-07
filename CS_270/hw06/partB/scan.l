%{
#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"
#include "gram.tab.h"

#define STRSIZE 1000

/*
#define PRINTF(x,y) printf (x, y)
*/
#define PRINTF(x,y)

int mylineno = 1;
char scan_str[STRSIZE];
char filename[1024];
char *funcname = NULL;

/* WARNING! Make sure this list of strings has a one-to-one
 * association with the 'keytokens' data structure that follows
 * it!
 */
char *keystrings[] =
    {
    "float",
    "double",
    "complex",
    "dot",
    "cross",
    "real",
    "imag",
    "step",
    "at",
    "array",
    "vector",
    "matrix",
    "cube",
    "sum",
    "min",
    "max",
    "and",
    "or",
    "median",
    "concat",
    "tile",
    "extents",
    "if",
    "else",
    "elif",
    "for",
    "while",
    "switch",
    "case",
    "default",
    "bool",
    "true",
    "false",
    "assert",
    "print",
    "array_conperim",
    "array_sum",
    "array_min",
    "array_max",
    "array_and",
    "array_or",
    "array_median",
    "window",
    "next",
    "product",
    "array_max_indices",
    "array_min_indices",
    "array_product",
    "mean",
    "mode",
    "array_mean",
    "array_mode", 
    "array_histogram",
    "array_concat",
    "histogram",
    "accum",
    "array_accum",
    "st_dev",
    "array_st_dev",
    "return",
    "final",
    "loop_indices",
    "vals_at_first_min",
    "vals_at_first_max",
    "vals_at_last_min",
    "vals_at_last_max",
    "vals_at_mins",
    "vals_at_maxs",
    "export",
    "no_inline",
    "no_unroll",
    "no_dfg",
    "lookup",
    "stripmine",
    "no_fuse",
    "block",
    "vhdl"
    };

int keytokens[] =
    {
    TOK_FLOAT,
    TOK_DOUBLE,
    TOK_COMPLEX,
    TOK_DOT,
    TOK_CROSS,
    TOK_REAL,
    TOK_IMAG,
    TOK_STEP,
    TOK_AT,
    TOK_ARRAY,
    TOK_VECTOR,
    TOK_MATRIX,
    TOK_CUBE,
    TOK_SUM,
    TOK_MIN,
    TOK_MAX,
    TOK_RED_AND,
    TOK_RED_OR,
    TOK_MEDIAN,
    TOK_CONCAT,
    TOK_TILE,
    TOK_EXTENTS,
    TOK_IF,
    TOK_ELSE,
    TOK_ELIF,
    TOK_FOR,
    TOK_WHILE,
    TOK_SWITCH,
    TOK_CASE,
    TOK_DEFAULT,
    TOK_BOOL,
    TOK_TRUE,
    TOK_FALSE,
    TOK_ASSERT,
    TOK_PRINT,
    TOK_ARR_CONPERIM,
    TOK_ARR_SUM,
    TOK_ARR_MIN,
    TOK_ARR_MAX,
    TOK_ARR_RED_AND,
    TOK_ARR_RED_OR,
    TOK_ARR_MEDIAN,
    TOK_WINDOW,
    TOK_NEXT,
    TOK_PRODUCT,
    TOK_ARR_MAX_INDICES,
    TOK_ARR_MIN_INDICES,
    TOK_ARR_PRODUCT,
    TOK_MEAN,
    TOK_MODE,
    TOK_ARR_MEAN,
    TOK_ARR_MODE,
    TOK_ARR_HIST,
    TOK_ARR_CONCAT,
    TOK_HIST,
    TOK_ACCUM,
    TOK_ARR_ACCUM,
    TOK_ST_DEV,
    TOK_ARR_ST_DEV,
    TOK_RETURN,
    TOK_FINAL,
    TOK_LOOP_INDICES,
    TOK_VAL_AT_FIRST_MIN,
    TOK_VAL_AT_FIRST_MAX,
    TOK_VAL_AT_LAST_MIN,
    TOK_VAL_AT_LAST_MAX,
    TOK_VAL_AT_MINS,
    TOK_VAL_AT_MAXS,
    TOK_EXPORT,
    TOK_NO_INLINE,
    TOK_NO_UNROLL,
    TOK_NO_DFG,
    TOK_LOOKUP,
    TOK_STRIPMINE,
    TOK_NO_FUSE,
    TOK_BLOCK,
    TOK_VHDL
    };

char *intrinsics[] = {
    "sin",
    "cos",
    "tan",
    "asin",
    "acos",
    "atan",
    "atan2",
    "sinh",
    "cosh",
    "tanh",
    "asinh",
    "acosh",
    "atanh",
    "sqrt",
    "cbrt",
    "pow",
    "modf",
    "exp",
    "frexp",
    "ldexp",
    "log",
    "log10",
    "expm1",
    "log1p",
    "ceil",
    "fabs",
    "floor",
    "fmod",
    "copysign",
    "hypot",
    "rint"
    };

Intrinsic iref[] = {
    IntrinSin,
    IntrinCos,
    IntrinTan,
    IntrinAsin,
    IntrinAcos,
    IntrinAtan,
    IntrinAtan2,
    IntrinSinh,
    IntrinCosh,
    IntrinTanh,
    IntrinAsinh,
    IntrinAcosh,
    IntrinAtanh,
    IntrinSqrt,
    IntrinCbrt,
    IntrinPow,
    IntrinModf,
    IntrinExp,
    IntrinFrexp,
    IntrinLdexp,
    IntrinLog,
    IntrinLog10,
    IntrinExpm1,
    IntrinLog1p,
    IntrinCeil,
    IntrinFabs,
    IntrinFloor,
    IntrinFmod,
    IntrinCopysign,
    IntrinHypot,
    IntrinRint
    };

%}

alpha			[a-zA-Z]
numeral			[0-9]
nums			({numeral}+)
hexnum			[0-9a-fA-F]
hexnums			({hexnum}+)
binnum			[0-1]
binnums			({binnum}+)
white			[ \t]
flnumber		{nums}(\.{numeral}*)?((e|E)"-"?{nums})?(f|F)?
name			{alpha}({alpha}|_|{numeral})*
nl			("\n")

%s PREPROC

%%
{white}+		;
\#			{
			/* assume that this is a line number from 'cpp' */
			BEGIN(PREPROC);
			}
<PREPROC>{nums}		{
			sscanf (yytext, "%d", &mylineno);
			}
<PREPROC>\"		{
			int idx = 0;
			int ch;

			/* grab the file name */
			while ((idx < STRSIZE) && (ch = input ()))
			    switch (ch)
				{
				case '\"' :
				    scan_str[idx] = '\0';
				    goto StrEnd;
				case '\n' :
				    mylineno++;
				    handle_error (ErrScanDanglingString, Deadly,
				    		mylineno, filename, funcname);
				default :
				    scan_str[idx] = ch;
				    idx++;
				    break;
				}
			StrEnd:
			strcpy (filename, scan_str);

			/* now chew up the rest of the line and reset to INITIAL state */
			while (input () != '\n');

			BEGIN(INITIAL);
			}
"in"			{
			PRINTF ("%s\n", yytext);
			return TOK_GEN;
			}
\"			{
			int idx = 0;
			int ch;

			while ((idx < STRSIZE) && (ch = input ()))
			    switch (ch)
				{
				case '\"' :
				    scan_str[idx] = '\0';
				    goto StrEnd1;
				case '\n' :
				    mylineno++;
				    handle_error (ErrScanDanglingString, Deadly,
				    		mylineno, filename, funcname);
				default :
				    scan_str[idx] = ch;
				    idx++;
				    break;
				}
			StrEnd1:
			yylval = make_node (STRING, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			SacMalloc (yylval->Strval, char*, (idx+1) * sizeof (char))
			strcpy (yylval->Strval, scan_str);
			PRINTF ("%s\n", yytext);
			return TOK_STRING; 
			}
{nl}			{
			mylineno++;
			}
"//"{white}*"PRAGMA"	{
			return TOK_PRAGMA;
			}
"//"			{
			while (input () != '\n');
			mylineno++;
			}
"/*"			{
			int state;
			int ch;

			state = 0;
			while (state < 2)
			    {
			    ch = input ();
			    if (ch == '\n')
				mylineno++;
			    switch (ch)
				{
				case '*' :
				    state = 1;
				    break;
				case '/' :
				    if (state == 1)
				      state = 2;
				    break;
				case EOF :
				    handle_error (ErrScanDanglingComment,
				    		Deadly, mylineno, filename, funcname);
				default	:
				    state = 0;
				}
			    }
			}
"bits"{nums}		{
			int sz;
			yylval = make_node (BITSTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			sscanf (yytext+4, "%d", &sz);
			yylval->Totsize = sz;
			if (sz > 32)
			    handle_error (ErrScanBitWidth,
			    		Deadly, mylineno, filename, funcname);
			if (sz < 1)
			    handle_error (ErrScanBitWidthLow,
			    		Deadly, mylineno, filename, funcname);
			PRINTF ("%s\n", yytext);
			return TOK_BITS;
			}
"uint"{nums}		{
			int sz;
			yylval = make_node (UINTTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			sscanf (yytext+4, "%d", &sz);
			yylval->Totsize = sz;
			if (sz > 32)
			    handle_error (ErrScanBitWidth,
			    		Deadly, mylineno, filename, funcname);
			if (sz < 1)
			    handle_error (ErrScanBitWidthLow,
			    		Deadly, mylineno, filename, funcname);
			PRINTF ("%s\n", yytext);
			return TOK_INT;
			}
"int"{nums}		{
			int sz;
			yylval = make_node (INTTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			sscanf (yytext+3, "%d", &sz);
			yylval->Totsize = sz;
			if (sz > 32)
			    handle_error (ErrScanBitWidth,
			    		Deadly, mylineno, filename, funcname);
			if (sz < 2)
			    handle_error (ErrScanBitWidthLow,
			    		Deadly, mylineno, filename, funcname);
			PRINTF ("%s\n", yytext);
			return TOK_INT;
			}
"ufix"{nums}\.{nums}	{
			int tsz, fsz;
			char *c;
			yylval = make_node (UFIXTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			sscanf (yytext+4, "%d", &tsz);
			yylval->Totsize = tsz;
			if (tsz > 32)
			    handle_error (ErrScanBitWidth,
			    		Deadly, mylineno, filename, funcname);
			if (tsz < 1)
			    handle_error (ErrScanBitWidthLow,
			    		Deadly, mylineno, filename, funcname);
			for (c=yytext; (*c)!='.'; c++);
			sscanf (c+1, "%d", &fsz);
			yylval->Fracsize = fsz;
			if (fsz > tsz)
			    handle_error (ErrScanBitWidthFrac,
			    		Deadly, mylineno, filename, funcname);
			PRINTF ("%s\n", yytext);
			return TOK_FIX;
			}
"fix"{nums}\.{nums}	{
			int tsz, fsz;
			char *c;
			yylval = make_node (FIXTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			sscanf (yytext+3, "%d", &tsz);
			yylval->Totsize = tsz;
			if (tsz > 32)
			    handle_error (ErrScanBitWidth,
			    		Deadly, mylineno, filename, funcname);
			if (tsz < 2)
			    handle_error (ErrScanBitWidthLow,
			    		Deadly, mylineno, filename, funcname);
			for (c=yytext; (*c)!='.'; c++);
			sscanf (c+1, "%d", &fsz);
			yylval->Fracsize = fsz;
			if (fsz >= tsz)
			    handle_error (ErrScanBitWidthFrac,
			    		Deadly, mylineno, filename, funcname);
			PRINTF ("%s\n", yytext);
			return TOK_FIX;
			}
{name}			{
			int n, i;
			SYM_ENTRY *s;

			n = sizeof (keystrings) / sizeof (char*);

			for (i=0; i<n; i++)
			    if (strcmp (keystrings[i], yytext) == 0)
				{
				PRINTF ("%s\n", yytext);
				return (keytokens[i]);
				}

			n = sizeof (intrinsics) / sizeof (char*);

			for (i=0; i<n; i++)
			    if (strcmp (intrinsics[i], yytext) == 0)
				{
				PRINTF ("%s\n", yytext);
				yylval = make_node (INTRINSIC_FUNC, NULL, NULL,
					NULL, NULL, NULL, NULL, mylineno,
					handle_filename (filename), funcname);
				yylval->Intrinval = iref[i];
				return (TOK_INTRINSIC);
				}

			s = handle_ident (yytext);
			yylval = make_node (IDENT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			yylval->Symval = s->val;
			yylval->Defval = -1;
			PRINTF ("%s\n", yytext);
			return (TOK_IDENT);
			}
"0x"{hexnums}		|
"0b"{binnums}		|
{nums}			{
			char *str;
			yylval = make_node (INTNUM, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			SacMalloc (str, char*, 1 + strlen (yytext))
			strcpy (str, yytext);
			yylval->Strval = str;
			PRINTF ("%s\n", yytext);
			return (TOK_INTNUM);
			}
{flnumber}		{
			char *str;
			yylval = make_node (FLOATNUM, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			SacMalloc (str, char*, 1 + strlen (yytext))
			strcpy (str, yytext);
			yylval->Strval = str;
			PRINTF ("%s\n", yytext);
			return (TOK_FLOATNUM);
			}
"&&"			{
			PRINTF ("%s\n", yytext);
			return (TOK_AND);
			}
"||"			{
			PRINTF ("%s\n", yytext);
			return (TOK_OR);
			}
"=="			{
			PRINTF ("%s\n", yytext);
			return (TOK_EQUAL);
			}
"!="			{
			PRINTF ("%s\n", yytext);
			return (TOK_NE);
			}
"<="			{
			PRINTF ("%s\n", yytext);
			return (TOK_LE);
			}
">="			{
			PRINTF ("%s\n", yytext);
			return (TOK_GE);
			}
"<<"			{
			PRINTF ("%s\n", yytext);
			return TOK_LEFT_SHIFT;
			}
">>"			{
			PRINTF ("%s\n", yytext);
			return TOK_RIGHT_SHIFT;
			}
"?"			|
"&"			|
"^"			|
"|"			|
"_"			|
"~"			|
"&"			|
"+"			|
"-"			|
"*"			|
"/"			|
"%"			|
"!"			|
"="			|
"<"			|
">"			|
","			|
":"			|
";"			|
"{"			|
"}"			|
"("			|
")"			|
"["			|
"]"			{
			PRINTF ("%s\n", yytext);
			return (yytext[0]);
			}
.			{
			handle_error (ErrScanInvalidChar, Fatal,
				mylineno, filename, funcname, (int)(yytext[0]));
			}
%%
int yywrap()
    {
    fclose (yyin);
    return (1);
    }
