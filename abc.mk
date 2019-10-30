cc = gcc
all : client server

server: 
	$(cc) -o serv server.c
client: 
	$(cc) -o clien client.c
