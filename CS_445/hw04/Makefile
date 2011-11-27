#############################################################################
#Class:		CS 445
#Semester:	Fall 2011
#Assignment:	Homework 4
#Author:	Colby Blair
#File name:	Makefile
#############################################################################

PROC=ec
CC=gcc
#PARSER=ecma1
PARSER=AS3
LEX=flex
YACC=bison
CFLAGS=-g -pg -Wno-write-strings
#CFLAGS=-g -pg -Wno-write-strings -DYYDEBUG=1
#CFLAGS=-g -pg -Wno-write-strings -DDEBUG_TREE -DDEBUG_SYMTAB
OBJS=parser.tab.o $(PARSER).yy.o main.o tree.o symtabc.o tree_symtab_insert.o
OBJS+=tree_symtab_gen.o


all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lfl -o $(PROC)

main.o: main.c  
	$(CC) $(CFLAGS) main.c -c 

tokenlist.o: tokenlist.c 
	$(CC) $(CFLAGS) tokenlist.c -c 

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) tree.c -c

symtabc.o: symtabc.c symtabc.h
	$(CC) $(CFLAGS) symtabc.c -c

tree_symtab_insert.o: tree_symtab_insert.c tree_symtab_insert.h
	$(CC) $(CFLAGS) tree_symtab_insert.c -c

tree_symtab_gen.o: tree_symtab_gen.c tree_symtab_gen.h
	$(CC) $(CFLAGS) tree_symtab_gen.c -c

parser.tab.o: parser.tab.c  
	$(CC) $(CFLAGS) parser.tab.c -c 

parser.tab.c parser.tab.h: $(PARSER).y
	$(YACC) -g -v --debug -d -b parser -t $(PARSER).y

$(PARSER).yy.o: $(PARSER).l parser.tab.h
	$(CC) $(CFLAGS) lex.yy.c -c -o $(PARSER).yy.o 

$(PARSER).l: parser.tab.h
	$(LEX) $(PARSER).l

turnin:
	tar cvf hw4.tar Makefile *.c *.h *.y *.l

clean:
	rm -f $(PROC) *.tab.h *.tab.c *.output lex.yy.c *.o *~ core gmon.out '#'* *.output hw*.tar
