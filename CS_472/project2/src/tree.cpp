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
	this->depth = depth;
	
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
	//cout << "DEBUG: tree.cpp: rand_val = " << rand_val << endl;
	// 1 out of 10 rand nodes get set to terminal 
	bool rand_term = (rand_val == 0);
	if(depth <= 0 ) //|| rand_term == true)
	{
		this->gen_rand_term_tree_node(dp);
		return;
	}

	//Nonterminal
	this->gen_rand_nonterm_tree_node(dp);

	//create the children
	this->nchildren = MAX_CHILDREN;
	for(int i = 0; i < MAX_CHILDREN; i++)
	{
		DEBUG_TREE_MSG("DEBUG: tree.cpp: Gen child " << i  << "at depth " << depth);	
		this->children[i] = new tree(depth - 1, dp);
	}
}


tree::~tree()
{
	for(int i = 0; i < this->nchildren; i++)
	{
		delete this->children[i];
	}
	
	delete this->tnp;
}


bool tree::copy(tree** to)
{
	//DEBUG_TREE_MSG("DEBUG: tree.cpp:");

	//init the 'to' tree with 'this's depth
	//(*to) = new tree(this->depth, this->dp);
	delete (*to);
	(*to) = (tree*)malloc(sizeof(class tree));
	(*to)->depth = this->depth;
	

	//copy tnp
	this->tnp->copy(&(*to)->tnp);

	//copy dp
	this->dp->copy(&(*to)->dp);

	//copy nchildren
	(*to)->nchildren = this->nchildren;
	
	//copy children
	for(int i = 0; i < this->nchildren; i++)
	{
		this->children[i]->copy(&(*to)->children[i]);
	}
	return(false);
}

tree_node *tree::gen_rand_nonterm_tree_node(darray *dp)
{
	/* initialize random seed: */
	srand ( clock() );

	/* generate secret number: */
	int type = rand() % NTYPES;	

	DEBUG_TREE_MSG("DEBUG: tree.cpp: Generating rand node with type " << type);
	switch (type)
	{
		case 0:
		{
			this->tnp = new tree_node(tree_node::plus, 0.0, NULL);
			break;
		}	
		case 1:
		{
			this->tnp = new tree_node(tree_node::minus, 0.0, NULL);
			break;
		}
		case 2:
		{
			this->tnp = new tree_node(tree_node::multi, 0.0, NULL);
			break;
		}
		case 3:
		{
			this->tnp = new tree_node(tree_node::div, 0.0, NULL);
			break;
		}
		default:
			cout << "ERROR: tree.cpp: No type for node, got type" << type << endl;
			exit(1);
		}
}


tree_node *tree::gen_rand_term_tree_node(darray *dp)
{
	/* initialize random seed: */
	srand ( clock() );

	/* generate secret number: */
	int type = rand() % NTERMTYPES;	

	DEBUG_TREE_MSG("DEBUG: tree.cpp: Generating rand term node with type " << type);
	switch (type)
	{
		case 0:
		{
			/* initialize random seed: */
			srand ( clock() );

			/* generate random double: */
			double d = ((double)rand()/(double)RAND_MAX);	

			this->tnp = new tree_node(tree_node::tree_double, d, NULL);
			break;
		}
		case 1:
		{
			this->tnp = new tree_node(tree_node::tree_var, 0.0, &dp);
			break;
		}
		default:
			cout << "ERROR: tree.cpp: No term type for node, got type " << type << endl;
			exit(1);
		}
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
			double sum = this->children[0]->eval();
			for(int i = 1; i < this->nchildren; i++)
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


tree *tree::get_nth_nonterm_subtree(int n)
{
	if(this == NULL || this->is_term())
	{
		//false if I am a child that didn't get a value
		return(NULL);
	}
	else if(this->is_nonterm())
	{
		SUM_TEMP++;
		if(SUM_TEMP >= n)
		{
			DEBUG_TREE_MSG("returning " << n " subtree node\n");
			return(this);
		}
		//do stuff here

		for(int i = 0; i < this->nchildren; i++)
		{
			tree *result = \
			 this->children[i]->get_nth_nonterm_subtree(n);

			if(result != NULL)
			{
				return(result);
			}
		}
	}
	
	DEBUG_TREE_MSG("didn't find " << n << " subtree node. ? SUM_TEMP = " << SUM_TEMP << "\n");
	return(NULL);
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
	cout << ", children = " << this->nchildren;
	cout << endl;

	for(int i = 0; i < this->nchildren; i++)
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


///////////////////////////////////////////////////////////////////////////////
//External tree functions
///////////////////////////////////////////////////////////////////////////////

bool tree_crossover(tree **tp1, tree **tp2)
{
	int rand_val;

	//Get subtrees
	// get tp1 subtree
	/* initialize random seed: */
	srand ( clock() );
	/* generate secret number: */
	rand_val = rand() % (*tp1)->count_nonterms(); //0-n values
	tree *tp1_sub = (*tp1)->get_nth_nonterm_subtree(rand_val);
	tree *tp1_sub_orig; //the original subtree for tp2 crossover
	tp1_sub->copy(&tp1_sub_orig);

	//get tp2 subtree
	/* initialize random seed: */
	srand ( clock() );
	/* generate secret number: */
	rand_val = rand() % (*tp2)->count_nonterms(); //0-n values
	tree *tp2_sub = (*tp2)->get_nth_nonterm_subtree(rand_val);

	//Replace tp1 rand subtree with rand tp2 subtree
	delete tp1_sub; tp1_sub = NULL;
	//TODO: this is where tree_gp->gen() seg faults, ?
	tp2_sub->copy(&tp1_sub);

	//Replace tp2 rand subtree with original rand tp1 subtree
	delete tp2_sub;
	tp1_sub_orig->copy(&tp2_sub);

	return(true);
}


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

	#ifdef DEBUG_TREE
	cout << string(depth, ' ') << depth << ":";
	(*tp)->print_tnp_ntype();
	cout << " = " << SUM_TEMP;
	#endif

	if(n == SUM_TEMP && (*tp)->is_nonterm())
	{
		#ifdef DEBUG_TREE
		cout << " !mutating!";
		#endif

		//set this tree node to a new rand tree until it is a 
		// nonterminal
		do
		{
			delete (*tp);
			(*tp) = new tree(new_depth, dp);
		} while ((*tp)->is_nonterm() != true);

		#ifdef DEBUG_TREE
		cout << endl;
		#endif

		return(true);
	}

	#ifdef DEBUG_TREE
	cout << endl;
	#endif

	//if we've already see the node to mutate
	
	if(n < SUM_TEMP)
	{
		return(true);
	}
	
	for(int i = 0; i < (*tp)->nchildren; i++)
	{
		mutate_nth_nonterm(&(*tp)->children[i], n, depth + 1, new_depth, 					dp);
	}
	
	return(true);
}


bool tree_replace_nth_nonterm(tree **tp, tree **with, int n)
{
	if((*tp) == NULL)
	{
		return(false);
	}

	if((*tp)->is_nonterm())
	{
		SUM_TEMP++;

		if(n == SUM_TEMP)
		{
			//copy
			delete (*tp);
			(*with)->copy(&(*tp));
			return(true);
		}
		else
		{
			for(int i = 0; i < (*tp)->nchildren; i++)
			{
				bool status = \
				 tree_replace_nth_nonterm(
							&(*tp)->children[i],
							&(*with), 
							n
							);
				if(status == true) {return(true);}
			}
		}
	}

	return(false);
}
