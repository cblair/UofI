## server.c
 #
 # CS 270.Rinker.........GNU Version 2...........Colby Blair
 # 11/19/09 ...........Dell Inspiron 1521........cblair@vandals.uidaho.edu
 # Assignment 06

 ###################SA-C Makefile####################
#This is the make file for the SA-C program



MAINOBJS = scan.o gram.o error.o const_fold_tree.o scoping.o symtab.o deftab.o type_pass1.o type_pass2.o type_pass3.o flow.o collect_decls.o build_ddcf.o tree.o array_casts.o code_motion.o ddcf_consistency.o func_inline.o lookup_tables.o window_tile.o ddcf2dfg.o show_ddcf.o switch_const.o size_propagation.o array_const_prop.o const_fold.o identities.o strength_reduction.o dead_code.o full_unroll.o cse.o arrayref_elision.o loop_fuse.o window_to_elegen.o duped_inputs.o convert_concat_masked.o intlist.o ddcf.o copy_graph.o type_unify.o num_converions.o show_tree.o bitwidth_narrow.o size_prop_down.o size_prop_up.c binary_read.o

main: main.c sassy.h node.h deftab.h ddcf.h error.h $(MAINOBJS)
	gcc -lm -o main main.c $(MAINOBJS)

####################Main Dependencies

scan.o: lex.yy.o gram.tab.h gram.tab.c
	gcc -c lex.yy.c -o scan.o

gram.o: gram.tab.h gram.tab.c
	gcc -c gram.tab.c -o gram.o 

lex.yy.c: scan.l
	flex scan.l 

gram.tab.h: gram.y
	bison -d gram.y 

error.o: error.c sassy.h node.h error.h
	gcc -c error.c 

const_fold_tree.o: const_fold_tree.c sassy.h node.h deftab.h error.h
	gcc -c const_fold_tree.c 

scoping.o: scoping.c sassy.h node.h deftab.h error.h
	gcc -c scoping.c 

symtab.o: symtab.c sassy.h error.h
	gcc -c symtab.c 

deftab.o: deftab.c sassy.h node.h deftab.h error.h
	gcc -c deftab.c 

type_pass1.o: type_pass1.c sassy.h node.h deftab.h error.h
	gcc -c type_pass1.c 

type_pass2.o: type_pass2.c sassy.h node.h deftab.h error.h
	gcc -c type_pass2.c 

type_pass3.o: type_pass3.c sassy.h node.h deftab.h error.h
	gcc -c type_pass3.c 

flow.o: flow.c sassy.h node.h deftab.h error.h
	gcc -c flow.c

collect_decls.o: collect_decls.c sassy.h node.h deftab.h error.h
	gcc -c collect_decls.c

build_ddcf.o: build_ddcf.c sassy.h node.h deftab.h ddcf.h error.h
	gcc -c build_ddcf.c

tree.o: tree.c sassy.h node.h error.h
	gcc -c tree.c

array_casts.o: array_casts.c sassy.h ddcf.h error.h
	gcc -c array_casts.c

code_motion.o: code_motion.c sassy.h ddcf.h error.h
	gcc -c code_motion.c

ddcf_consistency.o: ddcf_consistency.c sassy.h ddcf.h error.h 
	gcc -c ddcf_consistency.c

func_inline.o: func_inline.c sassy.h ddcf.h error.h 
	gcc -c func_inline.c

lookup_tables.o: lookup_tables.c sassy.h ddcf.h error.h 
	gcc -c lookup_tables.c

window_tile.o: window_tile.c sassy.h ddcf.h error.h 
	gcc -c window_tile.c

ddcf2dfg.o: ddcf2dfg.c sassy.h ddcf.h error.h 
	gcc -c ddcf2dfg.c

show_ddcf.o: show_ddcf.c sassy.h ddcf.h error.h 
	gcc -c show_ddcf.c

switch_const.o: switch_const.c sassy.h ddcf.h error.h 
	gcc -c switch_const.c

size_propagation.o: size_propagation.c sassy.h ddcf.h error.h 
	gcc -c size_propagation.c

array_const_prop.o: array_const_prop.c sassy.h ddcf.h error.h 
	gcc -c array_const_prop.c

const_fold.o: const_fold.c sassy.h ddcf.h error.h 
	gcc -c const_fold.c

identities.o: identities.c sassy.h ddcf.h error.h 
	gcc -c identities.c

strength_reduction.o: strength_reduction.c sassy.h ddcf.h error.h 
	gcc -c strength_reduction.c
 
dead_code.o: dead_code.c sassy.h ddcf.h error.h 
	gcc -c dead_code.c

full_unroll.o: full_unroll.c sassy.h ddcf.h error.h 
	gcc -c full_unroll.c

cse.o: cse.c sassy.h ddcf.h error.h 
	gcc -c cse.c

arrayref_elision.o: arrayref_elision.c sassy.h ddcf.h error.h 
	gcc -c arrayref_elision.c 

loop_fuse.o: loop_fuse.c sassy.h ddcf.h error.h 
	gcc -c loop_fuse.c

window_to_elegen.o: window_to_elegen.c sassy.h ddcf.h error.h 
	gcc -c window_to_elegen.c

duped_inputs.o: duped_inputs.c sassy.h ddcf.h error.h 
	gcc -c duped_inputs.c

convert_concat_masked.o: convert_concat_masked.c sassy.h ddcf.h error.h 
	gcc -c convert_concat_masked.c

intlist.o: intlist.c sassy.h error.h
	gcc -c intlist.c

ddcf.o: ddcf.c sassy.h ddcf.h error.h 
	gcc -c ddcf.c

copy_graph.o: copy_graph.c sassy.h ddcf.h error.h 
	gcc -c copy_graph.c

type_unify.o: type_unify.c sassy.h node.h deftab.h error.h 
	gcc -c type_unify.c

num_converions.o: num_converions.c sassy.h
	gcc -c num_converions.c

show_tree.o: show_tree.c sassy.h node.h
	gcc -c show_tree.c

bitwidth_narrow.o: bitwidth_narrow.c sassy.h ddcf.h error.h 
	gcc -c bitwidth_narrow.c

size_prop_down.o: size_prop_down.c sassy.h ddcf.h error.h 
	gcc -c size_prop_down.c

size_prop_up.o: size_prop_up.o sassy.h ddcf.h error.h 
	gcc -c size_prop_up.c

binary_read.o: binary_read.c sassy.h ddcf.h error.h 
	gcc -c binary_read.c

clean:
	rm *.o