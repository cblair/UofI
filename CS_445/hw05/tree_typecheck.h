///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 4
////Author:       Colby Blair
////File name:    tree_symtab_gen.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _TREE_TYPECHECK_H
#define _TREE_TYPECHECK_H

char *tree_type_trans_lit(char*);
int tree_type_is_assignable(char*, char*);

int tree_typecheck_assignmentExpression(struct tree *);

#endif
