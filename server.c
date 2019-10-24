#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
int clients[30];
int index=0;
static pthread_mutex_t mutex;
pthread_t thread;
int main(int argc, char* argv[]){
  int sock= socket(AF_NET, SOCK_STREAM, 0);
  struct sockaddr_in server;
  server.sin_family= AF_NET;
  server.sin_port= htons(1550);
  server.sin_addr.s_addr= htonl(INADDR_ANY);
  int b= bind(sock, (struct sockaddr*)&server, sizeof(server));
  if(b<0)
    printf("server setup failed");
  else
    printf("server setup successfull");
  if(listen(sock, 1024)<0){
    printf("listening failed");
    return 0;
  }
  while(1){
    int c_sock=accept(sock, (struct sockaddr*)NULL, NULL);
    if(c_sock<0)
      printf("failed");
    pthread_mutex_lock(&mutex);
    clients[index]=c_sock;
    index++;
    pthread_create(&thread, NULL, (void *)receive, &c_sock);
    pthread_mutex_unlock(&mutex);
  }
  close(sock);
}

