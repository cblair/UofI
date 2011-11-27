#ifndef _MEM_ADD_H
#define _MEM_ADD_H


//TODO: should be calculated of 32/64 bitness of arch
#define STEP_SIZE 8 //size of memory offset


enum REGION {GLOBAL_R, CLASS_R, PARAMETER_R, LOCAL_R, UNKNOW_R};


struct mem_add
{
	enum REGION region;
	int offset;
};


struct mem_add *mem_add_new(enum REGION, int);

void mem_add_print(struct mem_add*);

#endif
