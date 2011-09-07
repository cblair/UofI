all: client.o server.o
	gcc -o client client.o ; gcc -o server server.o

client.o: client.c cbstrings.h
	gcc -c client.c 

server.o: server.c cbstrings.h
	gcc -c server.c
clean: 
	rm *.o

uninstall:
	rm client ; rm server