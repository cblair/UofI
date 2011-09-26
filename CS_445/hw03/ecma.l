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

//#define DEBUG_PARSER

//macro for the tokenlist_append argument
#ifdef DEBUG_PARSER
#define _TLA(CATEGORY)  DEBUGMSG("DEBUG: %d - %s \t\t\tline: %d\n", CATEGORY, yytext, lineno); yylval.sval = strdup(yytext); return( tokenlist_append((int)CATEGORY, yytext, lineno, YY_FNAME) );
#else
#define _TLA(CATEGORY)  yylval.sval = strdup(yytext); return( tokenlist_append((int)CATEGORY, yytext, lineno, YY_FNAME) );
#endif

int lineno;
extern char *YY_FNAME;

%}

id		[a-zA-Z_]+[a-zA-Z0-9_]+ 
int_lit		[0-9]+
real_lit	[0-9]*"."[0-9]+

string_lit	"\""[^\"\n]*"\""|"'"[^\"\n]*"'" 

%%
"//".*$	      		{ }
"\n"          		{ ++lineno; }
" "			{ /*white space, no op for now*/ }
"\t"			{ /*white space, no op for now*/ }

"as"			{ _TLA(AS ) }
"Array"			{ _TLA(ARRAY ) }
"Boolean"		{ _TLA(BOOLEAN ) } 
"break"			{ _TLA(BREAK ) } 
"case" 			{ _TLA(CASE ) } 	
"catch"			{ _TLA(CATCH ) } 
"class"		 	{ _TLA(CLASS ) } 
"const"			{ _TLA(CONST ) }
"continue"		{ _TLA(CONTINUE ) }
"default"		{ _TLA(DEFAULT ) }
"delete"		{ _TLA(DELETE ) }
"do"			{ _TLA(DO ) }
"else"			{ _TLA(ELSE ) }
"extends"		{ _TLA(EXTENDS ) }
"false"			{ _TLA(FALSE ) }
"finally"		{ _TLA(FINALLY ) }
"for"			{ _TLA(FOR ) }
"function"		{ _TLA(FUNCTION ) }
"if"			{ _TLA(IF ) }
"implements"		{ _TLA(IMPLEMENTS ) }
"import"		{ _TLA(IMPORT ) }
"in"			{ _TLA(IN ) }
"instanceof"		{ _TLA(INSTANCEOF ) }
"int"			{ _TLA(INT ) }
"interface"		{ _TLA(INTERFACE ) }
"internal" 		{ _TLA(INTERNAL ) }
"is"			{ _TLA(IS ) }
"new"			{ _TLA(NEW ) }
"null"|"Null"		{ _TLA(NULL_LIT) }
"Number"		{ _TLA(NUMBER ) }
"Object"		{ _TLA(OBJECT ) }
"package"		{ _TLA(PACKAGE ) }
"private"		{ _TLA(PRIVATE ) }
"protected"		{ _TLA(PROTECTED ) }
"public"		{ _TLA(PUBLIC ) }
"return"		{ _TLA(RETURN ) }
"String"		{ _TLA(STRING ) }
"super"			{ _TLA(SUPER ) }
"switch"		{ _TLA(SWITCH ) }
"this"			{ _TLA(THIS) }
"throw"			{ _TLA(THROW ) }
"to"			{ _TLA(TO ) }
"true"			{ _TLA(TRUE ) }
"try"			{ _TLA(TRY ) }
"typeof"		{ _TLA(TYPEOF ) }
"uint"			{ _TLA(UINT ) }
"use"			{ _TLA(USE ) }
"var"			{ _TLA(VAR ) }
"void"			{ _TLA(VOID ) }
"while"			{ _TLA(WHILE ) }
"with"			{ _TLA(WITH ) }

"each"			{ _TLA(EACH ) }
"get"			{ _TLA(GET ) }
"set"			{ _TLA(SET ) }
"namespace"		{ _TLA(NAMESPACE ) }
"include"		{ _TLA(INCLUDE ) }
"dynamic"		{ _TLA(DYNAMIC ) }
"final"			{ _TLA(FINAL ) }
"native"		{ _TLA(NATIVE ) }
"override"		{ _TLA(OVERRIDE ) }
"static"		{ _TLA(STATIC ) }

";"			{ _TLA(SEMI) }
"="			{ _TLA(EQ) }
","			{ _TLA(COMMA) }
"/"			{ _TLA(SLASH) }
"\\"	 		{ _TLA(BSLASH) }
"[" 			{ _TLA(LBRACK) }
"]" 			{ _TLA(RBRACK) }
"{"			{ _TLA(LBRACE) }
"}"			{ _TLA(RBRACE) }
"("			{ _TLA(LPAREN) }
")"			{ _TLA(RPAREN) }
"'" 			{ _TLA(APOSTREPHE) }
"\""			{ _TLA(QUOTE) }
"?"			{ _TLA(QUESTION) }
":"			{ _TLA(COLON) }
"@"			{ _TLA(AT) }
"&"			{ _TLA(AMPER) }
"+"			{ _TLA(PLUS) }
"-"			{ _TLA(MINUS) }
"$"			{ _TLA(DOLLAR) }
"_"			{ _TLA(UNDERSCORE) }
"."			{ _TLA(DOT) }
"<"			{ _TLA(LT) }
">"			{ _TLA(GT) }
"<="			{ _TLA(LTEQ) }
">="			{ _TLA(GTEQ) }
"=="			{ _TLA(EQ2) }
"!="			{ _TLA(NOTEQ) }
"==="			{ _TLA(EQ3) }
"!=="			{ _TLA(NOTEQ2) }
"*"			{ _TLA(STAR) }
"%"			{ _TLA(PERCENT) }
"++"			{ _TLA(PLUS2) }
"--"			{ _TLA(MINUS) }
"<<"			{ _TLA(LSHIFT) }
">>"			{ _TLA(RSHIFT) }
">>>"			{ _TLA(GT3) }
"|"			{ _TLA(PIPE) }
"^"			{ _TLA(CAROT) }
"!"			{ _TLA(EXCLAMATION) }
"~"			{ _TLA(TILDE) }
"&&"			{ _TLA(AMPER2) }
"||"			{ _TLA(PIPE2) }
"+="			{ _TLA(PLUSEQ) }
"-="			{ _TLA(MINUSEQ) }
"*="			{ _TLA(TIMESEQ) }
"%="			{ _TLA(PERCENTEQ) }
"<<="			{ _TLA(LSHIFTEQ) }
">>="			{ _TLA(RSHIFTEQ) }
">>>="			{ _TLA(GT3EQ) }
"&="			{ _TLA(AMPEREQ) }
"|="			{ _TLA(PIPEEQ) }
"^="			{ _TLA(CAROTEQ) }
'/'			{ _TLA(DIVIDE) }
"/="			{ _TLA(DIVIDEEQ) }
'0'			{ _TLA(ZERO) }
[1-9]			{ _TLA(NON_DIGIT) }
[0-9]			{ _TLA(DIGIT) }
'f'			{ _TLA(LOWER_F) }
'c'			{ _TLA(LOWER_C) }
'n'			{ _TLA(LOWER_N) }
'r'			{ _TLA(LOWER_R) }
't'			{ _TLA(LOWER_T) }
'v'			{ _TLA(LOWER_V) }
'e'|'E'			{ _TLA(EXPONENT_INDICATOR) }
'f'|'n'|'r'|'t'|'v'	{ _TLA(CONTROL_ESCAPE_CHAR) }
'b'			{ _TLA(LOWER_B) }
'x'			{ _TLA(LOWER_X) }
'u'			{ _TLA(LOWER_U) }
[a-z]|[A-Z]		{ _TLA(ALPHA_CHARACTER) }
[a-z]			{ _TLA(LOWER_ALPHA_CHAR) }
[A-Z]			{ _TLA(UPPER_ALPHA_CHAR) }
" "|'\r'|'\t'		{ /*ignore*/ }

"x0"			{ _TLA(HEX_PREFIX) }

{int_lit}		{ _TLA(INT_LIT ) }
{real_lit}		{ _TLA(REAL_LIT ) }
{string_lit} 		{ _TLA(STRING_LIT) } 
{id} 			{ _TLA(ID) } 


[ -~]			{ _TLA(SOURCE_CHAR) }

<<EOF>>			{ return(EXIT); }

%%

