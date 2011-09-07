///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 1
//Author:	Colby Blair
//File name:	main.h
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "main.h"

FILE *yyin;
char yytext[1024];

int main(int argc, char *argv[])
{
	yytext[0] = '\0';

	int i;
	//for each filename given on the command line
	for(i = 1; i < argc; i++)
	{
		//open the file and store its reference in global variable yyin
		char *yyfname = argv[i];
		yyin = fopen(yyfname,"r");
		if (yyin == NULL) 
		{
			fprintf(stderr, "ERROR: Cannot open '%s'. Ignoring.\n",\
								yyfname);
			break;
	        }

		//while not end-of-file do
		int cat = yylex();
		while(cat != EXIT)
		{
			printf("%d\t%s\n", cat, lexeme);
			//clear_yytext();
			yytext[0] = 0; //clear our yytext
			cat = yylex();
		}
		fclose(yyin);
	}
}
