%{
///////////////////////////////////////////////////////////////////////////////
//Class:        CS 445
//Semester:     Fall 2011
//Assignment:   Homework 2
//Author:       Colby Blair
//File name:    ecma.l
///////////////////////////////////////////////////////////////////////////////


#include "ecma.tab.h"
#include "main.h"
#include "tokenlist.h"

//macro for the tokenlist_append argument
#define _TLA(CATEGORY)  tokenlist_append((int)CATEGORY, yytext, lineno, YY_FNAME)

int lineno;
extern char *YY_FNAME;

%}

id		[a-zA-Z_]+[a-zA-Z0-9_]+ 
int_lit		[0-9]+
real_lit	[0-9]*"."[0-9]+
period		"."
comma		","
semicolon	";"
colon		":"
lc		"{"
rc		"}"
lp		"("
rp		")"
lb		"["
rb		"]"
assign		"="
asterisk	"*"
lt		"<"
gt		">"
lte		">="
gte		"<="
dlt		"<<"
dgt		">>"
sum		"+"
dif		"-"
div		"/"
mul		"*"
pow		"^"
increment	"++"
decrement	"--"
amp		"&"
damp		"&&"
pipe		"|"
dpipe		"||"
at		"@"

string_lit	"\""[^\"\n]*"\""|"'"[^\"\n]*"'" 

%%
"//".*$	      		{ }
"EXIT"        		{ return(EXIT); }
"\n"          		{ ++lineno; }
" "			{ /*white space, no op for now*/ }
"\t"			{ /*white space, no op for now*/ }

"as"			{ return( _TLA(AS) ); }
"Array"			{ return( _TLA(ARRAY) ); }
"Boolean"		{ return( _TLA(BOOLEAN) ); } 
"break"			{ return( _TLA(BREAK) ); } 
"case" 			{ return( _TLA(CASE) ); } 	
"catch"			{ return( _TLA(CATCH) ); } 
"class"		 	{ return( _TLA(CLASS) ); } 
"const"			{ return( _TLA(CONST) ); }
"continue"		{ return( _TLA(CONTINUE) ); }
"default"		{ return( _TLA(DEFAULT) ); }
"delete"		{ return( _TLA(DELETE) ); }
"do"			{ return( _TLA(DO) ); }
"else"			{ return( _TLA(ELSE) ); }
"extends"		{ return( _TLA(EXTENDS) ); }
"false"			{ return( _TLA(FALSE) ); }
"finally"		{ return( _TLA(FINALLY) ); }
"for"			{ return( _TLA(FOR) ); }
"function"		{ return( _TLA(FUNCTION) ); }
"if"			{ return( _TLA(IF) ); }
"implements"		{ return( _TLA(IMPLEMENTS) ); }
"import"		{ return( _TLA(IMPORT) ); }
"in"			{ return( _TLA(IN) ); }
"instanceof"		{ return( _TLA(INSTANCEOF) ); }
"int"			{ return( _TLA(INT) ); }
"interface"		{ return( _TLA(INTERFACE) ); }
"internal" 		{ return( _TLA(INTERNAL) ); }
"is"			{ return( _TLA(IS) ); }
"new"			{ return( _TLA(NEW) ); }
"null"|"Null"		{ return( _TLA(NULL_TYPE) ); }
"Number"		{ return( _TLA(NUMBER) ); }
"Object"		{ return( _TLA(OBJECT) ); }
"package"		{ return( _TLA(PACKAGE) ); }
"private"		{ return( _TLA(PRIVATE) ); }
"protected"		{ return( _TLA(PROTECTED) ); }
"public"		{ return( _TLA(PUBLIC) ); }
"return"		{ return( _TLA(RETURN) ); }
"String"		{ return( _TLA(STRING) ); }
"super"			{ return( _TLA(SUPER) ); }
"switch"		{ return( _TLA(SWITCH) ); }
"this"			{ return( _TLA(THIS) ); }
"throw"			{ return( _TLA(THROW) ); }
"to"			{ return( _TLA(TO) ); }
"true"			{ return( _TLA(TRUE) ); }
"try"			{ return( _TLA(TRY) ); }
"typeof"		{ return( _TLA(TYPEOF) ); }
"uint"			{ return( _TLA(UINT) ); }
"use"			{ return( _TLA(USE) ); }
"var"			{ return( _TLA(VAR) ); }
"void"			{ return( _TLA(VOID) ); }
"while"			{ return( _TLA(WHILE) ); }
"with"			{ return( _TLA(WITH) ); }

"each"			{ return( _TLA(EACH) ); }
"get"			{ return( _TLA(GET) ); }
"set"			{ return( _TLA(SET) ); }
"namespace"		{ return( _TLA(NAMESPACE) ); }
"include"		{ return( _TLA(INCLUDE) ); }
"dynamic"		{ return( _TLA(DYNAMIC) ); }
"final"			{ return( _TLA(FINAL) ); }
"native"		{ return( _TLA(NATIVE) ); }
"override"		{ return( _TLA(OVERRIDE) ); }
"static"		{ return( _TLA(STATIC) ); }

{id}			{ return( _TLA(ID) ); } 
{int_lit}		{ return( _TLA(INT_LIT) ); }
{real_lit}		{ return( _TLA(REAL_LIT) ); }

{period}		{ return( _TLA(PERIOD) ); }
{comma}			{ return( _TLA(COMMA) ); }
{semicolon}		{ return( _TLA(SEMICOLON) ); }
{colon}			{ return( _TLA(COLON) ); }
{lc}			{ return( _TLA(LC) ); }
{rc}			{ return( _TLA(RC) ); }
{lp}			{ return( _TLA(LP) ); }
{rp}			{ return( _TLA(RP) ); }
{lb}			{ return( _TLA(LB) ); }
{rb}			{ return( _TLA(RB) ); }
{assign}		{ return( _TLA(ASSIGN) ); }
{lt}			{ return( _TLA(LT) ); }
{gt}			{ return( _TLA(GT) ); }
{lte}			{ return( _TLA(LTE) ); }
{gte}			{ return( _TLA(GTE) ); }
{dlt}			{ return( _TLA(DLT) ); }
{dgt}			{ return( _TLA(DGT) ); }
{sum}			{ return( _TLA(SUM) ); }
{dif}			{ return( _TLA(DIF) ); }
{div}			{ return( _TLA(DIV) ); }
{mul}			{ return( _TLA(MUL) ); }
{pow}			{ return( _TLA(POW) ); }
{increment}		{ return( _TLA(INCREMENT) ); }
{decrement}		{ return( _TLA(DECREMENT) ); }
{amp}			{ return( _TLA(AMP) ); }
{damp}			{ return( _TLA(DAMP) ); }
{pipe}			{ return( _TLA(PIPE) ); }
{dpipe}			{ return( _TLA(DPIPE) ); }
{at}			{ return( _TLA(AT) ); }

{string_lit} 		{ return( _TLA(STRING_LIT)); } 

<<EOF>>			{ lineno = 1; return(EXIT); }

%%

