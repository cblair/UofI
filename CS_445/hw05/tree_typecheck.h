///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 6
////Author:       Colby Blair
////File name:    tree_symtab_gen.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _TREE_TYPECHECK_H
#define _TREE_TYPECHECK_H

char *tree_type_trans_lit(char*);

//Checkers
int tree_type_is_assignable(char*, char*);

//Getters
char *tree_get_leaf_type(struct tree*);

void tree_typecheck_class_constructor(struct tree*, char*);

void tree_typecheck_assignmentExpression(struct tree *);
void tree_typecheck_classDefinition(struct tree *);
void tree_typecheck_function_call(struct tree *);

#endif
