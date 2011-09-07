#############################################################################
#Class:		CS 445
#Semester:	Fall 2011
#Assignment:	Homework 2
#Author:	Colby Blair
#File name:	Makefile
#############################################################################

PROC=scan
CC=gcc
PARSER=ecma
LEX=flex
YACC=bison
CFLAGS=-g -pg -Wno-write-strings
OBJS=$(PARSER).tab.o $(PARSER).yy.o main.o tokenlist.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lfl -o $(PROC)

main.o: main.c  
	$(CC) $(CFLAGS) main.c -c 

tokenlist.o: tokenlist.c 
	$(CC) $(CFLAGS) tokenlist.c -c 

$(PARSER).tab.o: $(PARSER).tab.c  
	$(CC) $(CFLAGS) $(PARSER).tab.c -c 

$(PARSER).tab.c $(PARSER).tab.h: $(PARSER).y
	$(YACC) -d -t $(PARSER).y

$(PARSER).yy.o: $(PARSER).l $(PARSER).tab.h
	$(CC) $(CFLAGS) lex.yy.c -c -o $(PARSER).yy.o 

$(PARSER).l: $(PARSER).tab.h
	$(LEX) $(PARSER).l

clean:
	rm -f $(PROC) *.tab.h *.tab.c *.output lex.yy.c *.o *~ core gmon.out '#'*

turnin:
	tar cf blai4021.tar makefile main.c yylex.c main.h
