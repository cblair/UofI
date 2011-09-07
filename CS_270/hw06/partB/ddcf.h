#include "sactypes.h"

#define CONST_STR_SIZE 128

typedef enum {
    /* compound nodes */
    ND_SWITCH,
    ND_CASE,
    ND_WHILE,
    ND_FORALL,
    ND_FORNXT,
    ND_FUNC,
    ND_CROSS_PROD,
    ND_DOT_PROD,
    ND_WHILE_PRED,

    /* 2-input, 1-output operator nodes */
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_MUL_MACH,
    ND_DIV,
    ND_MOD,
    ND_COMPLEX,
    ND_LT,
    ND_GT,
    ND_LE,
    ND_GE,
    ND_NEQ,
    ND_EQ,
    ND_AND,
    ND_OR,
    ND_BIT_AND,
    ND_BIT_OR,
    ND_BIT_EOR,
    ND_LEFT_SHIFT,
    ND_LEFT_SHIFT_MACH,
    ND_RIGHT_SHIFT,
    ND_SQRT_MACH,

    /* loop-related nodes */
    ND_SCALAR_GEN,
    ND_ELE_GEN,
    ND_SLICE_GEN,
    ND_WINDOW_GEN,
    ND_LOOP_INDICES,
    ND_REDUCE_SUM,
    ND_REDUCE_MIN,
    ND_REDUCE_MAX,
    ND_REDUCE_AND,
    ND_REDUCE_OR,
    ND_REDUCE_VAL_AT_FIRST_MAX,
    ND_REDUCE_VAL_AT_FIRST_MIN,
    ND_REDUCE_VAL_AT_LAST_MAX,
    ND_REDUCE_VAL_AT_LAST_MIN,
    ND_REDUCE_VAL_AT_MAXS,
    ND_REDUCE_VAL_AT_MINS,
    ND_REDUCE_PRODUCT,
    ND_REDUCE_MEAN,
    ND_REDUCE_ST_DEV,
    ND_REDUCE_MODE,
    ND_REDUCE_MEDIAN,
    ND_REDUCE_HIST,
    ND_CONSTRUCT_ARRAY,
    ND_CONSTRUCT_CONCAT,
    ND_CONSTRUCT_CONCAT_MASKED,
    ND_CONSTRUCT_CONCAT_MASKED_MANY,
    ND_CONSTRUCT_TILE,

    ND_ACCUM_SUM,
    ND_ACCUM_MIN,
    ND_ACCUM_MAX,
    ND_ACCUM_AND,
    ND_ACCUM_OR,
    ND_ACCUM_PRODUCT,
    ND_ACCUM_MEAN,
    ND_ACCUM_ST_DEV,
    ND_ACCUM_MEDIAN,
    ND_ACCUM_HIST,

    /* new macro nodes for unrolled loops */
    ND_REDUCE_SUM_MACRO,
    ND_REDUCE_PRODUCT_MACRO,
    ND_REDUCE_AND_MACRO,
    ND_REDUCE_OR_MACRO,
    ND_REDUCE_MIN_MACRO,
    ND_REDUCE_MAX_MACRO,
    ND_REDUCE_MEDIAN_MACRO,
    ND_REDUCE_UMEDIAN_MACRO,
    ND_REDUCE_IMEDIAN_MACRO,
    ND_REDUCE_VAL_AT_FIRST_MIN_MACRO,
    ND_REDUCE_VAL_AT_FIRST_MAX_MACRO,
    ND_REDUCE_VAL_AT_LAST_MIN_MACRO,
    ND_REDUCE_VAL_AT_LAST_MAX_MACRO,
    ND_REDUCE_VAL_AT_MINS_MACRO,
    ND_REDUCE_VAL_AT_MAXS_MACRO,
    ND_REDUCE_HIST_MACRO,

    /* input and output nodes for compounds */
    ND_G_INPUT,
    ND_G_INPUT_NEXT,
    ND_G_OUTPUT,

    /* 1-input, 1-output operator nodes */
    ND_REAL,
    ND_IMAG,
    ND_NOT,
    ND_NEG,

    /* other various kinds... */
    ND_ARR_CONCAT,
    ND_ARR_VERT_CONCAT,
    ND_CAST,
    ND_FCALL,
    ND_VHDL_CALL,
    ND_EXTENTS,
    ND_INTRINCALL,
    ND_ARR_CONPERIM,
    ND_ARRAYREF,
    ND_ARR_DEF,
    ND_SWITCH_KEY,
    ND_SELECTORS,
    ND_NEXT,
    ND_PRINT,
    ND_ASSERT,
    ND_VOIDED,
    ND_FEED_NEXT,
    ND_GRAPH,

    ND_RC_COMPUTE,
    ND_MALL_XFER,
    ND_RANK,
    ND_SIZE,
    ND_W_LOOP_EXTENT,
    ND_MALLOC_TGT_1_BIT_1D,
    ND_MALLOC_TGT_1_BIT_2D,
    ND_MALLOC_TGT_1_BIT_3D,
    ND_MALLOC_TGT_2_BIT_1D,
    ND_MALLOC_TGT_2_BIT_2D,
    ND_MALLOC_TGT_2_BIT_3D,
    ND_MALLOC_TGT_4_BIT_1D,
    ND_MALLOC_TGT_4_BIT_2D,
    ND_MALLOC_TGT_4_BIT_3D,
    ND_MALLOC_TGT_8_BIT_1D,
    ND_MALLOC_TGT_8_BIT_2D,
    ND_MALLOC_TGT_8_BIT_3D,
    ND_MALLOC_TGT_16_BIT_1D,
    ND_MALLOC_TGT_16_BIT_2D,
    ND_MALLOC_TGT_16_BIT_3D,
    ND_MALLOC_TGT_32_BIT_1D,
    ND_MALLOC_TGT_32_BIT_2D,
    ND_MALLOC_TGT_32_BIT_3D,
    ND_TRANSFER_TO_HOST_RET_ARRAY,
    ND_TRANSFER_TO_HOST_SCALAR,
    ND_BUFFER_TERMINATE,
    ND_RC_FORALL,
    ND_RC_WINDOW_GEN_1D,
    ND_RC_WINDOW_GEN_2D,
    ND_RC_SLICE_GEN_2D_COL,
    ND_RC_SLICE_GEN_2D_ROW,
    ND_RC_ELE_GEN_1D,
    ND_RC_ELE_GEN_2D,
    ND_RC_ELE_GEN_3D,
    ND_WRITE_TILE_1D_1D,
    ND_WRITE_TILE_1D_2D,
    ND_WRITE_TILE_1D_3D,
    ND_WRITE_TILE_2D_1D,
    ND_WRITE_TILE_2D_2D,
    ND_WRITE_TILE_2D_3D,
    ND_WRITE_TILE_3D_1D,
    ND_WRITE_TILE_3D_2D,
    ND_WRITE_TILE_3D_3D,
    ND_ARRAYREF_1D,
    ND_ARRAYREF_2D,
    ND_ARRAYREF_3D,
    ND_ARRAYREF_4D,
    ND_ARRAYREF_5D,
    ND_ARRAYREF_6D,
    ND_ARRAYREF_7D,
    ND_ARRAYREF_8D,
    ND_REDUCE_VAL_AT_FIRST_IMAX_MACRO,
    ND_REDUCE_VAL_AT_FIRST_IMIN_MACRO,
    ND_REDUCE_VAL_AT_LAST_IMAX_MACRO,
    ND_REDUCE_VAL_AT_LAST_IMIN_MACRO,
    ND_REDUCE_VAL_AT_FIRST_UMAX_MACRO,
    ND_REDUCE_VAL_AT_FIRST_UMIN_MACRO,
    ND_REDUCE_VAL_AT_LAST_UMAX_MACRO,
    ND_REDUCE_VAL_AT_LAST_UMIN_MACRO,
    ND_REDUCE_ISUM_MACRO,
    ND_REDUCE_USUM_MACRO,
    ND_REDUCE_IMIN_MACRO,
    ND_REDUCE_UMIN_MACRO,
    ND_REDUCE_IMAX_MACRO,
    ND_REDUCE_UMAX_MACRO,
    ND_USUM_VALUES,
    ND_ISUM_VALUES,
    ND_UMIN_VALUES,
    ND_IMIN_VALUES,
    ND_UMAX_VALUES,
    ND_IMAX_VALUES,
    ND_AND_VALUES,
    ND_OR_VALUES,
    ND_HIST_VALUES,
    ND_ACCUM_USUM_VALUES,
    ND_ACCUM_ISUM_VALUES,
    ND_ACCUM_UMAX_VALUES,
    ND_ACCUM_IMAX_VALUES,
    ND_ACCUM_UMIN_VALUES,
    ND_ACCUM_IMIN_VALUES,
    ND_ACCUM_AND_VALUES,
    ND_ACCUM_OR_VALUES,
    ND_ACCUM_HIST_VALUES,
    ND_IADD,
    ND_UADD,
    ND_ISUB,
    ND_USUB,
    ND_IMUL,
    ND_UMUL,
    ND_ULT,
    ND_ILT,
    ND_ULE,
    ND_ILE,
    ND_UGT,
    ND_IGT,
    ND_UGE,
    ND_IGE,
    ND_UEQ,
    ND_IEQ,
    ND_UNEQ,
    ND_INEQ,
    ND_BIT_COMPL,
    ND_CHANGE_WIDTH,
    ND_CHANGE_WIDTH_SE,
    ND_RC_SELECTOR,
    ND_CIRCULATE,
    ND_WRITE_SCALAR,
    ND_WRITE_VALS_TO_BUFFER
    } DdcfType;

typedef enum {
    PRAG_VHDL,		/* func-prototype-related */
    PRAG_NO_UNROLL,	/* loop-related */
    PRAG_NO_INLINE,	/* func-related */
    PRAG_NO_DFG,	/* loop-related */
    PRAG_LOOKUP,	/* func-related */
    PRAG_STRIPMINE,	/* loop-related */
    PRAG_NO_FUSE,	/* loop-related */
    PRAG_BLOCK		/* loop-related */
    } Pragma;

typedef struct prag_cell {
    Pragma prag;
    int vals[MAXRANK];
    char *str;
    struct prag_cell *link;
    } PragCell;

typedef struct edge {
    int node;
    int port;
    struct edge *link;
    } Edge;

typedef struct {
    TypeInfo ty;
    int is_const;
    char *constval;
    Edge *back_edges;
    Uint64 maxval;
    } InputPort;

typedef struct {
    int unique_id;	/* unique id within its function */
    TypeInfo ty;
    Edge *targets;
    Uint64 maxval;
    } OutputPort;

#define In_next_id specific.in_next_id
#define Io_num specific.io_info.io_num
#define My_graph specific.io_info.my_graph
#define My_inputs specific.g_info.my_inputs
#define My_outputs specific.g_info.my_outputs
#define My_nodes specific.g_info.my_nodes
#define Sym specific.sym
#define Intrin_func specific.intrin_func
#define Reftypes specific.reftypes
#define DefDims specific.arr_def_info.dims
#define DefRank specific.arr_def_info.rank
#define Rank specific.rank
#define VecSize specific.vecsize

typedef struct {
    DdcfType nodetype;
    Location loc;
    int num_inputs;
    InputPort *inputs;
    int num_outputs;
    OutputPort *outputs;
    union {
	int in_next_id;		/* for ND_NEXT nodes */
	struct {
	    int io_num;
	    int my_graph;
	    } io_info;		/* for ND_G_INPUT and ND_G_OUTPUT nodes */
	char *sym;		/* for ND_FCALL nodes */
	Intrinsic intrin_func;	/* for ND_INTRINCALL nodes */
	char *reftypes;		/* for ND_ARRAYREF and ND_ELE_GEN nodes */
	int rank;		/* for ND_SCALAR_GEN nodes */
	struct {
	    int rank;
	    int *dims;
	    } arr_def_info;	/* for ND_ARR_DEF nodes */
	struct {
	    int *my_inputs;
	    int *my_outputs;
	    IntList *my_nodes;
	    } g_info;		/* for any compound graph node */
	int vecsize;		/* for ND_REDUCE_VAL_AT_MxxS */
	} specific;
    int dim_sizes[8];		/* to hold extents of loop nodes */
    PragCell *pragmas;
    } DdcfNode;

typedef struct funcgraph {
    char *name;
    TypeInfo *params;
    TypeInfo *rets;
    DdcfNode *nodes;
    int nodes_allocated;
    int nodes_used;
    struct funcgraph *link;
    } FuncGraph;

typedef struct {
    int node;
    int port;
    } NodePort;

typedef struct {
    int is_const;
    TypeInfo ty;
    union {
        NodePort np;
	char constval[CONST_STR_SIZE];
	} ref;
    } AssocData;

typedef struct assoc {
    int dv;
    AssocData dt;
    struct assoc *link;
    } Assoc;

typedef struct input_item {
    int input_id;
    struct input_item *link;
    } InputItem;

typedef struct stack_item{
    int current_graph_id;
    Assoc *assocs;
    int assoc_idx;
    InputItem *inputs;
    int inputs_idx;
    IntList *my_nodes;
    struct stack_item *link;
    } StackItem;

typedef struct loop_strings {
    int id;
    DdcfType ty;
    int n;
    int var[8];
    char init[8][256];
    char incr[8][256];
    char terminate[8][256];
    char size[8][256];
    char totsize[256];
    struct loop_strings *link;
    } GenStrings;

void add_edge (FuncGraph *fg, int src_nd, int src_pt, int tgt_nd, int tgt_pt);
void remove_edge (FuncGraph *fg, int src_nd, int src_pt, int tgt_nd, int tgt_pt);
void move_graph_input (FuncGraph *fg, int id, int src, int tgt);
void retarget (FuncGraph *fg, Edge *bks, int old_nd, int old_pt, int new_nd, int new_pt);
void retarget_backedges (FuncGraph *fg, Edge *tgs, int old_nd, int old_pt, int new_nd, int new_pt);
void remove_targets_to_this_port (FuncGraph *fg, Edge *bks, int nd, int pt);
void remove_backedges_to_this_target (FuncGraph *fg, Edge *tgs, int nd, int pt);
void remove_any_edges_to_a_target_node (FuncGraph*, int, int, int);
int transfer_const_to_targets (FuncGraph *fg, int id, int out_port, char *str);
void free_ddcf_node (FuncGraph *fg, int id, IntList **itlist);
void append_target_list (Edge **tg1, Edge *tg2);
void free_edge_list (Edge **plist);
int is_simple_node (FuncGraph *fg, int id);
void copy_target_list (Edge **tgts, Edge *srcs);
Edge *malloc_target (int nd, int pt, Edge *link);
Edge *malloc_backedge (int nd, int pt, Edge *link);
void install_back_edges_for_node (FuncGraph *fg, int nd);
void install_back_edges (FuncGraph *fg);
int new_ddcf_node (FuncGraph *fg, DdcfType nty, int ninputs, int noutputs, Location loc);
int new_ddcf_graph (FuncGraph *fg, DdcfType nty, int num_inputs, int num_outputs, Location loc);
int type_equal (TypeInfo *t1, TypeInfo *t2);
void copy_type_info (TypeInfo *t0, TypeInfo *t1);
InputPort *make_input_ports (int n);
OutputPort *make_output_ports (int n);
int alloc_ddcf_node (FuncGraph *fg, Location loc);
int find_generator_graph (FuncGraph *fg, int id);
int find_switch_key (FuncGraph *fg, int id);
int find_selector_node (FuncGraph *fg, int id);
extern unsigned int mask_ones[33];
extern unsigned int sc_pow2[];
extern unsigned int set_ones[33];
void eliminate_graph_input (FuncGraph *fg, int id, int idx);
int all_inputs_are_constant (FuncGraph *fg, int id);
char *ddcf_node_str (DdcfType);
int is_reduction (DdcfType nd);
void destroy_graph (FuncGraph *fg, int id);
int *make_io_array (int n);
int propagate_and_delete (FuncGraph *fg, int id, char *str, IntList **itlist);
int copy_graph (FuncGraph*, FuncGraph*, int);
void lift_graph (FuncGraph*, int, IntList**);
void ddcf2c (char*);
int is_compound (DdcfType);
char *copy_to_const (char*);
PragCell *find_pragma (FuncGraph*, int, Pragma);
int has_pragma (FuncGraph*, int, Pragma);
void eliminate_pragma (FuncGraph*, int, Pragma);
void add_pragma (PragCell**, Pragma, int*);
PragCell *copy_pragmas (PragCell*);
void show_ddcf_func (FuncGraph*, FILE*, int);
void insert_node_in_intlist (FuncGraph*, int, IntList**);
void elegen_to_window (FuncGraph*, int, int, int);
int get_loop_rank (FuncGraph*, int);
Edge *copy_edge_list (Edge*);
void set_type_to_bool (TypeInfo *tt);
void set_type_to_uint (TypeInfo *tt, int sz);
void set_type_to_int (TypeInfo *tt, int sz);

FuncGraph *fgraphs;
