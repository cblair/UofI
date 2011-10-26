PROC=eval
CPP=g++
#CPPFLAGS=-g -pg -Wno-write-strings
#CPPFLAGS=-g -pg -Wno-write-strings -DDEBUG_TREE_GP=1
#CPPFLAGS=-g -pg -Wno-write-strings -DDEBUG=1
CPPFLAGS=-g -pg -Wno-write-strings -DDEBUG_TREE=1
OBJS=tree_gp.o darray.o tree_node.o tree.o main.o test.o

all: $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(PROC)

main.o:	main.cpp
	$(CPP) $(CPPFLAGS) main.cpp -c

tree_node.o: tree_node.cpp tree_node.h
	$(CPP) $(CPPFLAGS) tree_node.cpp -c

tree.o:	tree.cpp tree.h
	$(CPP) $(CPPFLAGS) tree.cpp -c

test.o: test.cpp test.h
	$(CPP) $(CPPFLAGS) test.cpp -c

darray.o: darray.cpp darray.h
	$(CPP) $(CPPFLAGS) darray.cpp -c 

tree_gp.o: tree_gp.cpp tree_gp.h
	$(CPP) $(CPPFLAGS) tree_gp.cpp -c

clean:
	rm $(PROC) *.o gmon.out
