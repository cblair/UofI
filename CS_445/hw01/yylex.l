%{

#include "main.h"

int nlin;

%}

id	[a-z]+[a-zA-Z]+ 
name	[A-Z]+[a-zA-Z]+ 
num	[0-9]+
punc	[^ \n]
mix	({id}|{name}|{num}|{punc})+

%%
"#".*$	      		{ lexeme = ""; }
"EXIT"        		{ lexeme = ""; return(EXIT); }
"\n"          		{ lexeme = ""; ++nlin; }
" "			{ lexeme = ""; /*white space, no op for now*/ }
{id}			{ lexeme = strdup(yytext); return(ID); 	}
{name}			{ lexeme = strdup(yytext); return(NAME); 	}
{num}	 		{ lexeme = strdup(yytext); return(NUM); 	}
{punc}	 		{ lexeme = strdup(yytext); return(PUNC); 	}
{mix}	 		{ lexeme = strdup(yytext); return(MIX); 	}
<<EOF>>      		{ lexeme = ""; return(EXIT); }

%%

