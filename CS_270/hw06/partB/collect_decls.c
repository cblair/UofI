#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

int *dvs = NULL;
int dvidx = 0;
int dvs_size = 0;

void search_decls (NODEPNT);
void push_dv (int);

void collect_decls (NODEPNT root)
    {
    NODEPNT n;
    int dv, i;

    SacMalloc (dvs, int*, 10 * sizeof (int))
    dvs_size = 10;

    for (n=root; n!=NULL; n=n->link)
	{
	dvidx = 0;

	search_decls (n->child[3]);

	dv = n->child[1]->Defval;
	deftable[dv].funcinfo->nlocals = dvidx;
	SacMalloc (deftable[dv].funcinfo->locdvs, int*, dvidx * sizeof (int))

	for (i=0; i<dvidx; i++)
	    deftable[dv].funcinfo->locdvs[i] = dvs[i];
	}

    SacFree (dvs)
    }

void search_decls (NODEPNT n)
    {
    NODEPNT r;
    int i;

    switch (n->nodetype)
	{
	case ASSIGN :
	    for (r=n->child[0]; r!=NULL; r=r->link)
		if ((r->nodetype == IDENT) && (r->child[0]->nodetype != NEXT))
		    push_dv (r->Defval);
	    
	    if (n->child[1] != NULL)
		search_decls (n->child[1]);

	    if (n->link != NULL)
		search_decls (n->link);
	    break;
	case WINDOW :
	    push_dv (n->child[0]->Defval);

	    search_decls (n->child[0]->child[1]);

	    search_decls (n->child[1]);

	    if (n->child[2] != NULL)
	        search_decls (n->child[2]);

	    for (r=n->child[3]; r!=NULL; r=r->link)
		if (r->nodetype == IDENT)
		    push_dv (r->Defval);

	    if (n->link != NULL)
		search_decls (n->link);
	    break;
	case ELE_GEN :
	case SLICE_GEN :
	    push_dv (n->child[0]->Defval);

	    search_decls (n->child[2]);

	    if (n->child[3] != NULL)
	        search_decls (n->child[3]);

	    for (r=n->child[4]; r!=NULL; r=r->link)
		if (r->nodetype == IDENT)
		    push_dv (r->Defval);

	    if (n->link != NULL)
		search_decls (n->link);
	    break;
	case SCALAR_GEN :
	    for (r=n->child[0]; r!=NULL; r=r->link)
		if (r->nodetype == IDENT)
		    push_dv (r->Defval);

	    if (n->link != NULL)
		search_decls (n->link);
	    break;
	default :
	    for (i=0; i<NUM_CHILDREN; i++)
		if (n->child[i] != NULL)
		    search_decls (n->child[i]);

	    if (n->link != NULL)
		search_decls (n->link);
	}
    }

void push_dv (int dv)
    {
    int *tmp;
    int cnt;

    if (dvidx >= dvs_size)
        {
        /* ran out of space, so need to allocate bigger and copy */
	tmp = dvs;
        cnt = dvs_size;
        SacMalloc (dvs, int*, 2 * cnt * sizeof (int))
	bcopy (tmp, dvs, cnt * sizeof (int));

        dvs_size = 2 * cnt;
        SacFree (tmp)
	}

    dvs[dvidx++] = dv;
    }
