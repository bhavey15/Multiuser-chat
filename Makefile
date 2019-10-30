
objects = client.o server.o
edit : $(objects)
	gcc -o -lpthread edit $(objects)
client.o : client.c
	gcc -c -lpthread client.c
server.o : server.c
	gcc -c -lpthread server.c 
clean : 
	rm edit $(objects)
