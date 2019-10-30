# Multiuser-chat
Server:
1. We establish the connection with the port using socket() and get the socket file descriptor. "0" in the argument specifies TCP.
2. bind function binds the socket to the address and port number specified.
3. Using listen(), we wait for the client to approach the server to make a connection.
4. After returning a new file descriptor using accept(), the client and server are ready to communicate.
5. Using client array, we keep the track of all the file descriptors of the socket to which different clients are connected.
6. A thread is created to receive what is being sent by the client and broadcast it to all the clients or send it to the specified client.
7. If there is any data in the recv_buf, it can be received by using recv().
8. Data is sent to all other clients using write().

Client: 
1. We establish the connection with the server running on the local host.
2. This connection is established using connect().
3. We create a thread to receive what is being sent by the server. pthread_create() is used to create the thread.If any data is present in recv_buf, it is received using recv().
4. The data received is being printed on the client screen.
5. The client sends the data to the server using write(). The message is taken as an input by stdin.

Compilation:
Compile against the Makefile using command "make".

Input: 
While running the client.c, pass the name of the client as the command line argument.

Output: 
Server screen displays whether the setup was successful or not. Whenever a new client is connected to the server, it displays the socket file descriptor on the screen respective to that client.
Client screen displays the message which is sent by other clients via server. Client may type the message on the screen to be sent to other clients.


