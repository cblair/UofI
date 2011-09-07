///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 1
//Author:	Colby Blair
//File name:	yylex.c
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#define YY_MAX_BUF 1024

extern FILE *yyin;
extern char yytext[YY_MAX_BUF];

int YY_FILE_READ = 0;
char * YY_FILE_TEXT;

//returns 1 for true, else 0
int is_id(char *val)
{
	int retval = 0;
	if(val == NULL)
	{
		retval = 0;
	}
	else if( (int)val[0] >= (int)'a' 
		&& (int)val[0] <= (int)'z') //if begins with ascii lower case
	{
		int i;
		retval = 1; //true until we find a non-letter
		for(i = 1; i < strlen(val); i++)
		{
			int i_val = (int)val[i];
			if( i_val < (int)'A' //if below 'A'
			    || (i_val > (int)'Z' && i_val < (int)'a')
			    || (i_val > (int)'z'))
			{
				retval = 0; //not true
			}
		}
	}

	return(retval);
}


//returns 1 for true, else 0
int is_name(char *val)
{
	int retval = 0;
	if(val == NULL)
	{
		retval = 0;
	}
	else if( (int)val[0] >= (int)'A' 
		&& (int)val[0] <= (int)'Z') //if begins with ascii upper case
	{
		int i;
		retval = 1; //true until we find a non-letter
		for(i = 1; i < strlen(val); i++)
		{
			int i_val = (int)val[i];
			if( i_val < (int)'A' //if below 'A'
			    || (i_val > (int)'Z' && i_val < (int)'a')
			    || (i_val > (int)'z'))
			{
				retval = 0; //not true
			}
		}
	}

	return(retval);
}


//returns 1 for true, else 0
int is_num(char *val)
{
	int retval = 0;
	if(val == NULL)
	{
		retval = 0;
	}
	else if( 
		( (int)val[0] >= (int)'0' 
		   && (int)val[0] <= (int)'9' //if begins with ascii number
		)
		|| val[0] == '.' || val[0] == '-' //or leading '-' or '.'
		)
	{
		int i;
		retval = 1; //true until

		//check that there is only one '.'
		int period_count = 0;
		for(i = 0; i < strlen(val); i++)
		{
			if(val[0] == '.')
			{
				period_count++;
			}
		}
		if(period_count > 1)
		{
			retval = 0;
		}

		//check that all chars are nums or '.'s
		for(i = 1; i < strlen(val); i++)
		{
			int i_val = (int)val[i];
			if( (i_val < (int)'0' //if below 'A'
			    || i_val > (int)'9')
			    && val[i] != '.'
			  )
			{
				retval = 0; //not true
			}
		}
	}

	return(retval);
}


//returns 1 for true, else 0
int is_punc(char *val)
{
	int retval = 0;
	if(val == NULL)
	{
		retval = 0;
	}
	else if(strlen(val) > 1)
	{
		retval = 0;
	}
	else 
	{
		if( 
			( 
	   			(int)val[0] >= (int)'!' 
			   	&& (int)val[0] <= (int)'/' 
				//if begins with ascii '!'-'/'
			)
			|| 
			(
			   	(int)val[0] >= (int)':' 
		   		&& (int)val[0] <= (int)'@' 
				//if begins with ascii ':'-'@'
			)
			||
			(
		   		(int)val[0] >= (int)'{' 
		   		&& (int)val[0] <= (int)'~' 
				//if begins with ascii '{'-'~'
			)
	       	)
		{
			retval = 1;
		}
	} //end else (if val != NULL)

	return(retval);
}


//returns 1 for true, else 0
int is_mix(char *orig_val)
{
	int retval = 0;

	//create a copy so we don't kill the original
	char *val = strdup(orig_val); 
	
	if(val == NULL)
	{
		retval = 0;
	}
	else if(is_id(val) != 0 || is_name(val) != 0 || is_num(val) != 0
		|| is_punc(val) != 0)
	{
		//add. check - if pure token, is not a mixture
		retval = 0;
	}
	else
	{
	
		int tok_found = 1; //at least one search
		while(tok_found)
		{
			tok_found = 0; //new search
			//start at end of string
			int i;
			int tok_loc = strlen(val) - 1;
			for(i = strlen(val) - 1; i >= 0; i--)
			{
				char *pch = &val[i];
				int is_tok = is_id(pch) != 0 || \
				is_name(pch) != 0 || is_num(pch) != 0 || \
				is_punc(pch) != 0;

				if(is_tok != 0)
				{
					tok_found = 1;
					tok_loc = i;
				}
				//printf(" '%s': %10d\t%5d\n", 
				//	pch, is_tok, tok_loc);
			}
			
			//if token was found, peel it off the end
			if(tok_found != 0)
			{
				val[tok_loc] = '\0';
			}
		}
	}

	//if we peeled off all chars b/c they belonged to tokens, then
	// we have a 'mixed' type
	if(strlen(val) == 0)
	{
		retval = 1;
	}

	free(val);
	return(retval);
}


int yy_get_cat(char *val)
{
	int retval = 0;

	if(is_id(val) != 0)
	{
		retval = 1;
	}
	else if(is_name(val) != 0)
	{
		retval = 2;
	}
	else if(is_num(val) != 0)
	{
		retval = 3;
	}
	else if(is_punc(val))
	{
		retval = 4;
	}
	else if(is_mix(val) != 0)
	{
		retval = 5;
	}

	return(retval);
}

void yy_read_file()
{
	long lSize;
	char * buffer;
	size_t result;

	// obtain file size:
	fseek (yyin , 0 , SEEK_END);
	lSize = ftell (yyin);
	rewind (yyin);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,yyin);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	//copy buffer into YY_FILE_TEXT
	YY_FILE_TEXT = strdup(buffer);
	free(buffer);
}

int yylex()
{
	int retval;
	char *delim = " \n\t";

	if (YY_FILE_READ == 0) //file not yet read
	{
		yy_read_file();
		YY_FILE_READ = 1;
	
		//set lexeme
  		lexeme = strtok (YY_FILE_TEXT,delim);
	}
	else
	{
		//reset lexeme
  		lexeme = strtok (NULL,delim);
	}

  	if(lexeme == NULL)
	{
		lexeme = "";
		YY_FILE_READ = 0; //ready for next file
		retval = EXIT; 
	}
	else
	{
		retval = yy_get_cat(lexeme); 
	}
	
	return(retval);
}
