PARSER=tree
CC=g++
YACC=bison
LEX=flex
CCFLAGS=-g
CFLAGS=-g

$(PARSER): $(PARSER).tab.o lex.yy.o
	$(CC) $(CCFLAGS)  lex.yy.o $(PARSER).tab.o  -lfl -o $(PARSER)

$(PARSER).tab.c $(PARSER).tab.h: $(PARSER).y
	$(YACC) -d -t $(PARSER).y

lex.yy.c: $(PARSER).l $(PARSER).tab.h
	$(LEX) $(PARSER).l

clean:
	rm -f *.tab.h *.tab.c *.output lex.yy.c *.o *~ core '#'*

tar:
	tar -cvf $(PARSER).tar  $(PARSER).y  $(PARSER).l  makefile treeClass.cpp treeClass.h data

