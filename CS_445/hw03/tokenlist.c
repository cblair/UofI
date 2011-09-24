///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 2
////Author:       Colby Blair
////File name:    tokenlist.c
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ecma.tab.h"
#include "tokenlist.h"

struct tokenlist *YY_TOKENLIST;
char *YY_FNAME;
extern int lineno;

int tokenlist_init()
{
	YY_TOKENLIST = NULL;
	lineno = 1;
}


int tokenlist_del()
{

}


char* tokenlist_print()
{
printf("\
Category	Text		Lineno		Filename	Ival/Sval\n\
-------------------------------------------------------------------------\n");

	struct tokenlist *tlp; 
	for(tlp = YY_TOKENLIST; tlp != NULL; tlp = tlp->next)
	{
		if(tlp->t->category == INT_LIT || tlp->t->category == REAL_LIT)
		{
			printf("%d\t\t%s\t\t%d\t\t%s\t%010X\n", 
					tlp->t->category, 
					tlp->t->text,
					tlp->t->lineno,
					tlp->t->filename,
					tlp->t->ival
					);
		}
		else if(tlp->t->category == STRING_LIT)
		{
			printf("%d\t\t%-16s%d\t\t%s\t%s\n", 
					tlp->t->category, 
					tlp->t->text,
					tlp->t->lineno,
					tlp->t->filename,
					tlp->t->sval
					);
	
		}
		else
		{
			printf("%d\t\t%s\t\t%d\t\t%s\n", 
					tlp->t->category, 
					tlp->t->text,
					tlp->t->lineno,
					tlp->t->filename
					);
	
		}
	}
}


int tokenlist_append(int cat, char *token_text, int lineno, char *fname)
{
	//make a new token structure
	struct token *token_new = (struct token*)malloc(sizeof(struct token));
	token_new->category = cat;
	token_new->text = strdup(token_text);
	token_new->lineno = lineno;
	token_new->filename = fname;
	if(cat == INT_LIT || cat == REAL_LIT)
	{	
		token_new->ival = atof(token_text);
		token_new->sval = NULL;
	}
	else if(cat == STRING_LIT)
	{
		token_new->ival = 0;
		token_new->sval = strdup(tokenlist_strip_quotes(token_text));
	}

	//if empty list, initialize memory
	if(YY_TOKENLIST == NULL)
	{
		YY_TOKENLIST = (struct tokenlist*)malloc(sizeof(struct tokenlist));
		//TODO: malloc check
		YY_TOKENLIST->t = token_new;
		YY_TOKENLIST->next = NULL;
	}
	else
	{
		//go to the last element in the list
		struct tokenlist *tlp;
		for(tlp = YY_TOKENLIST; tlp->next != NULL; tlp = tlp->next)
		{ ; }

		//set the last element's next to the new token structure
		tlp->next = (struct tokenlist*)malloc(sizeof(struct tokenlist));
		tlp->next->t = token_new;
		tlp->next->next = NULL; //init next pointer to NULL
	}

	return(cat); 
}

char *tokenlist_strip_quotes(char *text)
{
	//null check
	if(text == NULL)
	{
		return(text);
	}

	//single qoute strip
	int end = strlen(text) - 1;
	if(text[0] == '\'' && text[end] == '\'')
	{
		text++; //strip first '
		end--; //decrement end index
		text[end] = '\0'; //strip first '
	}
	//double qoute strip
	else if(text[0] == '"' && text[end] == '"')
	{
		text++; //strip first "
		end--; //decrement end index
		text[end] = '\0'; //strip last "
	}

	return(text);
}
