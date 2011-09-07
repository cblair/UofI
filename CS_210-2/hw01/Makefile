tree: main.o lex.yy.o
	gcc -o tree main.o lex.yy.o -ll

main.o: main.c
	gcc -c -g main.c

lex.yy.o: lex.yy.c
	gcc -c -g lex.yy.c

lex.yy.c: newick.l
	flex newick.l

clean:
	rm *.o; rm lex.yy.c