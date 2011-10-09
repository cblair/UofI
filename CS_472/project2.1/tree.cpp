#include <time.h>
#include <iomanip>

#include "tree.h"
#include "tree_node.h"
#include "main.h"


///////////////////////////////////////////////////////////////////////////
//Tree
///////////////////////////////////////////////////////////////////////////

tree::tree(int depth, darray *dp)
{
	if(depth == 0)
	{
		//have to be a terminal!
		return this->gen_rand_terminal_tree_node(dp);
	}

	this->dp = dp;
	this->tnp = this->gen_rand_tree_node(dp);

	//init children
	for(int i = 0; i < MAX_CHILDREN; i++)
	{
		this->children[i] = NULL;
	}

	//gen random amount of children
	/* initialize random seed: */
	srand ( clock() );

	/* generate rand number of children*/
	this->nchildren = rand() % MAX_CHILDREN + 1;	
	for(int i = 0; i < this->nchildren; i++)
	{
		DEBUGMSG << "DEBUG: tree.cpp: Gen child " << i  << "at depth " << depth << endl;	
		this->children[i] = new tree(depth - 1, dp);
	}
}


tree_node *tree::gen_rand_tree_node(darray *dp)
{
	tree_node *tp;

	/* initialize random seed: */
	srand ( clock() );

	/* generate secret number: */
	int type = rand() % NTYPES;	

	DEBUGMSG << "DEBUG: tree.cpp: Generating rand node with type " << type << endl;
	switch (type)
	{
		case 0:
		{
			tp = new tree_node(tree_node::plus, 0);
			break;
		}	
		case 1:
		{
			tp = new tree_node(tree_node::minus, 0);
			break;
		}
		case 2:
		{
			tp = new tree_node(tree_node::multi, 0);
			break;
		}
		case 3:
		{
			tp = new tree_node(tree_node::div, 0);
			break;
		}
		case 4:
		{
			/* initialize random seed: */
			srand ( clock() );

			/* generate secret number: */
			double d = ((double)rand()/(double)RAND_MAX);	

			tp = new tree_node(tree_node::tree_double, 1, d);
			break;
		}
		case 5:
		{
			tp = new tree_node(tree_node::tree_var, 1, dp);
			break;
		}
		default:
			DEBUGMSG << "DEBUG: tree.cpp: No type for node\n";
			exit(1);
		}
	
		return(tp);
}


bool tree::print(int depth)
{
	if(this == NULL)
	{
		//false if I am a child that didn't get a value
		return(false);
	}

	cout << string(depth, ' ') << depth << ":";
	this->tnp->print_ntype();
	cout << endl;

	for(int i = 0; i <= this->nchildren; i++)
	{
		this->children[i]->print(depth + 1);
	}
	
	return(true);
}


///////////////////////////////////////////////////////////////////////////
//DArray
///////////////////////////////////////////////////////////////////////////

darray::darray(int size)
{
	this->size = size;

	//init with nulls
	for(int i = 0; i < MAX_BUF; i++)
	{
		this->a[i] = NULL;
	}

	//re-init with rand vals
	for(int i = 0; i < this->size; i++)
	{
		/* initialize random seed: */
		srand ( clock() );

		/* generate secret number: */
		this->a[i] = ((double)rand()/(double)RAND_MAX);	
	}
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
