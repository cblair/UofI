CPP=g++
OBJS=node.o map.o

all: $(OBJS)
	$(CPP) -o search main.cpp $(OBJS)

map.o: map.cpp map.h
	$(CPP) -c map.cpp

node.o: node.cpp node.h
	$(CPP) -c node.cpp
