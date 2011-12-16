COPTS = -o32
all:
	cc -c $(COPTS) cccp.c -o cccp.o
	cc -c $(COPTS) alloca.c -o alloca.o
	bison cexp.y
	cc -c $(COPTS) cexp.tab.c -o cexp.tab.o
	cc -c $(COPTS) obstack.c -o obstack.o
	cc -c $(COPTS) version.c -o version.o
	ar q libacpp.a cccp.o alloca.o cexp.tab.o obstack.o version.o

clean:
	rm libacpp.a cccp.o alloca.o cexp.tab.o obstack.o version.o
