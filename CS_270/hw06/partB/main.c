#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "ddcf.h"
#include "error.h"
#include "math.h"

#define MAX_OPT_ITERS 100

YYSTYPE yylval;

/*
#define PRINT(x) {if (!quiet) printf (x), fflush (stdout);}
*/
#define PRINT(x)

void cyclic_opts ();

NODEPNT prog_tree;
StringList *export_list = NULL;

int quiet = FALSE;
int module = FALSE;
int rename_main = FALSE;
int use_dope_vectors = TRUE;
int optimize = FALSE;
int gen_dfg = FALSE;
int bit_width_opt = FALSE;
int buf_size_in_words = 1024;

int is_stable;

int main (int argc, char *argv[])
    {
    FuncGraph *fg;
    char becomes_main[128];
    char fname[256];
    char ddcf_file[256];
    FILE *ddcf_fp;
    int i, len;

    for (i=2; i<argc; i++)
	{
	if (strcmp (argv[i], "-quiet") == 0)
	    quiet = TRUE;
	else if (strcmp (argv[i], "-module") == 0)
	    module = TRUE;
	else if (strcmp (argv[i], "-m") == 0)
	    rename_main = TRUE;
	else if (strcmp (argv[i], "-ndv") == 0)
	    use_dope_vectors = FALSE;
	else if (strcmp (argv[i], "-dfg") == 0)
	    gen_dfg = TRUE;
	else if (strcmp (argv[i], "-bw") == 0)
	    bit_width_opt = TRUE;
	else if (strcmp (argv[i], "-O") == 0)
	    optimize = TRUE;
	else if (strcmp (argv[i], "-buf") == 0)
	    {
	    i++;

	    if (i >= argc)
	        handle_error (ErrMainBufSizeError, Deadly, 0, NULL, NULL);

	    if (sscanf (argv[i], "%d", &buf_size_in_words) < 1)
	        handle_error (ErrMainBufSizeError, Deadly, 0, NULL, NULL);
	    }
	}

    if ((argc < 2) || ((yyin = fopen (argv[1], "r")) == NULL))
	handle_error (ErrMainOpenSource, Deadly, 0, NULL, NULL);

    strcpy (fname, argv[1]);

    len = strlen (fname);
    if (strcmp (fname+len-4, ".cpp") == 0)
        fname[len-4] = '\0';

    strcpy (filename, fname);

    PRINT ("parsing...\n")
    if (yyparse () == 1)
	exit (ExitParse);

    if (global_error)
        exit (ExitParse);

    PRINT ("const_fold_tree...\n")
    const_fold_tree (prog_tree);

    /*
    show_tree (prog_tree);
    exit (1);
    */

    PRINT ("scoping...\n")
    scoping (&prog_tree);

    if (global_error)
        exit (ExitScope);

    /* if we have '-m' option, we will rename a func to "main";
     * we want to be sure that there isn't already a "main";
     */
    if (rename_main)
        {
	SYM_ENTRY *sm, *snew;

        for (i=0; i<128; i++)
            if (argv[1][i] == '.')
	        break;
	    else
	        becomes_main[i] = argv[1][i];
        becomes_main[i] = '\0';

	sm = handle_ident ("main");

	/* see if we already have a "main"; if so, it's an error; */
	for (i=0; i<defcnt; i++)
	    if ((deftable[i].tinfo.kind==Func) && (deftable[i].symval == sm->val))
		handle_error (ErrMainMainDashM, Deadly, 0, NULL, NULL);

	snew = handle_ident (becomes_main);

	/* find the top-level function and rename it "main" */
	for (i=0; i<defcnt; i++)
	    if ((deftable[i].tinfo.kind==Func) && (deftable[i].symval == snew->val))
		{
		deftable[i].symval = sm->val;
		break;
		}

	/* if we didn't find the top-level function, report the error */
	if (i == defcnt)
	    handle_error (ErrMainMissingDashM, Deadly, 0, NULL, NULL, snew->val);
        }

    PRINT ("type_pass1...\n")
    type_pass1 (prog_tree);

    if (global_error)
        exit (ExitTypePass1);

    PRINT ("type_pass2...\n")
    type_pass2 (prog_tree);

    if (global_error)
        exit (ExitTypePass1);

    PRINT ("type_pass3...\n")
    type_pass3 (prog_tree);

    if (global_error)
        exit (ExitTypePass1);

    PRINT ("flow...\n")
    flow (prog_tree);

    if (global_error)
        exit (ExitFlow);

    PRINT ("collect_decls...\n")
    collect_decls (prog_tree);

    PRINT ("build_ddcf...\n")
    build_ddcf (prog_tree);

    PRINT ("free_tree...\n")
    free_tree (prog_tree);
    prog_tree = NULL;

    PRINT ("push_array_cast...\n")
    push_array_cast ();

    PRINT ("code_motion...\n")
    code_motion ();

    PRINT ("ddcf_consistency...\n")
    ddcf_consistency ();

    if (optimize)
	{
	/* first we inline all but lookup functions */
        PRINT ("func_inline...\n")
        func_inline ();

        PRINT ("lookup_tables...\n")
	lookup_tables ();

	/* after lookup functions have been converted, inline them */
        PRINT ("func_inline...\n")
        func_inline ();

	/* we do this now to get any loop fusions to take place before window tilings */
	PRINT ("cyclic_opts...\n");
	cyclic_opts ();

	/* do window tiling twice, once to pick up 'block' and once to pick up 'stripmine' */
    	PRINT ("window_tile...\n")
	window_tile ();

    	PRINT ("window_tile...\n")
	window_tile ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

	PRINT ("cyclic_opts...\n");
	cyclic_opts ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();
	}

    PRINT ("ddcf_consistency...\n")
    ddcf_consistency ();

    if (gen_dfg)
        {
	PRINT ("ddcf2dfg...\n");
	ddcf2dfg ();
	}

    PRINT ("ddcf_consistency...\n")
    ddcf_consistency ();

DUMP:
    PRINT ("dumping ddcf...\n");
    sprintf (ddcf_file, "%s.ddcf", fname);

    if ((ddcf_fp = fopen (ddcf_file, "w")) == NULL)
	handle_error (ErrMainOpenDdcfDump, Deadly, 0, NULL, NULL);

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
	{
	show_ddcf_func (fg, ddcf_fp, TRUE);
	/*
	show_ddcf_func (fg, ddcf_fp, FALSE);
	*/
	}

    fclose (ddcf_fp);

    PRINT ("done\n");
    /*
    show_tree (prog_tree);
    show_deftable ();
    */
    exit (0);
    }

void cyclic_opts ()
    {
    int i;

    for (i=0; i<MAX_OPT_ITERS; i++)
        {
	is_stable = TRUE;

        PRINT ("code_motion...\n")
        code_motion ();

        PRINT ("push_array_cast...\n")
	push_array_cast ();

        PRINT ("switch_const...\n")
        switch_const ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("size_propagate...\n")
        size_propagate ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("array_const_prop...\n");
        array_const_prop ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("constant_fold...\n");
        constant_fold ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("identities...\n");
        identities ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("strength_reduction...\n");
        strength_reduction ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("dead_code...\n");
        dead_code ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("full_unroll...\n");
        full_unroll ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("cse...\n");
        cse ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("arrayref_elision...\n");
        arrayref_elision ();

    	PRINT ("loop_fuse...\n")
	loop_fuse ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

    	PRINT ("window_to_elegen...\n")
	window_to_elegen ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("duped_inputs...\n")
        duped_inputs ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

        PRINT ("convert_concat_masked...\n")
        convert_concat_masked ();

    	PRINT ("ddcf_consistency...\n")
    	ddcf_consistency ();

	if (is_stable)
		{
		if (!quiet)
		    printf ("  %d optimization sweeps performed\n", i+1);
	        break;
		}
	}

    if (i == MAX_OPT_ITERS)
	handle_error (ErrMainOptNotStable, Deadly, 0, NULL, NULL);
    }

void add_to_export_list (int sval)
    {
    StringList *tmp;

    SacMalloc (tmp, StringList*, sizeof (StringList))
    tmp->str = find_sym (sval);
    tmp->link = export_list;
    export_list = tmp;
    }
