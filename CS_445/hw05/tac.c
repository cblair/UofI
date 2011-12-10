#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "main.h"

#include "tac.h"

#define EIC_FNAME_POSTFIX "eic"


//tac_inst's

struct tac_inst *tac_inst_new(int nargs, ...)
{
	if(nargs > MAX_TAC_ATTRS)
	{
		fprintf(stderr,
			"ERROR: internal tac, too many attributes inserted. Exiting.\n");
		exit(ERROR_INTERNAL);
	}

	struct tac_inst *retval = \
		(struct tac_inst*)malloc(sizeof(struct tac_inst));

	//TODO: need way of creating this based on arch type
	//retval->opcode = ?;
	va_list args;
  	va_start ( args, nargs );
	int i;
  	for(i = 0; i < nargs; i++)
	{
		retval->s[i] = va_arg ( args, char* ); 
  	}
	va_end ( args );

	retval->n = nargs;
	
	return(retval);
}


//tac_inst_lists's
struct tac_inst_list *tac_inst_list_new(struct tac_inst *t)
{
	struct tac_inst_list *retval = \
		(struct tac_inst_list*)malloc(sizeof(struct tac_inst_list));

	retval->t = t;
	retval->next = NULL;

	return(retval);
}


int tac_inst_list_del(struct tac_inst_list *code)
{
	//TODO
}


int tac_inst_list_append(struct tac_inst_list *list, struct tac_inst *inst)
{
	if(inst == NULL)
	{
		fprintf(stderr, 
			"ERROR: trying to append a null tac list instruction. Exiting.\n");
		exit(ERROR_INTERNAL);
	}

	struct tac_inst_list *new_list = (struct tac_inst_list*)malloc(
					sizeof(struct tac_inst_list));
	new_list->t = inst;
	new_list->next = NULL;

	//if the existing list head is null, set it to the new list
	if(list == NULL)
	{
		list = new_list;
	}
	//else, append
	else
	{
		struct tac_inst_list *p;
		//stop at the last list element
		for(p = list; p->next != NULL; p = p->next)
		{;}
		p->next = new_list;
	}

	return(0); //success
}


void tac_inst_list_print(struct tac_inst_list *code)
{
	printf("TAC code:\n");

	struct tac_inst_list *p;
	for(p = code; p != NULL; p = p->next)
	{
		int i;
		for(i = 0; i < p->t->n; i++)
		{
			printf("%5s ", p->t->s[i]);
		}
		printf("\n");
	}
}


char *tac_inst_list_fname_ectoscriptize(char *fname)
{
	if(fname == NULL)
	{
		return(NULL);
	}

	//if we found a '.', strip it
	// set p to the last '.' pos
	int p = 0;
	int i;
	for(i = 0; i < strlen(fname); i++)
	{
		if(fname[i] == '.')
		{
			p = i;
		}
	}
	if(p != 0)
	{
		fname[p] = '\0';
	}

	char buffer[strlen(fname) + strlen(EIC_FNAME_POSTFIX)];
	sprintf(buffer, "%s.%s", fname, EIC_FNAME_POSTFIX);
	char *retval = strdup(buffer);

	return(retval);
}


int tac_inst_list_save(struct tac_inst_list *code, char *fname)
{
	if(fname == NULL)
	{
		return(1); //fail
	}

	FILE *fp; //file pointer
	fp = fopen(fname,"w");

	struct tac_inst_list *p;
	for(p = code; p != NULL; p = p->next)
	{
		int i;
		for(i = 0; i < p->t->n; i++)
		{
			fprintf(fp,"%s ", p->t->s[i]);
		}
		fprintf(fp,"\n");
	}

	return(fclose(fp)); /*done!*/ 
}
