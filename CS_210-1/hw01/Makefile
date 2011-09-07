iscan: main.o lex.yy.o
	cc -o iscan main.o lex.yy.o -ll

main.o: main.c
	cc -c -g main.c

lex.yy.o: lex.yy.c
	cc -c -g lex.yy.c

lex.yy.c: ivml.l
	flex ivml.l
