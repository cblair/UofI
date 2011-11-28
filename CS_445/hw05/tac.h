#ifndef _TAC_H
#define _TAC_H

#include "tac_x86_opcodes.h"
	
struct tac_inst
{
	enum OPCODE opcode;
	char *op; //operator
	char *o1; //operand 1
	char *o2; //operand 2
	char *result;
};

struct tac_inst_list
{
	struct tac_inst *t;
	struct tac_inst_list *next;
};


//global list of tac inst's
struct tac_inst_list *TAC_CODE;


struct tac_inst *tac_inst_new(char *, char*, char*, char*);

struct tac_inst_list *tac_inst_list_new(char*, char*, char*, char*);
int tac_inst_list_append(struct tac_inst_list *, char*, char*,  char*, char*);

void tac_inst_list_print(struct tac_inst_list*, char*);


#endif
