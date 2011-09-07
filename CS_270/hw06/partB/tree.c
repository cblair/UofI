#include "sassy.h"
#include "node.h"
#include "error.h"

void free_typeinfo_list (TypeInfo*);
TypeInfo *copy_typeinfo (TypeInfo*);
TypeInfo *copy_types_list (TypeInfo*);

NODEPNT make_node (int type, NODEPNT ch0, NODEPNT ch1, NODEPNT ch2,
	NODEPNT ch3, NODEPNT ch4, NODEPNT lnk, int lineno, char *file, char *func)
    {
    NODEPNT ret;

    MALLOC_NODE (ret, type)
    ret->Lineno = lineno;
    ret->Srcfile = file;
    ret->Srcfunc = func;
    ret->child[0] = ch0;
    ret->child[1] = ch1;
    ret->child[2] = ch2;
    ret->child[3] = ch3;
    ret->child[4] = ch4;
    ret->link = lnk;
    return ret;
    }

void free_tree (NODEPNT n)
    {
    int i;

    for (i=0; i<NUM_CHILDREN; i++)
        if (n->child[i] != NULL)
	    free_tree (n->child[i]);

    if (n->link != NULL)
        free_tree (n->link);

    free_typeinfo_list (n->types);

    bzero (n, sizeof (NODE));
    SacFree (n)
    }

void free_typeinfo_list (TypeInfo *t)
    {
    if (t == NULL)
	return;

    free_typeinfo_list (t->link);
    SacFree (t)
    }

int list_length (NODEPNT n)
    {
    int cnt;

    for (cnt=0; n!=NULL; cnt++, n=n->link);
    return cnt;
    }

NODEPNT copy_node (NODEPNT n)
    {
    NODEPNT ret;

    ret = make_node (n->nodetype, NULL, NULL,
    		NULL, NULL, NULL, NULL, n->Lineno, n->Srcfile, n->Srcfunc);

    switch (n->nodetype)
	{
	case BOOLTYPE :
	case UINTTYPE :
	case INTTYPE :
	case FLOATTYPE :
	case DOUBLETYPE :
	case CXINTTYPE :
	case CXFLOATTYPE :
	case CXDOUBLETYPE :
	    ret->Totsize = n->Totsize;
	    break;
	case UFIXTYPE :
	case FIXTYPE :
	case CXFIXTYPE :
	    ret->Totsize = n->Totsize;
	    ret->Fracsize = n->Fracsize;
	    break;
	case STRING :
	case INTNUM :
	case FLOATNUM :
	    SacMalloc (ret->Strval, char*, strlen (n->Strval) + 1)
	    strcpy (ret->Strval, n->Strval);
	    break;
	case IDENT :
	    ret->Symval = n->Symval;
	    ret->Defval = n->Defval;
	    break;
	default :
            break;
	}

    ret->types = copy_types_list (n->types);
    return ret;
    }

NODEPNT copy_tree (NODEPNT n)
    {
    NODEPNT ret;
    int i;

    ret = copy_node (n);

    for (i=0; i<NUM_CHILDREN; i++)
        if (n->child[i] != NULL)
	    ret->child[i] = copy_tree (n->child[i]);

    if (n->link != NULL)
        ret->link = copy_tree (n->link);

    return ret;
    }

TypeInfo *copy_types_list (TypeInfo *t)
    {
    TypeInfo *r=NULL, **p;

    p = &r;
    for (; t!=NULL; t=t->link)
        {
        *p = copy_typeinfo (t);
        p = &((*p)->link);
        }
    return r;
    }


TypeInfo *copy_typeinfo (TypeInfo *t)
    {
    TypeInfo *r;

    SacMalloc (r, TypeInfo*, sizeof (TypeInfo))
    r->stat = t->stat;
    r->type = t->type;
    r->totsize = t->totsize;
    r->fracsize = t->fracsize;
    r->kind = t->kind;
    r->dims = t->dims;
    r->link = NULL;

    return r;
    }
