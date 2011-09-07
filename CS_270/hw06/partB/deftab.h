#ifndef _DEFTAB_H
#define _DEFTAB_H

#include "sactypes.h"

typedef struct
    {
    Fstat stat;
    int nparams;	    /* the number of parameters */
    int *paramdvs;	    /* an array of param defvals */
    TypeInfo *paramtypes;   /* a linked list of param types */
    int nlocals;	    /* the number of local variables */
    int *locdvs;	    /* an array of local variable defvals */
    int nreturns;	    /* the number of return values */
    TypeInfo *rettypes;	    /* a linked list of return types */
    NODEPNT pragma;	    /* for prototype pragmas */
    } FUNCINFO;

typedef struct
    {
    TypeInfo tinfo;
    int symval;
    FUNCINFO *funcinfo;
    } DEFENTRY;

extern DEFENTRY *deftable;
extern int defcnt;
int newdef (Tstat, Type, int, int, Kind, int, int);
void show_deftable ();
char *kind_str (Kind);
char *type_str (Type);

#endif
