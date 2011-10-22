///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 3
//Author:	Colby Blair
//File name:	main.c
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "parser.tab.h"
#include "main.h"
#include "tree.h"

FILE *yyin;
char *YY_FNAME;
extern struct tree *YY_TREE;
extern int lineno;
extern int colno;
char yytext[YY_MAX_BUF];

int main(int argc, char *argv[])
{
	tree_init();
	yytext[0] = '\0';

	int i;
	//for each filename given on the command line
	for(i = 1; i < argc; i++)
	{
		//open the file and store its reference in global variable yyin
		YY_FNAME = argv[i];
		//yyrestart for multiple file parsing
		FILE *yyfile = fopen(YY_FNAME, "r");
        	yyrestart(yyfile); lineno = 1; colno = 1;
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

		#ifdef DEBUG_TREE
		//print our results
		treeprint(YY_TREE,0);
		#endif
		DEBUGMSG("DEBUG: done with file '%s'\n", YY_FNAME);
	}

	//generate code
	tree_gen_tac(YY_TREE, 0);

	tree_del();
}
