///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 4
////Author:       Colby Blair
////File name:    tree_symtab_gen.h
////////////////////////////////////////////////////////////////////////////////

#include "tree.h"
#include "symtabc.h"
#include "tree_tac_gen.h"
#include "parser.tab.h"
#include "main.h"

#define MAX_BUF_SIZE 256
#define MAX_RESULT_STRING_SIZE  10

extern struct tac_inst_list *CODE;


int tree_tac_is_sub_assignmentExpression(struct tree *t) 
{
	if(t == NULL)
	{
		return(1);
	}

	if(strcmp(t->prodrule, "multiplicativeExpression") == 0)
	{
		//if this is a full expression
		struct tree *temp = NULL;
		tree_get_subtree("multiplicativeOperator", t, &temp);
		if(temp != NULL)
		{
			return(0);
		}
		else
		{
			return(1);
		}
	}

	if(strcmp(t->prodrule, "additiveExpression") == 0)
	{
		struct tree *temp = NULL;
		tree_get_subtree("additiveOperator", t, &temp);
		if(temp != NULL)
		{
			return(0);
		}
		else
		{
			return(1);
		}
	}

	return(1);
}


int tree_tac_gen_sub_assignmentExpression(struct tree **t, 
						int *result_count)
{
	if((*t) == NULL)
	{
		return(1);
	}

	//recurse down first
	int i;
	for(i = 0; i < (*t)->nkids; i++)
	{
		tree_tac_gen_sub_assignmentExpression(&(*t)->kids[i], 
							&(*result_count));
	}

	if(tree_tac_is_sub_assignmentExpression((*t)) == 0)
	{
		//get left operand
		char *o1 = tree_get_ident_or_val((*t)->kids[0]);
		//get right operand
		char *o2 = tree_get_ident_or_val((*t)->kids[2]);
		//get operator
		(*result_count)++;
		char *op = tree_get_operator((*t));
		char buffer[MAX_RESULT_STRING_SIZE];
		sprintf(buffer, "t%d", (*result_count));
		char *result = strdup(buffer);

		//Gen code
		tac_inst_list_append(TAC_CODE, result, op, o1, o2);
	
		//blow away processed node, and replace with reference
		// to resulting register; faked out as variable sub tree
		tree_del((*t));
		struct tree *temp1 = \
			tree_create_node_from_token(VAR, result, -1, "");
		struct tree *temp2 =
		 	tree_create_node("ident", 1, temp1);
		struct tree *temp3 = \
		 	tree_create_node("qualifiedIdent", 1, temp2);
		(*t) = tree_create_node("primaryExpression", 1, temp3);
	}

	return(0);
}


int tree_tac_gen_assignmentExpression(struct tree *t)
{
	//call our recursive gen func to gen code for the right hand side
	int result_count = 0; //used to determine which result to store to
	tree_tac_gen_sub_assignmentExpression(&t, &result_count);

	//set left hand ident to right hand side result
	char *ident = tree_get_ident(t);
	char buffer[MAX_RESULT_STRING_SIZE];
	sprintf(buffer, "t%d", result_count);
	char *result = strdup(buffer);

	tac_inst_list_append(TAC_CODE, ident, result, "", "");
	
	return(0); //success
}


void tree_tac_gen_sub_classDefinition(struct tree *t, char *classname)
{
	if(t == NULL)
	{
		return; //failure
	}

	//methodDefinition
	if(strcmp(t->prodrule, "methodDefinition") == 0)
	{
		tree_tac_gen_methodDefinition(t, classname);
	}

	//variableDefinition
	if(strcmp(t->prodrule, "variableDefinition") == 0)
	{
		tree_tac_gen_variableDefinition(t, classname);
	}

	int i;
	for(i = 0; i < t->nkids; i++)
	{
		tree_tac_gen_sub_classDefinition(t->kids[i], classname);
	}
}


int tree_tac_gen_classDefinition(struct tree *t)
{
	char *classname = tree_get_ident(t);
	tree_tac_gen_sub_classDefinition(t, classname);

	return(0); //success
}


int tree_tac_gen_methodDefinition(struct tree *t, char *classname)
{
	char *ident = tree_get_ident(t);
	char buffer[MAX_BUF_SIZE];
	sprintf(buffer, "%s.%s", classname, ident);
	char *fq_method_name = strdup(buffer); //fully qualified method name
	tac_inst_list_append(TAC_CODE, fq_method_name, "", "", "");

	//modify tree node in case it is a class methodDefinition, so 
	// it isn't picked up later as class-less methodDefinition.
	t->prodrule = "methodDefinitionProcessed";

	return(0); //success
}


int tree_tac_gen_variableDefinition(struct tree *t, char *classname)
{
	char *ident = tree_get_ident(t);
	char buffer[MAX_BUF_SIZE];
	sprintf(buffer, "%s.%s", classname, ident);
	char *fq_var_name = strdup(buffer); //fully qualified method name

	//modify tree node in case it is a class variableDefinition, so 
	// it isn't picked up later as class-less variableDefinition.
	t->prodrule = "variableDefinitionProcessed";

	//
	char *ident_or_val = tree_get_ident_or_val(t);
	if(ident_or_val == NULL)
	{
		ident_or_val = "";
	}
	tac_inst_list_append(TAC_CODE, fq_var_name, "", ident_or_val, "");

	treeprint(t, 0);

	return(0); //success
}
