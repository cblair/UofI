#include "sassy.h"
#include "ddcf.h"
#include "error.h"

/*
#define SHOW_MAXVALS
*/

void print_a_type (TypeInfo*, FILE*);
void print_dims (int*, FILE*);
char *intrin_str (Intrinsic);
void assign_assocs (FuncGraph*, int*, int);
void show_intlist_assocs (IntList**, int*, FILE*);

char *ddcf_node_str (DdcfType ty)
    {
    switch (ty)
        {
	case ND_SWITCH :
	    return "ND_SWITCH";
	case ND_SWITCH_KEY :
	    return "ND_SWITCH_KEY";
	case ND_WHILE :
	    return "ND_WHILE";
	case ND_FORNXT :
	    return "ND_FORNXT";
	case ND_FORALL :
	    return "ND_FORALL";
	case ND_FUNC :
	    return "ND_FUNC";
	case ND_CROSS_PROD :
	    return "ND_CROSS_PROD";
	case ND_DOT_PROD :
	    return "ND_DOT_PROD";
	case ND_ADD :
	    return "ND_ADD";
	case ND_SUB :
	    return "ND_SUB";
	case ND_MUL :
	    return "ND_MUL";
	case ND_MUL_MACH :
	    return "ND_MUL_MACH";
	case ND_DIV :
	    return "ND_DIV";
	case ND_MOD :
	    return "ND_MOD";
	case ND_ELE_GEN :
	    return "ND_ELE_GEN";
	case ND_SLICE_GEN :
	    return "ND_SLICE_GEN";
	case ND_WINDOW_GEN :
	    return "ND_WINDOW_GEN";
	case ND_SCALAR_GEN :
	    return "ND_SCALAR_GEN";
	case ND_LOOP_INDICES :
	    return "ND_LOOP_INDICES";
	case ND_REDUCE_SUM :
	    return "ND_REDUCE_SUM";
	case ND_REDUCE_MIN :
	    return "ND_REDUCE_MIN";
	case ND_REDUCE_MAX :
	    return "ND_REDUCE_MAX";
	case ND_REDUCE_AND :
	    return "ND_REDUCE_AND";
	case ND_REDUCE_OR :
	    return "ND_REDUCE_OR";
	case ND_REDUCE_VAL_AT_FIRST_MAX :
	    return "ND_REDUCE_VAL_AT_FIRST_MAX";
	case ND_REDUCE_VAL_AT_FIRST_MIN :
	    return "ND_REDUCE_VAL_AT_FIRST_MIN";
	case ND_REDUCE_VAL_AT_LAST_MAX :
	    return "ND_REDUCE_VAL_AT_LAST_MAX";
	case ND_REDUCE_VAL_AT_LAST_MIN :
	    return "ND_REDUCE_VAL_AT_LAST_MIN";
	case ND_REDUCE_VAL_AT_MAXS :
	    return "ND_REDUCE_VAL_AT_MAXS";
	case ND_REDUCE_VAL_AT_MINS :
	    return "ND_REDUCE_VAL_AT_MINS";
	case ND_REDUCE_PRODUCT :
	    return "ND_REDUCE_PRODUCT";
	case ND_REDUCE_MEAN :
	    return "ND_REDUCE_MEAN";
	case ND_REDUCE_ST_DEV :
	    return "ND_REDUCE_ST_DEV";
	case ND_REDUCE_MODE :
	    return "ND_REDUCE_MODE";
	case ND_REDUCE_MEDIAN :
	    return "ND_REDUCE_MEDIAN";
	case ND_REDUCE_HIST :
	    return "ND_REDUCE_HIST";
	case ND_ACCUM_SUM :
	    return "ND_ACCUM_SUM";
	case ND_ACCUM_MIN :
	    return "ND_ACCUM_MIN";
	case ND_ACCUM_MAX :
	    return "ND_ACCUM_MAX";
	case ND_ACCUM_AND :
	    return "ND_ACCUM_AND";
	case ND_ACCUM_OR :
	    return "ND_ACCUM_OR";
	case ND_ACCUM_PRODUCT :
	    return "ND_ACCUM_PRODUCT";
	case ND_ACCUM_MEAN :
	    return "ND_ACCUM_MEAN";
	case ND_ACCUM_ST_DEV :
	    return "ND_ACCUM_ST_DEV";
	case ND_ACCUM_MEDIAN :
	    return "ND_ACCUM_MEDIAN";
	case ND_ACCUM_HIST :
	    return "ND_ACCUM_HIST";
	case ND_CONSTRUCT_ARRAY :
	    return "ND_CONSTRUCT_ARRAY";
	case ND_CONSTRUCT_CONCAT :
	    return "ND_CONSTRUCT_CONCAT";
	case ND_CONSTRUCT_CONCAT_MASKED :
	    return "ND_CONSTRUCT_CONCAT_MASKED";
	case ND_CONSTRUCT_CONCAT_MASKED_MANY :
	    return "ND_CONSTRUCT_CONCAT_MASKED_MANY";
	case ND_CONSTRUCT_TILE :
	    return "ND_CONSTRUCT_TILE";
	case ND_G_INPUT_NEXT :
	    return "ND_G_INPUT_NEXT";
	case ND_G_INPUT :
	    return "ND_G_INPUT";
	case ND_G_OUTPUT :
	    return "ND_G_OUTPUT";
	case ND_COMPLEX :
	    return "ND_COMPLEX";
	case ND_LT :
	    return "ND_LT";
	case ND_GT :
	    return "ND_GT";
	case ND_LE :
	    return "ND_LE";
	case ND_GE :
	    return "ND_GE";
	case ND_NEQ :
	    return "ND_NEQ";
	case ND_EQ :
	    return "ND_EQ";
	case ND_AND :
	    return "ND_AND";
	case ND_OR :
	    return "ND_OR";
	case ND_BIT_AND :
	    return "ND_BIT_AND";
	case ND_BIT_OR :
	    return "ND_BIT_OR";
	case ND_BIT_EOR :
	    return "ND_BIT_EOR";
	case ND_LEFT_SHIFT :
	    return "ND_LEFT_SHIFT";
	case ND_LEFT_SHIFT_MACH :
	    return "ND_LEFT_SHIFT_MACH";
	case ND_SQRT_MACH :
	    return "ND_SQRT_MACH";
	case ND_RIGHT_SHIFT :
	    return "ND_RIGHT_SHIFT";
	case ND_REAL :
	    return "ND_REAL";
	case ND_IMAG :
	    return "ND_IMAG";
	case ND_NOT :
	    return "ND_NOT";
	case ND_NEG :
	    return "ND_NEG";
	case ND_ARR_CONCAT :
	    return "ND_ARR_CONCAT";
	case ND_ARR_VERT_CONCAT :
	    return "ND_ARR_VERT_CONCAT";
	case ND_CAST :
	    return "ND_CAST";
	case ND_FCALL :
	    return "ND_FCALL";
	case ND_VHDL_CALL :
	    return "ND_VHDL_CALL";
	case ND_EXTENTS :
	    return "ND_EXTENTS";
	case ND_INTRINCALL :
	    return "ND_INTRINCALL";
	case ND_ARR_CONPERIM :
	    return "ND_ARR_CONPERIM";
	case ND_ARRAYREF :
	    return "ND_ARRAYREF";
	case ND_ARR_DEF :
	    return "ND_ARR_DEF";
	case ND_WHILE_PRED :
	    return "ND_WHILE_PRED";
	case ND_CASE :
	    return "ND_CASE";
	case ND_SELECTORS :
	    return "ND_SELECTORS";
	case ND_NEXT :
	    return "ND_NEXT";
	case ND_PRINT :
	    return "ND_PRINT";
	case ND_ASSERT :
	    return "ND_ASSERT";
	case ND_VOIDED :
	    return "ND_VOIDED";
	case ND_FEED_NEXT :
	    return "ND_FEED_NEXT";
	case ND_GRAPH :
	    return "ND_GRAPH";
	case ND_REDUCE_SUM_MACRO :
	    return "ND_REDUCE_SUM_MACRO";
	case ND_REDUCE_PRODUCT_MACRO :
	    return "ND_REDUCE_PRODUCT_MACRO";
	case ND_REDUCE_AND_MACRO :
	    return "ND_REDUCE_AND_MACRO";
	case ND_REDUCE_OR_MACRO :
	    return "ND_REDUCE_OR_MACRO";
	case ND_REDUCE_MIN_MACRO :
	    return "ND_REDUCE_MIN_MACRO";
	case ND_REDUCE_MAX_MACRO :
	    return "ND_REDUCE_MAX_MACRO";
	case ND_REDUCE_MEDIAN_MACRO :
	    return "ND_REDUCE_MEDIAN_MACRO";
	case ND_REDUCE_UMEDIAN_MACRO :
	    return "ND_REDUCE_UMEDIAN_MACRO";
	case ND_REDUCE_IMEDIAN_MACRO :
	    return "ND_REDUCE_IMEDIAN_MACRO";
	case ND_REDUCE_VAL_AT_FIRST_MIN_MACRO :
	    return "ND_REDUCE_VAL_AT_FIRST_MIN_MACRO";
	case ND_REDUCE_VAL_AT_FIRST_MAX_MACRO :
	    return "ND_REDUCE_VAL_AT_FIRST_MAX_MACRO";
	case ND_REDUCE_VAL_AT_LAST_MIN_MACRO :
	    return "ND_REDUCE_VAL_AT_LAST_MIN_MACRO";
	case ND_REDUCE_VAL_AT_LAST_MAX_MACRO :
	    return "ND_REDUCE_VAL_AT_LAST_MAX_MACRO";
	case ND_REDUCE_VAL_AT_MINS_MACRO :
	    return "ND_REDUCE_VAL_AT_MINS_MACRO";
	case ND_REDUCE_VAL_AT_MAXS_MACRO :
	    return "ND_REDUCE_VAL_AT_MAXS_MACRO";
	case ND_REDUCE_HIST_MACRO :
	    return "ND_REDUCE_HIST_MACRO";
	case ND_RC_COMPUTE :
	    return "ND_RC_COMPUTE";
	case ND_MALL_XFER :
	    return "ND_MALL_XFER";
	case ND_RANK :
	    return "ND_RANK";
	case ND_SIZE :
	    return "ND_SIZE";
	case ND_W_LOOP_EXTENT :
	    return "ND_W_LOOP_EXTENT";
	case ND_MALLOC_TGT_1_BIT_1D :
	    return "ND_MALLOC_TGT_1_BIT_1D";
	case ND_MALLOC_TGT_1_BIT_2D :
	    return "ND_MALLOC_TGT_1_BIT_2D";
	case ND_MALLOC_TGT_1_BIT_3D :
	    return "ND_MALLOC_TGT_1_BIT_3D";
	case ND_MALLOC_TGT_2_BIT_1D :
	    return "ND_MALLOC_TGT_2_BIT_1D";
	case ND_MALLOC_TGT_2_BIT_2D :
	    return "ND_MALLOC_TGT_2_BIT_2D";
	case ND_MALLOC_TGT_2_BIT_3D :
	    return "ND_MALLOC_TGT_2_BIT_3D";
	case ND_MALLOC_TGT_4_BIT_1D :
	    return "ND_MALLOC_TGT_4_BIT_1D";
	case ND_MALLOC_TGT_4_BIT_2D :
	    return "ND_MALLOC_TGT_4_BIT_2D";
	case ND_MALLOC_TGT_4_BIT_3D :
	    return "ND_MALLOC_TGT_4_BIT_3D";
	case ND_MALLOC_TGT_8_BIT_1D :
	    return "ND_MALLOC_TGT_8_BIT_1D";
	case ND_MALLOC_TGT_8_BIT_2D :
	    return "ND_MALLOC_TGT_8_BIT_2D";
	case ND_MALLOC_TGT_8_BIT_3D :
	    return "ND_MALLOC_TGT_8_BIT_3D";
	case ND_MALLOC_TGT_16_BIT_1D :
	    return "ND_MALLOC_TGT_16_BIT_1D";
	case ND_MALLOC_TGT_16_BIT_2D :
	    return "ND_MALLOC_TGT_16_BIT_2D";
	case ND_MALLOC_TGT_16_BIT_3D :
	    return "ND_MALLOC_TGT_16_BIT_3D";
	case ND_MALLOC_TGT_32_BIT_1D :
	    return "ND_MALLOC_TGT_32_BIT_1D";
	case ND_MALLOC_TGT_32_BIT_2D :
	    return "ND_MALLOC_TGT_32_BIT_2D";
	case ND_MALLOC_TGT_32_BIT_3D :
	    return "ND_MALLOC_TGT_32_BIT_3D";
	case ND_TRANSFER_TO_HOST_RET_ARRAY :
	    return "ND_TRANSFER_TO_HOST_RET_ARRAY";
	case ND_TRANSFER_TO_HOST_SCALAR :
	    return "ND_TRANSFER_TO_HOST_SCALAR";
	case ND_BUFFER_TERMINATE :
	    return "ND_BUFFER_TERMINATE";
	case ND_RC_FORALL :
	    return "ND_RC_FORALL";
	case ND_RC_WINDOW_GEN_1D :
	    return "ND_RC_WINDOW_GEN_1D";
	case ND_RC_WINDOW_GEN_2D :
	    return "ND_RC_WINDOW_GEN_2D";
	case ND_RC_SLICE_GEN_2D_COL :
	    return "ND_RC_SLICE_GEN_2D_COL";
	case ND_RC_SLICE_GEN_2D_ROW :
	    return "ND_RC_SLICE_GEN_2D_ROW";
	case ND_RC_ELE_GEN_1D :
	    return "ND_RC_ELE_GEN_1D";
	case ND_RC_ELE_GEN_2D :
	    return "ND_RC_ELE_GEN_2D";
	case ND_RC_ELE_GEN_3D :
	    return "ND_RC_ELE_GEN_3D";
	case ND_WRITE_TILE_1D_1D :
	    return "ND_WRITE_TILE_1D_1D";
	case ND_WRITE_TILE_1D_2D :
	    return "ND_WRITE_TILE_1D_2D";
	case ND_WRITE_TILE_1D_3D :
	    return "ND_WRITE_TILE_1D_3D";
	case ND_WRITE_TILE_2D_1D :
	    return "ND_WRITE_TILE_2D_1D";
	case ND_WRITE_TILE_2D_2D :
	    return "ND_WRITE_TILE_2D_2D";
	case ND_WRITE_TILE_2D_3D :
	    return "ND_WRITE_TILE_2D_3D";
	case ND_WRITE_TILE_3D_1D :
	    return "ND_WRITE_TILE_3D_1D";
	case ND_WRITE_TILE_3D_2D :
	    return "ND_WRITE_TILE_3D_2D";
	case ND_WRITE_TILE_3D_3D :
	    return "ND_WRITE_TILE_3D_3D";
	case ND_REDUCE_ISUM_MACRO :
	    return "ND_REDUCE_ISUM_MACRO";
	case ND_REDUCE_USUM_MACRO :
	    return "ND_REDUCE_USUM_MACRO";
	case ND_REDUCE_IMIN_MACRO :
	    return "ND_REDUCE_IMIN_MACRO";
	case ND_REDUCE_UMIN_MACRO :
	    return "ND_REDUCE_UMIN_MACRO";
	case ND_REDUCE_IMAX_MACRO :
	    return "ND_REDUCE_IMAX_MACRO";
	case ND_REDUCE_UMAX_MACRO :
	    return "ND_REDUCE_UMAX_MACRO";
	case ND_REDUCE_VAL_AT_FIRST_IMIN_MACRO :
	    return "ND_REDUCE_VAL_AT_FIRST_IMIN_MACRO";
	case ND_REDUCE_VAL_AT_FIRST_IMAX_MACRO :
	    return "ND_REDUCE_VAL_AT_FIRST_IMAX_MACRO";
	case ND_REDUCE_VAL_AT_LAST_IMIN_MACRO :
	    return "ND_REDUCE_VAL_AT_LAST_IMIN_MACRO";
	case ND_REDUCE_VAL_AT_LAST_IMAX_MACRO :
	    return "ND_REDUCE_VAL_AT_LAST_IMAX_MACRO";
	case ND_REDUCE_VAL_AT_FIRST_UMIN_MACRO :
	    return "ND_REDUCE_VAL_AT_FIRST_UMIN_MACRO";
	case ND_REDUCE_VAL_AT_FIRST_UMAX_MACRO :
	    return "ND_REDUCE_VAL_AT_FIRST_UMAX_MACRO";
	case ND_REDUCE_VAL_AT_LAST_UMIN_MACRO :
	    return "ND_REDUCE_VAL_AT_LAST_UMIN_MACRO";
	case ND_REDUCE_VAL_AT_LAST_UMAX_MACRO :
	    return "ND_REDUCE_VAL_AT_LAST_UMAX_MACRO";
	case ND_USUM_VALUES :
	    return "ND_USUM_VALUES";
	case ND_ISUM_VALUES :
	    return "ND_ISUM_VALUES";
	case ND_UMIN_VALUES :
	    return "ND_UMIN_VALUES";
	case ND_IMIN_VALUES :
	    return "ND_IMIN_VALUES";
	case ND_UMAX_VALUES :
	    return "ND_UMAX_VALUES";
	case ND_IMAX_VALUES :
	    return "ND_IMAX_VALUES";
	case ND_AND_VALUES :
	    return "ND_AND_VALUES";
	case ND_OR_VALUES :
	    return "ND_OR_VALUES";
	case ND_HIST_VALUES :
	    return "ND_HIST_VALUES";
	case ND_ACCUM_USUM_VALUES :
	    return "ND_ACCUM_USUM_VALUES";
	case ND_ACCUM_ISUM_VALUES :
	    return "ND_ACCUM_ISUM_VALUES";
	case ND_ACCUM_UMAX_VALUES :
	    return "ND_ACCUM_UMAX_VALUES";
	case ND_ACCUM_IMAX_VALUES :
	    return "ND_ACCUM_IMAX_VALUES";
	case ND_ACCUM_UMIN_VALUES :
	    return "ND_ACCUM_UMIN_VALUES";
	case ND_ACCUM_IMIN_VALUES :
	    return "ND_ACCUM_IMIN_VALUES";
	case ND_ACCUM_HIST_VALUES :
	    return "ND_ACCUM_HIST_VALUES";
	case ND_ACCUM_AND_VALUES :
	    return "ND_ACCUM_AND_VALUES";
	case ND_ACCUM_OR_VALUES :
	    return "ND_ACCUM_OR_VALUES";
	case ND_IADD :
	    return "ND_IADD";
	case ND_UADD :
	    return "ND_UADD";
	case ND_ISUB :
	    return "ND_ISUB";
	case ND_USUB :
	    return "ND_USUB";
	case ND_IMUL :
	    return "ND_IMUL";
	case ND_UMUL :
	    return "ND_UMUL";
	case ND_ULT :
	    return "ND_ULT";
	case ND_ILT :
	    return "ND_ILT";
	case ND_ULE :
	    return "ND_ULE";
	case ND_ILE :
	    return "ND_ILE";
	case ND_UGT :
	    return "ND_UGT";
	case ND_IGT :
	    return "ND_IGT";
	case ND_UGE :
	    return "ND_UGE";
	case ND_IGE :
	    return "ND_IGE";
	case ND_UEQ :
	    return "ND_UEQ";
	case ND_IEQ :
	    return "ND_IEQ";
	case ND_UNEQ :
	    return "ND_UNEQ";
	case ND_INEQ :
	    return "ND_INEQ";
	case ND_BIT_COMPL :
	    return "ND_BIT_COMPL";
	case ND_CHANGE_WIDTH :
	    return "ND_CHANGE_WIDTH";
	case ND_CHANGE_WIDTH_SE :
	    return "ND_CHANGE_WIDTH_SE";
	case ND_RC_SELECTOR :
	    return "ND_RC_SELECTOR";
	case ND_ARRAYREF_1D :
	    return "ND_ARRAYREF_1D";
	case ND_ARRAYREF_2D :
	    return "ND_ARRAYREF_2D";
	case ND_ARRAYREF_3D :
	    return "ND_ARRAYREF_3D";
	case ND_ARRAYREF_4D :
	    return "ND_ARRAYREF_4D";
	case ND_ARRAYREF_5D :
	    return "ND_ARRAYREF_5D";
	case ND_ARRAYREF_6D :
	    return "ND_ARRAYREF_6D";
	case ND_ARRAYREF_7D :
	    return "ND_ARRAYREF_7D";
	case ND_ARRAYREF_8D :
	    return "ND_ARRAYREF_8D";
	case ND_CIRCULATE :
	    return "ND_CIRCULATE";
	case ND_WRITE_SCALAR :
	    return "ND_WRITE_SCALAR";
	case ND_WRITE_VALS_TO_BUFFER :
	    return "ND_WRITE_VALS_TO_BUFFER";
	default :
            break;
	}
    return NULL;
    }

void show_ddcf_node (FuncGraph *fg, int id, int *node_assocs, FILE *fp)
    {
    Edge *tt;
    int i;

    if (fg->nodes[id].nodetype == ND_VOIDED)
        return;

    fprintf (fp, "  %d %s", node_assocs[id], ddcf_node_str (fg->nodes[id].nodetype));
    switch (fg->nodes[id].nodetype)
        {
	case ND_NEXT :
	    fprintf (fp, "  (back to node %d)", node_assocs[fg->nodes[id].In_next_id]);
	    break;
	case ND_FCALL :
	case ND_VHDL_CALL :
            fprintf (fp, "  \"%s\"", fg->nodes[id].Sym);
	    break;
	case ND_INTRINCALL :
	    fprintf (fp, "  \"%s\"", intrin_str (fg->nodes[id].Intrin_func));
	    break;
	case ND_ARRAYREF :
	case ND_ELE_GEN :
	case ND_SLICE_GEN:
	    {
	    int rank = fg->nodes[id].inputs[0].ty.dims;
	    fprintf (fp, "  [");
	    for (i=0; i<rank; i++)
		{
		fprintf (fp, "%c", fg->nodes[id].Reftypes[i]);

		if (i+1 < rank)
		    fprintf (fp, ",");
		}
	    fprintf (fp, "]");
	    }
	    break;
	case ND_ARR_DEF :
	    {
	    int rank = fg->nodes[id].DefRank;
	    fprintf (fp, "  [");
	    for (i=0; i<rank; i++)
		{
		fprintf (fp, "%d", fg->nodes[id].DefDims[i]);

		if (i+1 < rank)
		    fprintf (fp, ",");
		}
	    fprintf (fp, "]");
	    }
	    break;
	case ND_SCALAR_GEN :
	    fprintf (fp, "  rank %d", fg->nodes[id].Rank);
	    break;
	case ND_REDUCE_VAL_AT_FIRST_MAX_MACRO :
	case ND_REDUCE_VAL_AT_FIRST_MIN_MACRO :
	case ND_REDUCE_VAL_AT_LAST_MAX_MACRO :
	case ND_REDUCE_VAL_AT_LAST_MIN_MACRO :
	case ND_REDUCE_VAL_AT_MAXS_MACRO :
	case ND_REDUCE_VAL_AT_MINS_MACRO :
	case ND_REDUCE_VAL_AT_FIRST_MAX :
	case ND_REDUCE_VAL_AT_FIRST_MIN :
	case ND_REDUCE_VAL_AT_LAST_MAX :
	case ND_REDUCE_VAL_AT_LAST_MIN :
	case ND_REDUCE_VAL_AT_MAXS :
	case ND_REDUCE_VAL_AT_MINS :
	    fprintf (fp, "  vecsize %d", fg->nodes[id].VecSize);
	    break;
	case ND_G_INPUT :
	case ND_G_INPUT_NEXT :
	    fprintf (fp, "  (input %d for graph node %d)", fg->nodes[id].Io_num, node_assocs[fg->nodes[id].My_graph]);
	    break;
	case ND_G_OUTPUT :
	    fprintf (fp, "  (output %d for graph node %d)", fg->nodes[id].Io_num, node_assocs[fg->nodes[id].My_graph]);
	    break;
	case ND_FUNC :
	case ND_FORALL :
	case ND_FORNXT :
	case ND_WHILE :
	case ND_WHILE_PRED :
	case ND_SWITCH :
	case ND_CASE :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	case ND_GRAPH :
	case ND_RC_COMPUTE :
	case ND_RC_FORALL :
	    fprintf (fp, " (my nodes: ");
	    show_intlist_assocs (&(fg->nodes[id].My_nodes), node_assocs, fp);
	    fprintf (fp, ")");
	    break;
	default :
            break;
	}

    switch (fg->nodes[id].nodetype)
        {
	case ND_FORALL :
	case ND_FORNXT :
	case ND_RC_FORALL :
	    fprintf (fp, " extents ");
	    print_dims (fg->nodes[id].dim_sizes, fp);
	    break;
	default :
            break;
	}

    fprintf (fp, " <\"%s\", \"%s\", %d>\n",
                fg->nodes[id].loc.file, fg->nodes[id].loc.func, fg->nodes[id].loc.line);

    if (fg->nodes[id].pragmas != NULL)
        {
	PragCell *pc;
	int i;

	fprintf (fp, "    pragmas (");

	for (pc=fg->nodes[id].pragmas; pc!=NULL; pc=pc->link)
	    {
	    switch (pc->prag)
	        {
		case PRAG_NO_INLINE :
		    fprintf (fp, "no_inline");
		    break;
		case PRAG_NO_UNROLL :
		    fprintf (fp, "no_unroll");
		    break;
		case PRAG_NO_DFG :
		    fprintf (fp, "no_dfg");
		    break;
		case PRAG_LOOKUP :
		    fprintf (fp, "lookup");
		    break;
		case PRAG_STRIPMINE :
		    fprintf (fp, "stripmine (");
		    for (i=0; i<MAXRANK; i++)
			{
		        fprintf (fp, "%d", pc->vals[i]);
			if (i<MAXRANK-1 && pc->vals[i+1]>=0)
			    fprintf (fp, ",");
			else
			    break;
			}
		    fprintf (fp, ")");
		    break;
		case PRAG_BLOCK :
		    fprintf (fp, "block (");
		    for (i=0; i<MAXRANK; i++)
			{
		        fprintf (fp, "%d", pc->vals[i]);
			if (i<MAXRANK-1 && pc->vals[i+1]>=0)
			    fprintf (fp, ",");
			else
			    break;
			}
		    fprintf (fp, ")");
		    break;
		case PRAG_NO_FUSE :
		    fprintf (fp, "no_fuse");
		    break;
		case PRAG_VHDL :
		    fprintf (fp, "vhdl \"%s\"", pc->str);
		    break;
		}
	    
	    if (pc->link != NULL)
	        fprintf (fp, ", ");
	    }
	
	fprintf (fp, ")\n");
	}

    fprintf (fp, "    %d inputs:", fg->nodes[id].num_inputs);
    switch (fg->nodes[id].nodetype)
        {
	case ND_FUNC :
	case ND_FORALL :
	case ND_FORNXT :
	case ND_WHILE :
	case ND_WHILE_PRED :
	case ND_SWITCH :
	case ND_CASE :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	case ND_GRAPH :
	case ND_RC_COMPUTE :
	case ND_RC_FORALL :
	    fprintf (fp, " (nodes ");
	    for (i=0; i<fg->nodes[id].num_inputs; i++)
	        fprintf (fp, " %d", node_assocs[fg->nodes[id].My_inputs[i]]);
	    fprintf (fp, ")");
	    break;
	default :
            break;
	}
    fprintf (fp, "\n");

    for (i=0; i<fg->nodes[id].num_inputs; i++)
	{
	fprintf (fp, "      port %d <", i);
	print_a_type (&(fg->nodes[id].inputs[i].ty), fp);
	fprintf (fp, ">");
	if (fg->nodes[id].inputs[i].is_const)
	    fprintf (fp, "  value \"%s\"", fg->nodes[id].inputs[i].constval);
#ifdef SHOW_MAXVALS
	fprintf (fp, " ## maxval = 0x%x ##", fg->nodes[id].inputs[i].maxval);
#endif

	fprintf (fp, "\n");
	}
    fprintf (fp, "    %d outputs:", fg->nodes[id].num_outputs);

    switch (fg->nodes[id].nodetype)
        {
	case ND_FUNC :
	case ND_FORALL :
	case ND_FORNXT :
	case ND_WHILE :
	case ND_WHILE_PRED :
	case ND_SWITCH :
	case ND_CASE :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	case ND_GRAPH :
	case ND_RC_COMPUTE :
	case ND_RC_FORALL :
	    fprintf (fp, " (nodes ");
	    for (i=0; i<fg->nodes[id].num_outputs; i++)
	        fprintf (fp, " %d", node_assocs[fg->nodes[id].My_outputs[i]]);
	    fprintf (fp, ")");
	    break;
	default :
            break;
	}
    fprintf (fp, "\n");

    for (i=0; i<fg->nodes[id].num_outputs; i++)
	{
	fprintf (fp, "      port %d <", i);
	print_a_type (&(fg->nodes[id].outputs[i].ty), fp);
	fprintf (fp, "> ");
	for (tt=fg->nodes[id].outputs[i].targets; tt!=NULL; tt=tt->link)
	    fprintf (fp, "  %d.%d", node_assocs[tt->node], tt->port);
#ifdef SHOW_MAXVALS
	fprintf (fp, " ## maxval = 0x%x ##", fg->nodes[id].outputs[i].maxval);
#endif
	fprintf (fp, "\n");
	}
    fprintf (fp, "    ;\n");
    fflush (fp);
    }

void show_ddcf_func (FuncGraph *fg, FILE *fp, int no_voids)
    {
    TypeInfo *tt;
    int i, *node_assocs;

    SacMalloc (node_assocs, int*, fg->nodes_used * sizeof (int))
    for (i=0; i<fg->nodes_used; i++)
        node_assocs[i] = -1;

    assign_assocs (fg, node_assocs, no_voids);

    fprintf (fp, "function \"%s\"\n", fg->name);
    fprintf (fp, "  param types : ");
    for (tt=fg->params; tt!=NULL; tt=tt->link)
        {
	print_a_type (tt, fp);
	if (tt->link != NULL)
	    fprintf (fp, ", ");
	}
    fprintf (fp, "\n");
    fprintf (fp, "  return types: ");
    for (tt=fg->rets; tt!=NULL; tt=tt->link)
        {
	print_a_type (tt, fp);
	if (tt->link != NULL)
	    fprintf (fp, ", ");
	}
    fprintf (fp, "\n");
    for (i=0; i<fg->nodes_used; i++)
        show_ddcf_node (fg, i, node_assocs, fp);

    SacFree (node_assocs)
    }

void assign_assocs (FuncGraph *fg, int *node_assocs, int no_voids)
    {
    int i, j;

    for (i=0, j=0; i<fg->nodes_used; i++)
	if (no_voids)
	    {
            if (fg->nodes[i].nodetype != ND_VOIDED)
	        {
	        /*
	        printf ("  assoc %d to %d\n", i, j);
	        */
	        node_assocs[i] = j++;
	        }
	    }
	else
	    node_assocs[i] = j++;
    }

void print_dims (int *szs, FILE *fp)
    {
    int i;

    fprintf (fp, "[");
    for (i=0; i<MAXRANK; i++)
        {
        if ((szs)[i] < 0)
            fprintf (fp, ":");
        else
            fprintf (fp, "%d", (szs)[i]);

        if (i < 7)
            fprintf (fp, ",");
        }
    fprintf (fp, "]");
    }

void show_intlist_assocs (IntList **list, int *node_assocs, FILE *fp)
    {
    IntList *tmp;

    for (tmp=*list; tmp!=NULL; tmp=tmp->link)
        fprintf (fp, " %d", node_assocs[tmp->val]);
    }
