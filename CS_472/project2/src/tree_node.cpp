#include <iostream>
#include <stdarg.h>
#include <typeinfo>
#include <cstdlib>

#include "tree_node.h"
#include "tree.h"
#include "main.h"

using namespace std;

tree_node::tree_node(tree_node::node_type val, double dval, darray **dp)
{
	DEBUG_TREENODE_MSG("DEBUG: tree_node.cpp: Setting node type");
	
	//init members to default vals
	this->dval = 0;
	this->dp = NULL;
	this->ddp = NULL;

	switch (val)
	{
		case tree_node::plus:
		{
			this->ntype = val;
			DEBUG_TREENODE_MSG(" Node type == plus");
			break;
		}
		case tree_node::minus:
		{
			this->ntype = val;
			DEBUG_TREENODE_MSG(" Node type == minus");
			break;
		}
		case tree_node::multi:
		{
			this->ntype = val;
			DEBUG_TREENODE_MSG(" Node type == multi");
			break;
		}
		case tree_node::div:
		{
			this->ntype = val;
			DEBUG_TREENODE_MSG(" Node type == div");
			break;
		}
		case tree_node::tree_double:
		{
			this->ntype = val;
			//get the float val
			this->dval = dval;
			DEBUG_TREENODE_MSG(" Node type == tree_double");
			DEBUG_TREENODE_MSG(" Node val == " << this->dval);
			break;
		}
		case tree_node::tree_var:
		{
			DEBUG_TREENODE_MSG(" Node type == tree_var");
	
			this->ntype = val;
			//get the float val
			//get darray pointer from va_args
			//TODO: pass dp by reference instead

			/* initialize random seed: */
			srand ( clock() );

			//set dp to point to reference of passed in dp
			this->dp = (*dp);

			//set dpi
			/* generate secret number: */
			// select random element in dp
			this->dpi = rand() % this->dp->get_size();

			//set ddp to point to a random element of dp->a
			this->ddp = &this->dp->a[this->dpi];
			DEBUG_TREENODE_MSG(" Node val from rand index " << this->dpi << "== " << *this->ddp);
			break;
		}
		default:
			cerr << "ERROR: Node type not set, got val " \
				<< val << endl;
			exit(1);
	}		
}


bool tree_node::copy(tree_node** to)
{
	if(this == NULL)
	{
		(*to) = NULL;
		return(NULL);
	}

	switch (this->ntype)
	{
		case tree_node::tree_double:
		{
			(*to) = new tree_node(this->ntype, this->dval, NULL);
			return(true);
		}
		case tree_node::tree_var:
		{
			//TODO: not sure how stable this is exactly
			(*to) = new tree_node(this->ntype, 0.0, &this->dp);
			//TODO: set ddp to dp index
			(*to)->set_ddp(this->dpi);
			return(true);
		}
		default:
		{
			(*to) = new tree_node(this->ntype, 0.0, NULL);
			return(true);
		}
	}
}

double tree_node::get_dval()
{
	if(this == NULL)
	{	
		return(NULL);
	}

	return(this->dval);
}


double tree_node::get_ddp_val()
{
	if(this == NULL)
	{	
		return(NULL);
	}

	return(*this->ddp);
}


tree_node::node_type tree_node::get_ntype()
{
	if(this == NULL)
	{	
		return(tree_node::null);
	}

	return(this->ntype);
}


bool tree_node::set_ddp(int i)
{
	if(i >= this->dp->get_size())
	{
		return(false);
	}

	this->dpi = i;
	this->ddp = &this->dp->a[i];

	return(true);
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


bool tree_node::print_dval()
{
	cout << this->dval;
	return(true);
}


bool tree_node::print_ddp()
{
	if(this->ddp == NULL)
	{
		cout << "  : ";
	}
	else
	{
		cout << *this->ddp << " : ";
	}

	return(true);
}

bool tree_node::print_members()
{
	this->print_ntype();
	cout << " : ";
	this->print_dval();
	cout << " : ";
	this->print_ddp();
	cout << "\n";
}
