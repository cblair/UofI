#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#define MAXRANK 8

extern int mylineno;
extern FILE *yyin;

#define FALSE 0
#define TRUE !FALSE

#define YYSTYPE NODEPNT

typedef unsigned long long int Uint64;
typedef long long int Int64;

typedef struct sym_entry
    {
    char *name;
    int val;
    struct sym_entry *link;
    } SYM_ENTRY;

typedef struct intcell
    {
    int val;
    struct intcell *link;
    } IntList;

extern SYM_ENTRY *sym_table;
extern int sym_cnt;

void add_to_export_list (int);
SYM_ENTRY *handle_ident (char*);
char *handle_filename (char*);
char *find_sym (int);
int int_from_string (char*);
int is_hex (char*);
int is_binary (char*);
int is_float (char*);

typedef struct strlist {
    char *str;
    struct strlist *link;
    } StringList;

typedef struct {
    int line;
    char *file;
    char *func;
    } Location;

typedef enum {
    IntrinSin,
    IntrinCos,
    IntrinTan,
    IntrinAsin,
    IntrinAcos,
    IntrinAtan,
    IntrinAtan2,
    IntrinSinh,
    IntrinCosh,
    IntrinTanh,
    IntrinAsinh,
    IntrinAcosh,
    IntrinAtanh,
    IntrinSqrt,
    IntrinCbrt,
    IntrinPow,
    IntrinModf,
    IntrinExp,
    IntrinFrexp,
    IntrinLdexp,
    IntrinLog,
    IntrinLog10,
    IntrinExpm1,
    IntrinLog1p,
    IntrinCeil,
    IntrinFabs,
    IntrinFloor,
    IntrinFmod,
    IntrinCopysign,
    IntrinHypot,
    IntrinRint
    } Intrinsic;

void add_to_list (int, IntList**);
void append_intlist (IntList **, int);
int in_list (int, IntList**);
int length_intlist (IntList**);
void free_intlist (IntList**);
void show_intlist (IntList**, FILE*);
void reverse_intlist (IntList**);
void delete_ele_from_intlist (int, IntList**);
void replace_in_intlist (IntList**, int, int);
void copy_intlist (IntList**, IntList**);
void replace_val_with_list (int, IntList**, IntList**);
void insert_val_after_in_intlist (int, int, IntList**);
void insert_val_before_in_intlist (int, int, IntList**);
int sign_extend (int, int, int);
int yyparse ();
void const_fold_tree ();
void type_pass1 ();
void type_pass2 ();
void type_pass3 ();
void collect_decls ();
void build_ddcf ();
void push_array_cast ();
void code_motion ();
void ddcf_consistency ();
void func_inline ();
void lookup_tables ();
void window_tile ();
void ddcf2dfg ();
void switch_const ();
void size_propagate ();
void array_const_prop ();
void constant_fold ();
void identities ();
void strength_reduction ();
void dead_code ();
void full_unroll ();
void cse ();
void arrayref_elision ();
void loop_fuse ();
void window_to_elegen ();

extern char filename[];
extern char *funcname;
extern int module;
extern int use_dope_vectors;
extern int is_stable;
extern int gen_dfg;
extern int bit_width_opt;
extern int buf_size_in_words;
extern StringList *export_list;

#define SacMalloc(Tgt,Ty,Sz){\
    Tgt = (Ty) malloc (Sz);\
    if (Tgt == NULL)\
        handle_error (ErrMallocFailure, Deadly, 0, NULL, NULL);\
    }
#define SacFree(Tgt) {free (Tgt);}
