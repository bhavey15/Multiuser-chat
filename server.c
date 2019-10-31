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
int idex=0;
// static pthread_mutex_t mutex;
pthread_t thread;
void *receive(void *args){
  int ssock= (*(int*)args);
  char msg[500];
  while(1){
    int len=recv(ssock,msg, 1024,0);
    if(len<=0)
      return 0;
    else{
      msg[len]='\0';
      int i=0;
      char s_msg[500];
      while(msg[i]!='\0'){
        if(i>0){
          s_msg[i-1]=msg[i];
        }
        i++;
      }
      s_msg[i-1]='\0';
      i=0;
      if(isdigit(msg[0])>0){
          int sno=msg[0] - '0';
          if(sno-1>idex)
            printf("sending failed... client not avaliable\n");
          else if(sno!=-1){
            int status = write(clients[sno-1], s_msg, strlen(s_msg));
            if(status<0){
              printf("sending failed\n");
            }
          }
          else{
            while(i<idex){
              if(clients[i]!=ssock){
                int status=write(clients[i], msg, strlen(msg));
                if(status<0){
                  printf("send failed\n");
                  continue;
                }
              }
              i++;
            }
          }
        }
      else
        printf("cannot send\n");
      } 
    }
}
int main(int argc, char* argv[]){
  int sock= socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server;
  server.sin_family= AF_INET;
  server.sin_port= htons(1550);
  server.sin_addr.s_addr= htonl(INADDR_ANY);
  int b= bind(sock, (struct sockaddr*)&server, sizeof(server));
  printf("bind value %d \n", b);
  if(b<0)
    printf("server setup failed \n");
  else
    printf("server setup successful \n");
  if(listen(sock, 1024)<0){
    printf("listening failed \n");
    return 0;
  }
  while(1){
    int c_sock=accept(sock, (struct sockaddr*)NULL, NULL);
    if(c_sock<0)
      printf("failed\n");
    else
      printf("Socket number -> %d \n",c_sock);
    clients[idex]=c_sock;
    idex++;
    pthread_create(&thread, NULL, (void *)receive, &c_sock);
  }
  return 0;
}
