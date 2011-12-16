/*******************************************************************************

   MACROS.H

   Author: Peter Loan

   Date of creation: 01-JUL-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains all macros which are used by the Dynamics
                Pipeline.

*******************************************************************************/

#ifndef MACROS_H
#define MACROS_H

#define MALLOC(a) ((a)*)simm_malloc(sizeof(a))
#define ABS(a) ((a)>0?(a):(-(a)))
#define DOUBLE_ABS(a) ((a)>(double)0.0?(a):(-(a)))
#define DABS(a) ((a)>(double)0.0?(a):(-(a)))
#define EQUAL_WITHIN_ERROR(a,b) (DABS(((a)-(b))) <= ROUNDOFF_ERROR)
#define NOT_EQUAL_WITHIN_ERROR(a,b) (DABS(((a)-(b))) > ROUNDOFF_ERROR)
#define EQUAL_WITHIN_TOLERANCE(a,b,c) (DABS(((a)-(b))) <= (c))
#define NOT_EQUAL_WITHIN_TOLERANCE(a,b,c) (DABS(((a)-(b))) > (c))
#define NEAR_LT_OR_EQ(a, b) ((b) <= (b) + ROUNDOFF_ERROR)
#define NEAR_GT_OR_EQ(a, b) ((a) >= (b) - ROUNDOFF_ERROR)
#define SIGN(a) ((a)>=0?(1):(-1))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define MIN(a,b) ((a)<=(b)?(a):(b))
#define SQR(x)	((x) * (x))
#define CUBE(x)	((x) * (x) * (x))
#define DSIGN(a) ((a)>=0.0?(1):(-1))
#define CEILING(a,b) (((a)+(b)-1)/(b))
#define FREE_IFNOTNULL(p) if (p != NULL) free(p)
#define STRLEN(p) (strlen(p)+1)
#define CURSOR_IN_REGION_PTR(mx,my,vp) ((SBoolean) ((mx) >= (vp)->x1 && (mx) <= (vp)->x2 && \
(my) >= (vp)->y1 && (my) <= (vp)->y2))
#define CURSOR_IN_REGION(mx,my,vp) ((SBoolean) ((mx) >= (vp).x1 && (mx) <= (vp).x2 && \
(my) >= (vp).y1 && (my) <= (vp).y2))
#define PERCENT_FROM_MIDPOINT(mx,vp) (((double)(mx)-((vp)->x1+(vp)->x2)/2)/(((vp)->x2-(vp)->x1)/2))
#define DISTANCE_FROM_MIDPOINT(mx,vp) (((mx)-((vp)->x1+(vp)->x2)/2))
#define VECTOR_MAGNITUDE(vec) (sqrt((vec[0]*vec[0])+(vec[1]*vec[1])+(vec[2]*vec[2])))
#define CHAR_IS_WHITE_SPACE(ch) ((ch) == ' ' || (ch) == '\t' || (ch) == '\n' || (ch) == '\r')
#define CHAR_IS_NOT_WHITE_SPACE(ch) ((ch) != ' ' && (ch) != '\t' && (ch) != '\n' && (ch) != '\r')
#define NULLIFY_STRING(str) ((str)[0] = '\0')
#define STRING_IS_NULL(str) (str[0] == '\0')
#define STRING_IS_NOT_NULL(str) (str[0] != '\0')
#define STRINGS_ARE_EQUAL(ptr1,ptr2) (!strcmp(ptr1,ptr2))
#define STRINGS_ARE_NOT_EQUAL(ptr1,ptr2) (strcmp(ptr1,ptr2))
#define FORCE_VALUE_INTO_RANGE(a,b,c) if ((a)<(b)) (a)=(b); else if ((a)>(c)) (a)=(c);
#define MODULATE_VALUE_INTO_RANGE(a,b,c) while ((a)<(b)) (a)+=((c)-(b)); \
while ((a)>(c)) (a)-=((c)-(b));
#define CHECK_DIVIDE(t,b) ((ABS(b))<TINY_NUMBER)?(ERROR_DOUBLE):((t)/(b))
#define SET_BOX1221(box,a,b,c,d) (box).x1=(a);(box).x2=(b);(box).y2=(c);(box).y1=(d)
#define SET_BOX(box,a,b,c,d) (box).x1=(a); (box).x2=(b); (box).y1=(c); (box).y2=(d)
#define GET_PATH(mod,f1,f2) (model[mod]->pathptrs[model[mod]->numsegments*f1+f2])
#define MAKE_3DVECTOR(pt1,pt2,pt3) {pt3[XX]=pt2[XX]-pt1[XX];pt3[YY]=pt2[YY]-pt1[YY];pt3[ZZ]=pt2[ZZ]-pt1[ZZ];}
#define MAKE_3DVECTOR21(pt1,pt2,pt3) {pt3[XX]=pt1[XX]-pt2[XX];pt3[YY]=pt1[YY]-pt2[YY];pt3[ZZ]=pt1[ZZ]-pt2[ZZ];}
#define DOT_VECTORS(u,v)      ( (u[0])*(v[0]) + (u[1])*(v[1]) + (u[2])*(v[2]) )
#define CALC_DETERMINANT(m) (((m[0][0]) * ((m[1][1])*(m[2][2]) - (m[1][2])*(m[2][1]))) - \
((m[0][1]) * ((m[1][0])*(m[2][2]) - (m[1][2])*(m[2][0]))) + \
((m[0][2]) * ((m[1][0])*(m[2][1]) - (m[1][1])*(m[2][0]))))
#define ANGLES_APX_EQ(x, y)              (DABS((x) - (y)) <= ANGLE_EPSILON)
#define PTS_ARE_EQUAL(Pt1, Pt2) (BOOL_APX_EQ(Pt1[0], Pt2[0]) && \
                                 BOOL_APX_EQ(Pt1[1], Pt2[1]) && \
                                 BOOL_APX_EQ(Pt1[2], Pt2[2]))

#define PTS_ARE_EQUAL3(Pt1, Pt2) (BOOL_APX_EQ3(Pt1[0], Pt2[0]) && \
				  BOOL_APX_EQ3(Pt1[1], Pt2[1]) && \
				  BOOL_APX_EQ3(Pt1[2], Pt2[2]))

#define READ4(ptr,fp) {\
*(((char*)(ptr))  )=getc(fp);\
*(((char*)(ptr))+1)=getc(fp);\
*(((char*)(ptr))+2)=getc(fp);\
*(((char*)(ptr))+3)=getc(fp);\
}

#define COPY_1X4VECTOR(from,to) {\
to[0] = from[0];\
to[1] = from[1];\
to[2] = from[2];\
to[3] = from[3];\
}
#define COPY_1X3VECTOR(from,to) {\
to[0] = from[0];\
to[1] = from[1];\
to[2] = from[2];\
}

#define MAKE_IDENTITY_MATRIX(mat) {\
mat[0][1] = mat[0][2] = mat[0][3] = 0.0;\
mat[1][0] = mat[1][2] = mat[1][3] = 0.0;\
mat[2][0] = mat[2][1] = mat[2][3] = 0.0;\
mat[3][0] = mat[3][1] = mat[3][2] = 0.0;\
mat[0][0] = mat[1][1] = mat[2][2] = mat[3][3] = 1.0;\
}

#endif /* MACROS_H */
