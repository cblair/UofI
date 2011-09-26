///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 2
//Author:	Colby Blair
//File name:	main.c
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "ecma.tab.h"
#include "main.h"
#include "tokenlist.h"

FILE *yyin;
extern char *YY_FNAME;
extern int lineno;
char yytext[YY_MAX_BUF];

int main(int argc, char *argv[])
{
	tokenlist_init();
	yytext[0] = '\0';

	int i;
	//for each filename given on the command line
	for(i = 1; i < argc; i++)
	{
		//open the file and store its reference in global variable yyin
		YY_FNAME = argv[i];
		//yyrestart for multiple file parsing
		FILE *yyfile = fopen(YY_FNAME, "r");
        	yyrestart(yyfile); lineno = 1;
        	//instead of - yyin = fopen(YY_FNAME,"r");
		if (yyin == NULL) 
		{
			fprintf(stderr, "ERROR: Cannot open '%s'. Ignoring.\n",\
								YY_FNAME);
			continue;
	        }
		
		//while not end-of-file do
		/*int cat = yylex(); //get first token
		while(cat != EXIT)
		{
			yytext[0] = 0; //clear our yytext
			cat = yylex(); //get next token
		}
		fclose(yyin);
		*/
		yyparse();

		//print our results
		//tokenlist_print();		
		DEBUGMSG("DEBUG: done with file '%s'\n", YY_FNAME);
	}

	tokenlist_del();
}
