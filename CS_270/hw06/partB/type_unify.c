#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

extern Type rettype[14][14];
int is_signed (Type);

void make_composite_type (TypeInfo *t0, TypeInfo *t1, Type *pty, int *ptsz, int *pfsz)
    {
    Type rtype;
    int isz0, isz1, fsz0, fsz1, tsz0, tsz1;
    int isz, tsz, fsz;
    int signed0, signed1;

    rtype = rettype[t0->type][t1->type];
    *pty = rtype;

    switch (rtype)
        {
	case Bool :
	    *ptsz = 1;
	    *pfsz = 0;
	    break;
	case Bits :
	    tsz0 = t0->totsize;
	    tsz1 = t1->totsize;
	    tsz = (tsz0>tsz1)?tsz0:tsz1;
	    *ptsz = tsz;
	    *pfsz = 0;
	    break;
	case Float :
	case CxFloat :
	    *ptsz = 32;
	    *pfsz = 0;
	    break;
	case Double :
	case CxDouble :
	    *ptsz = 64;
	    *pfsz = 0;
	    break;
	default :
    	    signed0 = is_signed (t0->type);
    	    signed1 = is_signed (t1->type);

    	    fsz0 = t0->fracsize;
    	    fsz1 = t1->fracsize;
    	    isz0 = t0->totsize - fsz0;
    	    isz1 = t1->totsize - fsz1;

    	    if (signed0)
        	isz0--;
    	    if (signed1)
        	isz1--;

    	    isz = (isz0>isz1)?isz0:isz1;
    	    fsz = (fsz0>fsz1)?fsz0:fsz1;
    	    tsz = isz + fsz;
    	    if (signed0 || signed1)
        	tsz++;

	    if (tsz>32)
	        {
		*ptsz = 32;
		if (tsz - 32 < fsz)
		    *pfsz = fsz - (tsz - 32);
		else
		    *pfsz = 0;
		}
	    else
	        {
		*ptsz = tsz;
		*pfsz = fsz;
		}
	}
    }

int is_signed (Type ty)
    {
    switch (ty)
        {
	case Int :
	case Fix :
	case CxInt :
	case CxFix :
	    return TRUE;
	default :
            break;
	}
    return FALSE;
    }

Type rettype[14][14] = {
 /*          */ /*  Tnone   Unknown   Bits    Bool    Uint    Int     Ufix    Fix     Float   Double  CxInt     CxFix     CxFloat   CxDouble */
 /* ---------------------------------------------------------------------------------------------------------------------------------------- */
 /* Tnone    */     {Tnone,  Tnone,    Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,    Tnone,    Tnone,    Tnone},
 /* Unknown  */     {Tnone,  Unknown,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,    Tnone,    Tnone,    Tnone},
 /* Bits     */     {Tnone,  Tnone,    Bits,   Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,    Tnone,    Tnone,    Tnone},
 /* Bool     */     {Tnone,  Tnone,    Tnone,  Bool,   Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,    Tnone,    Tnone,    Tnone},
 /* Uint     */     {Tnone,  Tnone,    Tnone,  Tnone,  Uint,   Int,    Ufix,   Fix,    Float,  Double, Tnone,    Tnone,    Tnone,    Tnone},
 /* Int      */     {Tnone,  Tnone,    Tnone,  Tnone,  Int,    Int,    Fix,    Fix,    Float,  Double, Tnone,    Tnone,    Tnone,    Tnone},
 /* Ufix     */     {Tnone,  Tnone,    Tnone,  Tnone,  Ufix,   Fix,    Ufix,   Fix,    Float,  Double, Tnone,    Tnone,    Tnone,    Tnone},
 /* Fix      */     {Tnone,  Tnone,    Tnone,  Tnone,  Fix,    Fix,    Fix,    Fix,    Float,  Double, Tnone,    Tnone,    Tnone,    Tnone},
 /* Float    */     {Tnone,  Tnone,    Tnone,  Tnone,  Float,  Float,  Float,  Float,  Float,  Double, Tnone,    Tnone,    Tnone,    Tnone},
 /* Double   */     {Tnone,  Tnone,    Tnone,  Tnone,  Double, Double, Double, Double, Double, Double, Tnone,    Tnone,    Tnone,    Tnone},
 /* CxInt    */     {Tnone,  Tnone,    Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  CxInt,    CxFix,    CxFloat,  CxDouble},
 /* CxFix    */     {Tnone,  Tnone,    Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  CxFix,    CxFix,    CxFloat,  CxDouble},
 /* CxFloat  */     {Tnone,  Tnone,    Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  CxFloat,  CxFloat,  CxFloat,  CxDouble},
 /* CxDouble */     {Tnone,  Tnone,    Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  Tnone,  CxDouble, CxDouble, CxDouble, CxDouble}
 };
