#include <stdlib.h>

#include "mem_add.h"


struct mem_add_list *mem_add_list_new(enum REGION region)
{
	struct mem_add_list *retval = (struct mem_add_list*)malloc(
					sizeof(struct mem_add_list));
	retval->p = mem_add_new(region, 0);
	retval->next = NULL;

	return(retval);
}


//returns 0 for success, 1 for failure
int mem_add_list_append(struct mem_add_list **l, enum REGION region)
{
	struct mem_add_list *p;
	enum REGION last_region_seen;

	//Calculate offset
	int offset;
	int last_offset_seen = 0;

	//find the last seen region
	for(p = (*l); p != NULL; p = (*l)->next)
	{
		if(p->p != NULL) //should never == NULL
		{
			last_region_seen = p->p->region;
			last_offset_seen = p->p->offset;
		}
	}

	//if the last seen region is different than the new region, offset
	// should be 0
	if(last_region_seen != region)
	{
		offset = 0;
	}
	//else, it should be the last seen offset plus the step size
	else
	{
		//negative since we grow in the negative direction
		offset = - (last_offset_seen + STEP_SIZE);
	}

	//Append the new list entry
	//goto the last element in (*l)
	for(p = (*l); p->next != NULL; p = (*l)->next)
	{;}
	//p now set to last element in (*l)

	//create new entry
	p->next = (struct mem_add_list*)malloc(sizeof(struct mem_add_list));
	p->next->p = mem_add_new(region, offset);
	p->next->next = NULL;

	return(0); 
}


struct mem_add *mem_add_new(enum REGION region, int offset)
{
	struct mem_add *retval = (struct mem_add*)malloc(
						sizeof(struct mem_add));
	retval->region = region;
	retval->offset = offset;
}
