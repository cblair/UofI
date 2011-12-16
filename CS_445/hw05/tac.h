///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 6
//Author:	Colby Blair
//File name:	tac.h
///////////////////////////////////////////////////////////////////////////////

#ifndef _TAC_H
#define _TAC_H

#include <stdarg.h>

#include "tac_x86_opcodes.h"

#define MAX_TAC_ATTRS 10

struct tac_inst
{
	enum OPCODE opcode;
	char *s[MAX_TAC_ATTRS];
	int n;
};

struct tac_inst_list
{
	struct tac_inst *t;
	struct tac_inst_list *next;
};


//global list of tac inst's
struct tac_inst_list *TAC_CODE;

struct tac_inst *tac_inst_new(int, ...);

struct tac_inst_list *tac_inst_list_new(struct tac_inst*);
int tac_inst_list_append(struct tac_inst_list *, struct tac_inst *);

void tac_inst_list_print(struct tac_inst_list*);

char *tac_inst_list_fname_ectoscriptize(char*);
int tac_inst_list_save(struct tac_inst_list*, char*);


#endif
