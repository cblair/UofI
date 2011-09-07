#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

int is_int_const (NODEPNT);
int get_c_val (NODEPNT);

void const_fold_tree (NODEPNT n)
    {
    int v0, v1, i;

    for (i=0; i<NUM_CHILDREN; i++)
        if (n->child[i] != NULL)
	    const_fold_tree (n->child[i]);

    if (n->link != NULL)
        const_fold_tree (n->link);

    switch (n->nodetype)
        {
	case ADD :
	    if (is_int_const (n->child[0]) && is_int_const (n->child[1]))
	        {
		v0 = get_c_val (n->child[0]);
		v1 = get_c_val (n->child[1]);
		free_tree (n->child[0]);
		free_tree (n->child[1]);
		n->child[0] = NULL;
		n->child[1] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", v0+v1);
		}
	    break;
	case SUB :
	    if (is_int_const (n->child[0]) && is_int_const (n->child[1]))
	        {
		v0 = get_c_val (n->child[0]);
		v1 = get_c_val (n->child[1]);
		free_tree (n->child[0]);
		free_tree (n->child[1]);
		n->child[0] = NULL;
		n->child[1] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", v0-v1);
		}
	    break;
	case MUL :
	    if (is_int_const (n->child[0]) && is_int_const (n->child[1]))
	        {
		v0 = get_c_val (n->child[0]);
		v1 = get_c_val (n->child[1]);
		free_tree (n->child[0]);
		free_tree (n->child[1]);
		n->child[0] = NULL;
		n->child[1] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", v0*v1);
		}
	    break;
	case DIV :
	    if (is_int_const (n->child[0]) && is_int_const (n->child[1]))
	        {
		v0 = get_c_val (n->child[0]);
		v1 = get_c_val (n->child[1]);
		free_tree (n->child[0]);
		free_tree (n->child[1]);
		n->child[0] = NULL;
		n->child[1] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", v0/v1);
		}
	    break;
	case MOD :
	    if (is_int_const (n->child[0]) && is_int_const (n->child[1]))
	        {
		v0 = get_c_val (n->child[0]);
		v1 = get_c_val (n->child[1]);
		free_tree (n->child[0]);
		free_tree (n->child[1]);
		n->child[0] = NULL;
		n->child[1] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", v0%v1);
		}
	    break;
	case NEG :
	    if (is_int_const (n->child[0]))
	        {
		v0 = get_c_val (n->child[0]);
		free_tree (n->child[0]);
		n->child[0] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", -v0);
		}
	    break;
	case LEFT_SHIFT :
	    if (is_int_const (n->child[0]) && is_int_const (n->child[1]))
	        {
		v0 = get_c_val (n->child[0]);
		v1 = get_c_val (n->child[1]);
		free_tree (n->child[0]);
		free_tree (n->child[1]);
		n->child[0] = NULL;
		n->child[1] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", v0<<v1);
		}
	    break;
	case RIGHT_SHIFT :
	    if (is_int_const (n->child[0]) && is_int_const (n->child[1]))
	        {
		v0 = get_c_val (n->child[0]);
		v1 = get_c_val (n->child[1]);
		free_tree (n->child[0]);
		free_tree (n->child[1]);
		n->child[0] = NULL;
		n->child[1] = NULL;
		n->nodetype = INTNUM;
		SacMalloc (n->Strval, char*, 128)
		sprintf (n->Strval, "%d", v0>>v1);
		}
	    break;
	default :
            break;
	}
    }

int is_int_const (NODEPNT n) {
    return ((n->nodetype==ICONST) || (n->nodetype==INTNUM));
    }

int get_c_val (NODEPNT n) {
    int val;

    if (n->nodetype == ICONST)
        return n->Iconst;
    else
	{
        sscanf (n->Strval, "%d", &val);
	return (val);
	}
    }
