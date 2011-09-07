#############################################################################
#Class:		CS 445
#Semester:	Fall 2011
#Assignment:	Homework 1
#Author:	Colby Blair
#File name:	Makefile
#############################################################################

PROC=scan
CC=gcc
LEX=flex
CFLAGS=-g -pg -Wno-write-strings
OBJS=yylex.o main.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lfl -o $(PROC)

main.o: main.c 
	$(CC) $(CFLAGS) main.c -c 

#yylex() without flex
yylex.o: yylex.c
	$(CC) $(CFLAGS) yylex.c -c 
	
##use these instead of above yylex.o for using flex
#yylex.o: lex.yy.c
#	$(CC) $(CFLAGS) lex.yy.c -c -o yylex.o 
#
#lex.yy.c: yylex.l
#	$(LEX) yylex.l

clean:
	rm -f $(PROC) *.tab.h *.tab.c *.output lex.yy.c *.o *~ core gmon.out '#'*

turnin:
	tar cf blai4021.tar makefile main.c yylex.c main.h
