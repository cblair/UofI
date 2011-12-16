#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ga_i.h"

struct ga_i *ga_i_init(double *darray, int nattr)
{
	struct ga_i *retval = (struct ga_i*)malloc(sizeof(struct ga_i));

	//Init attr
	retval->nattr = nattr;

	int i;
	for(i = 0; i < nattr; i++)
	{
		if(darray == NULL)
		{
			retval->attr[i] = 0;
		}
		else
		{
			retval->attr[i] = darray[i];
		}
	}

	return(retval);
}


void ga_i_copy(struct ga_i **to, struct ga_i **from)
{
	if((*from) == NULL)
	{
		fprintf(stderr, 
			"ERROR: copying individual, 'from' is null. Exiting.\n");
		exit(1);
	}

	//init memory
	(*to) = (struct ga_i*)malloc(sizeof(struct ga_i));

	//set attrs
	int i;
	for(i = 0; i < (*from)->nattr; i++)
	{
		(*to)->attr[i] = (*from)->attr[i];
	}

	//copy nattr
	(*to)->nattr = (*from)->nattr;

	//set the random function pointer
	(*to)->random = (*from)->random;
}


int ga_i_del(struct ga_i **p)
{
	free((*p));
	(*p) = NULL;

	return(0); //no way to check free
}


void ga_i_2p_crossover(struct ga_i **i1, struct ga_i **i2)
{
	//Sanity checks
	//nattrs equal
	if((*i1)->nattr != (*i2)->nattr)
	{
		fprintf(stderr, 
		"ERROR: individuals have different nattr during 2p crossover. Exiting.\n");
		exit(1);
	}

	//if nattrs == 0, random point gen will infinite loop
	if((*i1)->nattr <= 0)
	{
		fprintf(stderr,
			"ERROR: zero / negative nattr values during 2p crossover. Exiting.\n");
		exit(1);
	}

	/* initialize random seed: */
  	//srand ( time(NULL) );

	//Crossover points
	int p1 = 0;
	int p2 = 0;
	while(p1 == p2)
	{
		//+1 to ignore rand() == 0. each are decremented again below
		p1 = rand() % (*i1)->nattr + 1; 
		p2 = rand() % (*i1)->nattr + 1; 
	}

	//if p1 > p2, swap so p1 is the first index / point
	if(p1 > p2)
	{
		int temp = p1;
		p1 = p2;
		p2 = temp;
	}

	//decrement each index by 1 for the 0..n-1 array indexing
	p1--;
	p2--;

	//Middle segment crossover
	int i;
	for(i = p1; i < p2; i++)
	{
		int old_i1_attr_val = (*i1)->attr[i];
		(*i1)->attr[i] = (*i2)->attr[i];
		(*i2)->attr[i] = old_i1_attr_val;
	}
}


void ga_i_uniform_mutation(struct ga_i **p)
{
	if((*p) == NULL)
	{
		fprintf(stderr, 
			"ERROR: trying to mutate a null individual. Exiting.\n");
		exit(1);
	}

	int i;
	int k = (*p)->nattr / 10; //k = % of all the attributes
	for(i = 0; i < k; i++)
	{
		//select a random attribute
		int j = rand() % (*p)->nattr;

		//get a random value
		double drand = (*p)->random((*p)->attr[j]);
		
		//make the mutation
		(*p)->attr[j] = drand;
	}
}


void ga_i_print_attr(struct ga_i *p)
{
	int i;
	for(i = 0; i < p->nattr; i++)
	{
		printf("%10f", p->attr[i]);
		if( ((i+1) % 5) == 0 )
		{
			printf("\n");
		}
	}
	printf("\n");
}
