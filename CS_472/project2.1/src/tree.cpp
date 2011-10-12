#include <time.h>
#include <iomanip>
#include <cmath>
#include <cstdlib>

#include "tree.h"
#include "tree_node.h"
#include "main.h"

int SUM_TEMP;

/*
This is the structure I am trying to represent

	+---------------+
	|tree		|
	|tree_node *tnp=|------------------------------ +---------------+
	|...		|				|tree_node	|
	|int nchildren= |				|enum node_type	|
	| 2 (nonterminal)|				|double dval	|
	|tree *children[]|------			|double variable*|
	+---------------+	|			+---------------+
				|
				... (many more non-terminals)
		----------------|
		|		|
		|	+---------------+
		|	|tree		|
		|	|tree_node *tnp=|-------------- +---------------+
		|	|...		|		|tree_node	|
		|	|int nchildren= |		|enum node_type	|
		|	| 0 (terminal)	|		|double dval	|
		|	|tree *children[]|--NULL	|double variable*|
		|	+---------------+		+---------------+
	+---------------+
	|tree		|
	|tree_node *tnp=|------------------------------ +---------------+
	|...		|				|tree_node	|
	|int nchildren= |				|enum node_type	|
	| 0 (terminal)	|				|double dval	|
	|tree *children[]|-------NULL			|double variable*|
	+---------------+				+---------------+
*/

///////////////////////////////////////////////////////////////////////////
//Tree
///////////////////////////////////////////////////////////////////////////

tree::tree(int depth, darray *dp)
{
	this->dp = dp;
	
	//init null children
	this->nchildren = 0;
	for(int i = 0; i < MAX_CHILDREN; i++)
	{
		this->children[i] = NULL;
	}

	//Terminal
	// if we've reached the bottom, or a random fraction of total nodes
	// be a terminal
	/* initialize random seed: */
	srand ( clock() );
	/* generate secret number: */
	int rand_val = rand() % 10; //0-9 values
	// 1 out of 10 rand nodes get set to terminal 
	bool rand_term = (rand_val == 0);
	if(depth <= 0 || rand_term == true)
	{
		this->tnp = this->gen_rand_term_tree_node(dp);
		return;
	}

	//Nonterminal
	this->tnp = this->gen_rand_nonterm_tree_node(dp);

	//create the children
	this->nchildren = MAX_CHILDREN;
	for(int i = 0; i < MAX_CHILDREN; i++)
	{
		DEBUGMSG("DEBUG: tree.cpp: Gen child " << i  << "at depth " << depth);	
		this->children[i] = new tree(depth - 1, dp);
	}
}


tree::~tree()
{
	for(int i = 0; i < this->nchildren; i++)
	{
		delete this->children[i];
	}
}

tree_node *tree::gen_rand_nonterm_tree_node(darray *dp)
{
	tree_node *tp;

	/* initialize random seed: */
	srand ( clock() );

	/* generate secret number: */
	int type = rand() % NTYPES;	

	DEBUGMSG("DEBUG: tree.cpp: Generating rand node with type " << type);
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
		default:
			cout << "DEBUG: tree.cpp: No type for node, got type" << type << endl;
			exit(1);
		}
	
		return(tp);
}


tree_node *tree::gen_rand_term_tree_node(darray *dp)
{
	tree_node *tp;

	/* initialize random seed: */
	srand ( clock() );

	/* generate secret number: */
	int type = rand() % NTERMTYPES;	

	DEBUGMSG("DEBUG: tree.cpp: Generating rand term node with type " << type);
	switch (type)
	{
		case 0:
		{
			/* initialize random seed: */
			srand ( clock() );

			/* generate random double: */
			double d = ((double)rand()/(double)RAND_MAX);	

			tp = new tree_node(tree_node::tree_double, 1, d);
			break;
		}
		case 1:
		{
			tp = new tree_node(tree_node::tree_var, 1, dp);
			break;
		}
		default:
			cout << "DEBUG: tree.cpp: No term type for node, got type " << type << endl;
			exit(1);
		}
	
		return(tp);
}


double tree::eval()
{
	switch(this->tnp->get_ntype())
	{
		//nonterminals
		case tree_node::plus:
		{
			double sum = 0;
			for(int i = 0; i < this->nchildren; i++)
			{
				sum += this->children[i]->eval();
			}
			return(sum);
		}
		case tree_node::minus:
		{
			double sum = 0;
			for(int i = 0; i < this->nchildren; i++)
			{
				sum -= this->children[i]->eval();
			}
			return(sum);
		}
		case tree_node::multi:
		{
			double prod = 1;
			for(int i = 0; i < this->nchildren; i++)
			{
				prod *= this->children[i]->eval();
			}
			return(prod);
		}
		case tree_node::div:
		{
			double quot = 1;
			for(int i = 0; i < this->nchildren; i++)
			{
				//divide by zero safety
				if(this->children[i]->eval() == 0)
				{
					quot = 0;
				}
				else
				{
					quot /= this->children[i]->eval();
				}
			}
			return(quot);
		}
		
		//terminals
		case tree_node::tree_double:
		{
			return(this->tnp->get_dval());
		}
		case tree_node::tree_var:
		{
			return(this->tnp->get_ddp_val());
		}
		default:
		{
			cerr << "ERROR: No type for eval()\n";
			exit(1);
		}
	}
}


//set / change values in dp, and then run
double tree::fitness(double dexpected)
{
	return(abs(this->eval() - dexpected));
}


bool tree::is_term()
{
	if(this->nchildren <= 0)
	{
		return(true);
	}

	return(false);
}


bool tree::is_nonterm()
{
	if(this->nchildren <= 0)
	{
		return(false);
	}

	return(true);
}


int tree::count_terms()
{
	if(this->is_term() == true)
	{
		return(1);
	}

	int sum = 0;
	for(int i = 0; i < this->nchildren; i++)
	{
		sum += this->children[i]->count_terms();
	}
	
	return(sum);
}


int tree::count_nonterms()
{
	int sum = 0;

	if(this->is_nonterm() == true)
	{
		sum = 1;
	}

	for(int i = 0; i < this->nchildren; i++)
	{
		sum += this->children[i]->count_nonterms();
	}
	
	return(sum);
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
	cout << " = " << this->eval();
	//more debugging stuff
	//cout << ", term:nonterm == " << this->is_term() << ":" << this->is_nonterm();
	//cout << " nterm:nnonterm == " << this->count_terms() << ":" << this->count_nonterms();
	cout << endl;

	for(int i = 0; i <= this->nchildren; i++)
	{
		this->children[i]->print(depth + 1);
	}
	
	return(true);
}


bool tree::print_tnp_ntype()
{
	if(this == NULL)
	{
		return(false);
	}
	return(this->tnp->print_ntype());
}

///////////////////////////////////////////////////////////////////////////
//DArray
///////////////////////////////////////////////////////////////////////////

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


///////////////////////////////////////////////////////////////////////////////
//External tree functions
///////////////////////////////////////////////////////////////////////////////

bool mutate_nth_nonterm(tree **tp, int n, int depth, int new_depth, darray *dp)
{
	if( (*tp) == NULL)
	{
		return(false);
	}

	if( (*tp)->is_nonterm())
	{
		SUM_TEMP++;
	}
	cout << string(depth, ' ') << depth << ":";
	(*tp)->print_tnp_ntype();
	cout << " = " << SUM_TEMP;

	if(n == SUM_TEMP && (*tp)->is_nonterm())
	{
		cout << " !mutating!";
	
		//set this tree node to a new rand tree until it is a 
		// nonterminal
		do
		{
			delete (*tp);
			(*tp) = new tree(new_depth, dp);
		} while ((*tp)->is_nonterm() != true);

		cout << endl;
		return(true);
	}

	cout << endl;

	//if we've already see the node to mutate
	
	if(n < SUM_TEMP)
	{
		return(true);
	}
	
	for(int i = 0; i <= (*tp)->nchildren; i++)
	{
		mutate_nth_nonterm(&(*tp)->children[i], n, depth + 1, new_depth, 
							dp);
	}
	
	return(true);
}

