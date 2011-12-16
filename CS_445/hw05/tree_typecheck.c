///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 6
////Author:       Colby Blair
////File name:    tree_typecheck.c
////////////////////////////////////////////////////////////////////////////////

#include "tree.h"
#include "symtabc.h"
#include "tree_typecheck.h"
#include "parser.tab.h"
#include "main.h"

#define MAX_BUF_SIZE	256

#define POSTFIX_DELIM 	'.'
#define FS_DIR_DELIM 	'/' //linux!


char *tree_typecheck_classify(char *fname)
{
	if(fname == NULL)
	{
		return(NULL);
	}

	int i;
	for(i = 0; i < strlen(fname); i++)
	{
		//strip off directory delim and what's before it
		if(	fname[i] == FS_DIR_DELIM
			&& (i + 1) < strlen(fname) 
		)
		{
			fname = &fname[i+1];
			i = 0;
		}

		//blow away delim and everything after
		if(fname[i] == POSTFIX_DELIM)
		{
			fname[i] = '\0';
		}
	}

	return(fname);
}


//get's the type of the leaf; determines a literal type, or symtab lookups
// a ident's type
char *tree_get_leaf_type(struct tree *t)
{
	if(t == NULL)
	{
		return(NULL);
	}

	//if t is an ident, return its type, defined in symtab
	if(strcmp(t->prodrule, "ident") == 0)
	{
		SymTabEntry *p = SymTab_lookup(t->kids[0]->prodrule);
		if(p == NULL)
		{
			return(NULL);
		}
		return(p->type);
	}
	//else if we hit the leaf, t is a literal. return the translated
	// type?
	else if(t->leaf != NULL)
	{
		return(tree_type_trans_lit(t->prodrule));
	}

	int i;
	for(i = 0; i < t->nkids; i++)
	{
		char *retval = tree_get_leaf_type(t->kids[i]);
		if(retval != NULL)
		{
			return(retval);
		}
	}

	//didn't find a leaf? should never happen
	return(NULL);
}


//translates a literal val, always a char*, to a real AS3 type
char *tree_type_trans_lit(char *lval)
{
	if(lval == NULL)
	{
		return(NULL);
	}
	
	//String
	// if begins and ends with quotes
	if(
		(lval[0] == '"' && lval[strlen(lval) - 1] == '"')
		||
		(lval[0] == '\'' && lval[strlen(lval) - 1] == '\'')
	)
	{
		return("String");
	}
	//int
	else if(
		atoi(lval) != 0 //char * to non-zero int
		|| strcmp(lval, "0") == 0 //zero int
	)
	{
		return("int");
	}
	//TODO: Number, unint, Boolean, Object

	//don't know, some user defined type
	return("user");
}


//returns 0 for True, 1 for False
int tree_type_is_assignable(char *to, char *from)
{
	//if a type for one is not yet defined, its ok
	if(to == NULL || from == NULL)
	{
		return(0);
	}

	//int
	if(
		strcmp(to, "int") == 0 && strcmp(from, "int") == 0
	)
	{
		return(0);
	}
	//String
	else if(
		strcmp(to, "String") == 0 && strcmp(from, "String") == 0
	)
	{
		return(0);
	}
	//Boolean
	else if(
		strcmp(to, "Boolean") == 0 && strcmp(from, "Boolean") == 0
	)
	{
		return(0);
	}
	//anything from a function
	else if( strcmp(from, "function") == 0)
	{
		return(0);
	}

	return(1);
}


void tree_typecheck_packageDecl(struct tree *t)
{
	//Class == file name check

	//get the class subtree
	struct tree *class_tree = NULL;
	tree_get_subtree("classDefinition", t, &class_tree);

	//classify fname
	if(class_tree == NULL)
	{
		fprintf(stderr, 
			"ERROR: internal class name check. Exiting.\n");
		exit(ERROR_INTERNAL);
	}

	//get the class name
	char *class_name = tree_get_ident(class_tree);

	//get the file name
	// packageDecl->"package"->fname
	char *fname = t->kids[0]->leaf->fname;
	char *fname_classified = tree_typecheck_classify(fname);

	//classify fname
	if(class_name == NULL
		|| fname_classified == NULL)
	{
		fprintf(stderr, 
			"ERROR: internal class name check. Exiting.\n");
		exit(ERROR_INTERNAL);
	}

	//check
	if(strcmp(class_name, fname_classified) != 0)
	{
		fprintf(stderr,
			"ERROR: class name '%s' doesn't match filename '%s'. Exiting.\n",
			class_name, fname_classified);
		exit(ERROR_SEMANTIC);
	}
}


void tree_typecheck_class_constructor(struct tree *t, char *class_name)
{
	if(t == NULL)
	{
		return;
	}
	
	//construct test
	if(tree_is_class_constructor(t, class_name) == 0)
	{
		struct tree *return_tree = NULL;
		tree_get_subtree("returnStatement", t, &return_tree);

		//constructor result check
		if(return_tree != NULL)
		{
			fprintf(stderr,
				"ERROR: constructor cannot have a return value. Exiting.\n");
			exit(ERROR_SEMANTIC);
		}
	}

	int i;
	for(i = 0; i < t->nkids; i++)
	{
		tree_typecheck_class_constructor(t->kids[i], class_name);
	}
}


void tree_typecheck_assignmentExpression(struct tree *t)
{
	//Expression type check
	//see if assignment op is in tree
	struct tree *subtree = NULL;
	tree_get_subtree("=",t, &subtree);
	//if no 'op' branch, this is a right hand assignmentExpression with a
	// literal, which we'll ignore now and handle when we see the entire 
	//expression later (below)
	if(subtree == NULL)
	{
		return;
	}
	//else, this is an entire assignmentExpression to a identifier
	else 
	{	
		//get the token name
		char *tname = tree_get_ident(t);		

		SymTabEntry *p = SymTab_lookup(tname);
	
		//get the subtree symbol for report info
		struct tree *temp = NULL;
		tree_get_subtree(tname, t, &temp);
	
		//Use before declaration check
		if(p == NULL)
		{
			if(temp != NULL)
			{
				struct tree_token *symbol = temp->leaf;
		 		fprintf(stderr,
		  	 	"ERROR: identifier '%s' used before declared, in file '%s' on line %d\n", 
					tname, symbol->fname, symbol->lineno);
			}
			else
			{
				fprintf(stderr,
		  	 	"ERROR: identifier '%s' used before declared, and some horrible internal errors also occured.\n",
					tname); 
			}
		 
			exit(ERROR_SEMANTIC);
		}

		//Assignment to const check
		if(p->aux_flag != NULL 
			&& strcmp(p->aux_flag, "const") == 0)
		{
			if(temp != NULL)
		 	{
				struct tree_token *symbol = temp->leaf;
				fprintf(stderr,
				 "ERROR: assignment to constant '%s', in file '%s' on line %d\n", 
			 	tname, symbol->fname, symbol->lineno);
	
			}
			else
			{
				fprintf(stderr,
				 "ERROR: assignment to constant '%s', and some horrible internal errors also occured.\n",
				 tname);
			}

			exit(ERROR_SEMANTIC);
		}
	} //end else (if subtree != NULL)

	//Type assignment check
	//get left and right branches of tree
	struct tree *ltree, *rtree;
	ltree = t->kids[0]; //left hand side
	rtree = t->kids[2]; //right hand side

	//get ident on left branch
	struct tree *itree = NULL; //ident tree
	tree_get_subtree("ident", ltree, &itree);

	//get value on right value
	struct tree *vtree = NULL; //value tree
	tree_get_subtree("primaryExpression", rtree, &vtree);

	//get branch types	
	char *itype, *vtype;
	itype = tree_get_leaf_type(itree);
	vtype = tree_get_leaf_type(vtree);

	if(tree_type_is_assignable(itype, vtype) != 0)
	{
		fprintf(stderr, "ERROR: type '%s' and '%s' are not assignable, in file '' on line ''. Exiting...\n", itype, vtype);
		exit(ERROR_SEMANTIC);
	}
}


void tree_typecheck_classDefinition(struct tree *t)
{
	char *class_name = tree_get_ident(t);

	if(class_name == NULL)
	{
		fprintf(stderr,
			"ERROR: internal class definition. Exiting.\n");
		exit(ERROR_INTERNAL);
	}

	tree_typecheck_class_constructor(t, class_name);
}


void tree_typecheck_get_argument_type_list(struct tree *t, int *len, 
					char *arg_type_list[])
{
	if(t == NULL)
	{
		return;
	}

	if(strcmp(t->prodrule, "assignmentExpression") == 0)
	{
		char *type = NULL;
		char *ident_or_val = tree_get_ident_or_val(t);
		
		//see if the expression is an ident
		SymTab_lookup_type(ident_or_val, &type);

		//if it isn't, it is a literal, so look up its type
		if(type == NULL)
		{
			type = tree_type_trans_lit(ident_or_val);
		}

		arg_type_list[(*len)] = type;
		(*len)++;
	}

	int i;
	for(i = 0; i < t->nkids; i++)
	{
		tree_typecheck_get_argument_type_list(t->kids[i], 
						&(*len), arg_type_list);
	}
}


void tree_typecheck_get_parameter_type_list(struct tree *t, int *len, 
					char *param_type_list[])
{
	if(t == NULL)
	{
		return;
	}

	if(strcmp(t->prodrule, "parameterDeclaration") == 0)
	{
		char *type = "void";
		
		struct tree *type_tree = NULL;
		tree_get_subtree("optionalTypeExpression", t, &type_tree);

		//if there is an optional type expression, then we know the
		// type
		if(type_tree != NULL)
		{
			type = tree_get_ident(type_tree);
		}
		//else, default is void
		
		param_type_list[(*len)] = type;
		(*len)++;
	}

	int i;
	for(i = 0; i < t->nkids; i++)
	{
		tree_typecheck_get_parameter_type_list(t->kids[i], 
						&(*len), param_type_list);
	}
}


void tree_typecheck_arguments(struct tree *t)
{
	//Get the function name
	char *func_name = tree_get_ident(t);
	if(func_name == NULL)
	{
		fprintf(stderr,
			"ERROR: internal function call. Exiting");
		exit(ERROR_INTERNAL);
	}

	//Get the line number for error reporting
	struct tree *temp = NULL;
	tree_get_subtree(func_name, t, &temp);
	int lineno = temp->leaf->lineno;

	//Get the function call argument tree
	struct tree *arg_tree = NULL;
	tree_get_subtree("arguments", t, &arg_tree);
	if(arg_tree == NULL)
	{
		fprintf(stderr,
			"ERROR: internal function call. Exiting");
		exit(ERROR_INTERNAL);
	}

	//Get the method definition tree from the symbol table
	SymTabEntry *p = lookupSymTabEntry(func_name);
	if(p == NULL)
	{
		fprintf(stderr,
			"ERROR: internal function call. Exiting");
		exit(ERROR_INTERNAL);
	}

	//Get the parameter tree from the methodDefinition tree
	struct tree *param_tree = NULL;
	tree_get_subtree("parameterDeclarationList", p->def_t, &param_tree);
	if(param_tree == NULL)
	{
		fprintf(stderr,
			"ERROR: internal function call. Exiting");
		exit(ERROR_INTERNAL);
	}

	//Get the argument type list
	char *arg_type_list[MAX_BUF_SIZE];
	int arg_len = 0;
	tree_typecheck_get_argument_type_list(arg_tree, 
						&arg_len, arg_type_list);

	//Get the parameter type list
	char *param_type_list[MAX_BUF_SIZE];
	int param_len = 0;
	tree_typecheck_get_parameter_type_list(param_tree, &param_len, 
						param_type_list);

	//Check... finally!
	
	//first, should have same number of args as params
	if(arg_len != param_len)
	{
		fprintf(stderr,
			"ERROR: function call '%s' has %d parameters, but was called with %d on line %d. Exiting.\n",
			func_name, param_len, arg_len, lineno);
		exit(ERROR_SEMANTIC);
	}

	//check if they are all the same type, ignoring any void
	// definitions
	int i;
	for(i = 0; i < param_len; i++)
	{
		if(strcmp(param_type_list[i], "void") != 0)
		{
			if(strcmp(param_type_list[i], arg_type_list[i]) != \
				0)
			{
				fprintf(stderr,
					"ERROR: mismatched argument type forargument %d, function call '%s' on \nline %d; type should be '%s' but is called as '%s'. Exiting.\n",
					i, func_name, lineno, 
					param_type_list[i], 
					arg_type_list[i]);
			}
		}
	}
}


void tree_typecheck_function_call(struct tree *t)
{
	if(t == NULL)
	{
		fprintf(stderr,
			"ERROR: internal function call. Exiting");
		exit(ERROR_INTERNAL);
	}

	tree_typecheck_arguments(t);
}
