#include "sassy.h"
#include "error.h"

SYM_ENTRY *file_table = NULL;
SYM_ENTRY *sym_table = NULL;
int sym_cnt = 0;

/* search for the string in the symbol list; if not there,
 * add it; return a pointer to its struct;
 */
SYM_ENTRY *handle_ident (char *name)
    {
    SYM_ENTRY *x;
 
    for (x=sym_table; x!=NULL; x=x->link)
        if (strcmp (name, x->name) == 0)
            return x;
 
    SacMalloc (x, SYM_ENTRY*, sizeof (SYM_ENTRY))
    SacMalloc (x->name, char*, 1 + strlen (name))
    strcpy (x->name, name);
    x->val = sym_cnt++;
    x->link = sym_table;
    sym_table = x;
 
    return x;
    }

/* find symbol 'n' and return its string */
char *find_sym (int n)
    {
    SYM_ENTRY *x;
     
    for (x=sym_table; x!=NULL; x=x->link)
	if (x->val == n)
	    return x->name;

    return NULL;
    }

char *handle_filename (char *name)
    {
    SYM_ENTRY *x;
 
    for (x=file_table; x!=NULL; x=x->link)
        if (strcmp (name, x->name) == 0)
            return x->name;
 
    SacMalloc (x, SYM_ENTRY*, sizeof (SYM_ENTRY))
    SacMalloc (x->name, char*, 1 + strlen (name))
    strcpy (x->name, name);
    x->link = file_table;
    file_table = x;
 
    return x->name;
    }
