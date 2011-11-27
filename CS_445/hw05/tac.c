#include <stdlib.h>
#include <stdio.h>

#include "tac.h"


//tac_inst's

struct tac_inst *tac_inst_new(char *result, char *op, char *o1, char *o2)
{
	struct tac_inst *retval = \
		(struct tac_inst*)malloc(sizeof(struct tac_inst));

	//TODO: need way of creating this based on arch type
	//retval->opcode = ?;
	retval->op = op;
	retval->o1 = o1;
	retval->o2 = o2;
	retval->result = result;

	return(retval);
}


//tac_inst_lists's

struct tac_inst_list *tac_inst_list_new(char *result,
					char *op, char *o1, char *o2)
{
	struct tac_inst_list *retval = \
		(struct tac_inst_list*)malloc(sizeof(struct tac_inst_list));

	retval->t = tac_inst_new(result, op, o1, o2);
	retval->next = NULL;

	return(retval);
}


int tac_inst_list_del(struct tac_inst_list *code)
{
	//TODO
}


int tac_inst_list_append(struct tac_inst_list *list, char *result, char *op, 
			char *o1, char *o2)
{
	struct tac_inst_list *new_list = tac_inst_list_new (result, op, o1, o2);

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
		printf("%5s : %5s %5s %5s\n", p->t->result, p->t->o1, 
							p->t->op, p->t->o2);
	}
}
