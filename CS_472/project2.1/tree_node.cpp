#include <iostream>
#include <stdarg.h>
#include <typeinfo>

#include "tree_node.h"
#include "tree.h"
#include "main.h"

using namespace std;

/*
	Sets the node's type. Pretty redundant, but would be 
	useful if operators ever took more than two parameters.
*/
tree_node::tree_node(tree_node::node_type val, int n_args, ...)
{
	DEBUGMSG << "DEBUG: tree_node.cpp: Setting node type\n";
	switch (val)
	{
		case tree_node::plus:
		{
			this->ntype = val;
			DEBUGMSG << " Node type == plus\n";
			break;
		}
		case tree_node::minus:
		{
			this->ntype = val;
			DEBUGMSG << " Node type == minus\n";
			break;
		}
		case tree_node::multi:
		{
			this->ntype = val;
			DEBUGMSG << " Node type == multi\n";
			break;
		}
		case tree_node::div:
		{
			this->ntype = val;
			DEBUGMSG << " Node type == div\n";
			break;
		}
		case tree_node::tree_double:
		{
			this->ntype = val;
			//get the float val
			// start vargs
			register int i;
        		va_list ap;
			va_start(ap, n_args);
			this->dval = va_arg(ap, double);
			//this->ival = va_arg(ap, int);
			va_end(ap);
			DEBUGMSG << " Node type == tree_double\n";
			DEBUGMSG << " Node val == " << this->dval << endl;
			break;
		}
		case tree_node::tree_var:
		{
			DEBUGMSG << " Node type == tree_var\n";
	
			this->ntype = val;
			//get the float val
			// start vargs
			register int i;
        		va_list ap;
			va_start(ap, n_args);
			//get darray pointer from va_args
			darray *dp = va_arg(ap, darray*);
			va_end(ap);

			/* initialize random seed: */
			srand ( clock() );

			/* generate secret number: */
			// select random element in dp
			int j = rand() % dp->get_size() + 1;
			//set ddp to point to a random element of dp->a
			this->ddp = &dp->a[j];
			DEBUGMSG << " Node val == " << *this->ddp << endl;
			break;
		}
		default:
			DEBUGMSG << " Node type not set\n";
			exit(1);
	}		
}

bool tree_node::print_ntype()
{
	if(this == NULL)
	{
		cout << "(!null!)";
	}
	else
	{
		switch (this->ntype)
		{
			case tree_node::plus:
			{
				cout << "plus";
				break;
			}
			case tree_node::minus:
			{
				cout << "minus";
				break;
			}
			case tree_node::multi:
			{
				cout << "multi";
				break;
			}
			case tree_node::div:
			{
				cout << "div";
				break;
			}
			case tree_node::tree_double:
			{
				cout << "tree_double";
				break;
			}
			case tree_node::tree_var:
			{
				cout << "tree_var";
				break;
			}
		} //end switch
	}
}
