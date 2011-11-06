#ifndef _MEM_ADD_H
#define _MEM_ADD_H

//TODO: should be calculated of 32/64 bitness of arch
#define STEP_SIZE 8 //size of memory offet


enum REGION {GLOBAL, CLASS, PARAMETER, LOCAL};


struct mem_add
{
	enum REGION region;
	int offset;
};


struct mem_add_list
{
	struct mem_add *p;
	struct mem_add_list *next;
};


struct mem_add_list *mem_add_list_new(enum REGION);
int mem_add_list_append(struct mem_add_list **, enum REGION);

struct mem_add *mem_add_new(enum REGION, int);

#endif
