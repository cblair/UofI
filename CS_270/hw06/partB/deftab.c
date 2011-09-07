#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

DEFENTRY *deftable = NULL;
int defsize = 0;
int defcnt = 0;

void print_a_type (TypeInfo*, FILE*);

int newdef (Tstat stat, Type type, int totsize,
		int fracsize, Kind kind, int dims, int symval)
    {
    DEFENTRY *new_space;
    int i;

    /*
    printf ("entering 'newdef'\n"); fflush (stdout);
    */
    if (defsize == 0)
        {
	/*
	printf ("  initiating deftable\n"); fflush (stdout);
	*/
	SacMalloc (deftable, DEFENTRY*, 10 * sizeof (DEFENTRY))
	defsize = 10;
	}

    if (defsize == defcnt)
        {
	/*
	printf ("  expanding deftable\n"); fflush (stdout);
	*/
	SacMalloc (new_space, DEFENTRY*, 2 * defsize * sizeof (DEFENTRY))
	bcopy (deftable, new_space, defsize * sizeof (DEFENTRY));
	SacFree (deftable)
	deftable = new_space;
	defsize *= 2;
	}

    deftable[defcnt].tinfo.stat = stat;
    deftable[defcnt].tinfo.type = type;
    deftable[defcnt].tinfo.totsize = totsize;
    deftable[defcnt].tinfo.fracsize = fracsize;
    deftable[defcnt].tinfo.kind = kind;
    deftable[defcnt].tinfo.dims = dims;
    deftable[defcnt].symval = symval;
    deftable[defcnt].funcinfo = (FUNCINFO*)NULL;
    for (i=0; i<MAXRANK; i++)
        deftable[defcnt].tinfo.dim_sizes[i] = -1;
    defcnt++;
    /*
    printf ("leaving 'newdef'\n"); fflush (stdout);
    */
    return defcnt-1;
    }

void show_deftable ()
    {
    int i, j;
    TypeInfo *ty;

    for (i=0; i<defcnt; i++)
        {
        printf ("defval %d:\n", i);

	if (deftable[i].tinfo.stat == Okay)
	    {
            printf ("  type:       %s\n",
	    			type_str (deftable[i].tinfo.type));
            printf ("  totsize:    %d\n", deftable[i].tinfo.totsize);

	    switch (deftable[i].tinfo.type)
	        {
		case Fix :
		case Ufix :
		case CxFix :
            	    printf ("  fracsize:   %d\n", deftable[i].tinfo.fracsize);
		default :
                    break;
		}

            printf ("  kind:       %s\n", kind_str (deftable[i].tinfo.kind));
            printf ("  dims:       %d\n", deftable[i].tinfo.dims);
	    if (deftable[i].tinfo.kind == Array)
	        {
	        printf ("  dim vals:   ");
	        for (j=0; j<deftable[i].tinfo.dims; j++)
		    if (deftable[i].tinfo.dim_sizes[j] == -1)
	                printf (" _ ");
		    else
		        printf (" %d ", deftable[i].tinfo.dim_sizes[j]);
	        printf ("\n");
		}
	    }
	else
	    printf ("  (associated with error)\n");

        printf ("  symval:     %d\n", deftable[i].symval);
        printf ("  name:       '%s'\n", find_sym (deftable[i].symval));

        if ((deftable[i].tinfo.kind == Func) && (deftable[i].funcinfo != NULL))
            {
            if (deftable[i].funcinfo->stat == StatDecl)
                printf ("  params: %d\n", deftable[i].funcinfo->nparams);
            else
                {
                printf ("  param dvs: ");
                for (j=0; j<deftable[i].funcinfo->nparams; j++)
                    printf (" %d", deftable[i].funcinfo->paramdvs[j]);
                printf ("\n");
                }

            printf ("  param types:  ");
            for (ty=deftable[i].funcinfo->paramtypes; ty!=NULL; ty=ty->link)
		{
		print_a_type (ty, stdout);
		if (ty->link != NULL)
		    printf (",  ");
		}
	    printf ("\n");

            printf ("  returns:    %d\n", deftable[i].funcinfo->nreturns);
            printf ("  return types:  ");
            for (ty=deftable[i].funcinfo->rettypes; ty!=NULL; ty=ty->link)
		{
		print_a_type (ty, stdout);
		if (ty->link != NULL)
		    printf (",  ");
		}
	    printf ("\n");

            printf ("  local dvs: ");
            for (j=0; j<deftable[i].funcinfo->nlocals; j++)
                printf (" %d", deftable[i].funcinfo->locdvs[j]);
            printf ("\n");
            }
        }
    }


void print_a_type (TypeInfo *tt, FILE *fp)
    {
    int i;

    switch (tt->type)
        {
	case Unknown :
	    fprintf (fp, "unknown type");
	    break;
	case Int :
	    fprintf (fp, "int%d", tt->totsize);
	    break;
	case Uint :
	    fprintf (fp, "uint%d", tt->totsize);
	    break;
	case Fix :
	    fprintf (fp, "fix%d.%d", tt->totsize, tt->fracsize);
	    break;
	case Ufix :
	    fprintf (fp, "ufix%d.%d", tt->totsize, tt->fracsize);
	    break;
	case Float :
	    fprintf (fp, "float");
	    break;
	case Double :
	    fprintf (fp, "double");
	    break;
	case CxInt :
	    fprintf (fp, "complex int%d", tt->totsize);
	    break;
	case CxFix :
	    fprintf (fp, "complex fix%d.%d", tt->totsize, tt->fracsize);
	    break;
	case CxFloat :
	    fprintf (fp, "complex float");
	    break;
	case CxDouble :
	    fprintf (fp, "complex double");
	    break;
	case Bool :
	    fprintf (fp, "bool");
	    break;
	case Bits :
	    fprintf (fp, "bits%d", tt->totsize);
	    break;
	default :
            break;
	}

    if (tt->kind == Array)
        {
	fprintf (fp, "[");
	for (i=0; i<tt->dims; i++)
	    {
	    if ((tt->dim_sizes)[i] < 0)
	        fprintf (fp, ":");
	    else
	        fprintf (fp, "%d", (tt->dim_sizes)[i]);

	    if (i < tt->dims-1)
	        fprintf (fp, ",");
	    }
	fprintf (fp, "]");
	}
    }

char *type_str (Type t)
    {
    switch (t)
	{
	case Tnone :
	    return "Tnone";
	case Bool :
	    return "Bool";
	case Int :
	    return "Int";
	case Uint :
	    return "Uint";
	case Fix :
	    return "Fix";
	case Ufix :
	    return "Ufix";
	case Float :
	    return "Float";
	case Double :
	    return "Double";
	case CxInt :
	    return "CxInt";
	case CxFix :
	    return "CxFix";
	case CxFloat :
	    return "CxFloat";
	case CxDouble :
	    return "CxDouble";
	case Bits :
	    return "Bits";
	case Unknown :
	    return "Unknown";
	default :
	    assert (0);
	}
    }

char *kind_str (Kind k)
    {
    switch (k)
	{
	case Knone :
	    return "Knone";
	case Array :
	    return "Array";
	case Scalar :
	    return "Scalar";
	case Func :
	    return "Func";
	default :
	    assert (0);
	}
    }
