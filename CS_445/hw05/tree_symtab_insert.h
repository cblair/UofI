///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 6
////Author:       Colby Blair
////File name:    tree_symtab_insert.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _TREE_SYMTAB_INSERT_H
#define _TREE_SYMTAB_INSERT_H

int tree_symtab_insert_variableDefinition(struct tree *);
int tree_symtab_insert_memberDefinition(struct tree *);
int tree_symtab_insert_parameterDeclaration(struct tree *);

#endif
