%{

#include "ecma.tab.h"
#include "main.h"
#include "tokenlist.h"

//macro for the tokenlist_append argument
#define _TLA(CATEGORY)  tokenlist_append((int)CATEGORY, yytext, lineno, YY_FNAME)

int lineno;
extern char *YY_FNAME;

%}

id	[a-z]+[a-zA-Z]+ 
name	[A-Z]+[a-zA-Z]+ 
num	[0-9]+
punc	[^ \n\t]

string	"\""([^\"\n]*)"\""

%%
"//".*$	      		{ }
"EXIT"        		{ return(EXIT); }
"\n"          		{ ++lineno; }
" "			{ /*white space, no op for now*/ }
"\t"			{ /*white space, no op for now*/ }

"as"			{ return( _TLA(AS) ); }
"break"
"case" 
"catch"
"class" 
"const"
"continue"
"default"
"delete" 
"do" 
"else"
"extends"
"false"
"finally"
"for"
"function"
"if"
"implements"
"import"
"in"
"instanceof"
"interface"
"internal" 
"is"
"new"
"null"
"package"
"private"
"protected"
"public"
"return"
"super"
"switch"
"this"
"throw"
"to"
"true"
"try"
"typeof"
"use"
"var"
"void"
"while"
"with"

"each"
"get"
"set"
"namespace"
"include"
"dynamic" 
"final" 
"native"
"override"
"static"

{id}		{ return( _TLA(ID) ); } 
{name}		{ return( _TLA(NAME) ); }
{num}		{ return( _TLA(NUM) ); }
{punc}		{ return( _TLA(PUNC) ); }

{string} 	{ return( _TLA(STRING)); } 

<<EOF>>		{ return(EXIT); }

%%

