all : client server

client : client.c
	gcc -pthread client.c -o client
server : server.c
	gcc -pthread server.c -o server
