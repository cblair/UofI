PROC=test
CC=gcc
CFLAGS=-g -pg -Wno-write-strings
OBJS=main.o ga_i.o ga_pop.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROC)

main.o: main.c  
	$(CC) $(CFLAGS) main.c -c 

ga_i.o: ga_i.c ga_i.h
	$(CC) $(CFLAGS) ga_i.c -c 

ga_pop.o: ga_pop.c ga_pop.h
	$(CC) $(CFLAGS) ga_pop.c -c 

clean:
	rm -f $(PROC) *.tab.h *.tab.c *.output lex.yy.c *.o *~ core gmon.out '#'* *.output hw*.tar
