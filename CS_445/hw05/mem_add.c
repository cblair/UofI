///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 6
//Author:	Colby Blair
//File name:	mem_add.c
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>

#include "mem_add.h"


struct mem_add *mem_add_new(enum REGION region, int offset)
{
	struct mem_add *retval = (struct mem_add*)malloc(
						sizeof(struct mem_add));
	retval->region = region;
	retval->offset = offset;

	return(retval);
}

void mem_add_print(struct mem_add *map)
{
	if(map == NULL)
	{
		printf("(null)");
	}

	switch (map->region)
	{
		case GLOBAL_R:
		{
			printf("global-region");
			break;
		}
		case CLASS_R:
		{
			printf("class-region");
			break;
		}
		case PARAMETER_R:
		{
			printf("parameter-region");
			break;
		}
		case LOCAL_R:
		{
			printf("local-region");
			break;
		}
		default:
		{
			printf("UNKNOWN-REGION");
			break;
		}
	}

	printf(":%d (byte %d)", map->offset, map->offset * STEP_SIZE);
}
