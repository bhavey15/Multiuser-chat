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
char msg[500];
void* receive(void* args){
  char data[1024];
  int clientsocket= *((int *)args);
  while(1){
    int r_val=recv(clientsocket, data, 1024, 0);
    if(r_val<=0)
      break;
    else{
    data[r_val]='\0';
    puts(data);
    }
  }
}

int main(int argc, char* argv[]){
  int clientsocket= socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sket;
  sket.sin_family = AF_INET;
  sket.sin_addr.s_addr = htonl(INADDR_ANY);
  sket.sin_port = htons(1550);
  char client_name[500];
  strcpy(client_name, argv[1]);
  if(connect(clientsocket, (struct sockaddr*) &sket, sizeof(sket))<0){
    printf("Connection did not establish.. exiting");
    return 0;
  }
  pthread_t thread;
  pthread_create(&thread, NULL, (void *)receive, &clientsocket);
  while(1){
    char s_msg[500];
    int status=fgets(msg, 500, stdin);
    if(status<=0)
	break;
    //scanf("%s", msg);
    strcpy(s_msg, client_name);
    strcat(s_msg, ":");
    strcat(s_msg, msg);
    int s= write(clientsocket, s_msg, strlen(s_msg));
    if(s<0){
      printf("message not sent \n");
    }
  }
  pthread_join(thread, NULL);
  close(clientsocket);
  return 0;
}
