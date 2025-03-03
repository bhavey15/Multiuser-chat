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
      char ch;
      while(i<strlen(msg)-2){
        s_msg[i]=msg[i];
        i++;
      }
      ch=msg[i];	
      s_msg[i]='\0';
      i=0;
      //puts(s_msg);
      //printf("idex = %d\n",idex);
      //printf("%d\n",isdigit(ch));
      if(isdigit(ch)>0){
          int sno=ch - '0';
	  //printf("%d\n", sno);
          if(sno-1>idex)
            printf("sending failed... client not avaliable\n");
          else{
	    //printf("ek ko jayega bhaya\n");
            int status = write(clients[sno-1], s_msg, strlen(s_msg));
            if(status<0){
              printf("sending failed\n");
            }
          }
      }
      else if(ch=='n'){
	//printf("idhar bhi aagaya bhaya... ab sabko jayega\n");
        while(i<idex){
              if(clients[i]!=ssock){
                int status=write(clients[i], s_msg, strlen(s_msg));
                if(status<0){
                  printf("send failed\n");
                  continue;
                }
              }
              i++;
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
  //printf("bind value %d \n", b);
  if(b<0){
    printf("server setup failed \n");
    exit(0);
  }
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
