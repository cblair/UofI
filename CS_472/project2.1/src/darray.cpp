#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "darray.h"

using namespace std;

darray::darray(int size, bool rand_gen)
{
	this->size = size;

	//init with nulls
	for(int i = 0; i < MAX_BUF; i++)
	{
		this->a[i] = NULL;
	}

	if(rand_gen == true)
	{
		//re-init with rand vals
		for(int i = 0; i < this->size; i++)
		{
			/* initialize random seed: */
			srand ( clock() );
	
			/* generate secret number: */
			this->a[i] = ((double)rand()/(double)RAND_MAX);	
		}
	}
	//else, need to set manually, ie darray->a[0..n] = 1,2,...
}


bool darray::copy(darray **to)
{
	//init 'to' with our size
	(*to) = new darray(this->size, false);
	//TODO: return false if new failed

	//copy over all the elements in this->a
	for(int i = 0; i < this->size; i++)
	{
		(*to)->a[i] = this->a[i];
	}

	return(true);
}

int darray::get_size()
{
	return(this->size);
}


double darray::get_val(int i)
{
	if(i >= this->size)
	{
		return(NULL);
	}

	return(this->a[i]);
}


bool darray::print_vals()
{
	for(int i = 0; i < this->size; i++)
	{
		cout << this->a[i];
		//I dunno, 5 vals per line sounds good
		if( (i % 5) == 0 && i != 0) 
		{
			cout << endl;
		}
		else //a delim
		{
			cout << " : ";
		}
	}

	cout << endl;

	return(true); 
}
