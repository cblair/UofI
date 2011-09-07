#include "sassy.h"
#include "error.h"

void add_to_list (int dv, IntList **list)
    {
    IntList *tmp;

    SacMalloc (tmp, IntList*, sizeof (IntList))
    tmp->val = dv;
    tmp->link = *list;
    *list = tmp;
    }

void append_intlist (IntList **list, int v)
    {
    IntList **p, *tmp;

    for (p=list; (*p)!=NULL; p=&((*p)->link));
    SacMalloc (tmp, IntList*, sizeof (IntList))
    tmp->val = v;
    tmp->link = NULL;
    *p = tmp;
    }

int in_list (int dv, IntList **list)
    {
    IntList *tmp;

    for (tmp=*list; tmp!=NULL; tmp=tmp->link)
        if (tmp->val == dv)
	    {
	    return TRUE;
	    }

    return FALSE;
    }

int length_intlist (IntList **list)
    {
    IntList *tmp;
    int cnt;
    for (tmp=*list, cnt=0; tmp!=NULL; tmp=tmp->link, cnt++);
    return cnt;
    }

void free_intlist (IntList **list)
    {
    IntList *tmp;

    while (*list != NULL)
        {
	tmp = *list;
	*list = tmp->link;
	SacFree (tmp)
	}
    }

void show_intlist (IntList **list, FILE *fp)
    {
    IntList *tmp;

    for (tmp=*list; tmp!=NULL; tmp=tmp->link)
        fprintf (fp, " %d", tmp->val);
    }

void reverse_intlist (IntList **list)
    {
    IntList *m, *tmp, *n = *list;

    tmp = NULL;

    while (n != NULL)
        {
        m = n->link;
        n->link = tmp;
        tmp = n;
        n = m;
        }

    *list = tmp;
    }

void delete_ele_from_intlist (int val, IntList **list)
    {
    IntList **p, *it;

    for (p=list; (*p)!=NULL; )
        {
	it = *p;

	if (it->val == val)
	    {
	    *p = it->link;
	    SacFree (it)
	    }
	else
	    p=&((*p)->link);
	}
    }

void replace_in_intlist (IntList **list, int old_val, int new_val)
    {
    IntList *it;

    for (it=*list; it!=NULL; it=it->link)
        if (it->val == old_val)
	    {
	    it->val = new_val;
	    return;
	    }

    assert (0);
    }

void copy_intlist (IntList **tgt, IntList **src)
    {
    IntList *it;

    for (it=*src; it!=NULL; it=it->link)
        {
	SacMalloc (*tgt, IntList*, sizeof (IntList))
	(*tgt)->val = it->val;
	tgt = &((*tgt)->link);
	}

    *tgt = NULL;
    }

void replace_val_with_list (int val, IntList **list, IntList **seg)
    {
    IntList *tmp, **pit;
    
    for (pit=list; (*pit)!=NULL; pit=&((*pit)->link))
	{
        if ((*pit)->val == val)
	    {
	    tmp = *pit;
	    break;
	    }
	}

    assert ((*pit) != NULL);

    *pit = *seg;

    for ( ; (*pit)!=NULL; pit=&((*pit)->link));

    *pit = tmp->link;
    SacFree (tmp)
    }

void insert_val_after_in_intlist (int newval, int srchval, IntList **list)
    {
    IntList *tmp, *it;

    for (it=*list; it!=NULL; it=it->link)
        if (it->val == srchval)
	    break;

    assert (it != NULL);

    SacMalloc (tmp, IntList*, sizeof (IntList))
    tmp->val = newval;
    tmp->link = it->link;
    it->link = tmp;
    }

void insert_val_before_in_intlist (int newval, int srchval, IntList **list)
    {
    IntList *tmp, **pit;

    for (pit=list; (*pit)!=NULL; pit=&((*pit)->link))
        if ((*pit)->val == srchval)
	    break;

    assert ((*pit) != NULL);

    SacMalloc (tmp, IntList*, sizeof (IntList))
    tmp->val = newval;
    tmp->link = *pit;
    *pit = tmp;
    }
