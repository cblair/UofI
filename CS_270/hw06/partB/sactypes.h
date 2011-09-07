#ifndef _SACTYPES_H
#define _SACTYPES_H

typedef enum {
    Tnone,	/* no type */
    Unknown,	/* type not yet known */
    Bits,	/* bit vector */
    Bool,	/* boolean */
    Uint,	/* unsigned integer */
    Int,	/* signed integer */
    Ufix,	/* unsigned fixed point */
    Fix,	/* signed fixed point */
    Float,	/* 32-bit floating point */
    Double,	/* 64-bit floating point */
    CxInt,	/* complex with signed integer */
    CxFix,	/* complex with signed fixed point */
    CxFloat,	/* complex with 32-bit floats */
    CxDouble	/* complex with 64-bit floats */
    } Type;

typedef enum {
    Knone,	/* no kind */
    Array,	/* array */
    Scalar,	/* scalar */
    Func	/* function */
    } Kind;

typedef enum {
    StatNone,
    StatDecl,
    StatDef
    } Fstat;

typedef enum {
    Okay,
    Err
    } Tstat;

typedef struct tinfo
    {
    Tstat stat;
    Type type;
    int totsize;
    int fracsize;
    Kind kind;
    int dims;
    int dim_sizes[MAXRANK];	    /* -1 indicates no size available */
    struct tinfo *link;
    } TypeInfo;

int is_complex_num_type (Type);

#endif
