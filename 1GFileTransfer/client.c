#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dbtime.h"

#define PORT 8001

int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr;

  // create a socket
  int clientfd = socket(AF_INET,SOCK_STREAM,0);
  if(clientfd<0)
  {
    printf("create socket failed!\n");
    exit(1);
  }
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //connet socket 
  
  if(connect(clientfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
  {
     printf("server accept failed\n");
     exit(1);
  }  
  FILE *fp=fopen("recv1G.txt","w+");
  char recvBuffer[2000]={0};

  //timing

dbtime_startTest ("timing");
  while(1)
  {
    //char sendBuffer[1000]="Hi";
	
    int rlen=recv(clientfd,recvBuffer,sizeof(recvBuffer),0);
	if(rlen<=0)
	{
		printf("finish!\n");
		break;
	}
	fputs(recvBuffer,fp);
//	printf("recv data: %s\n",recvBuffer);
	memset(recvBuffer,0,sizeof(recvBuffer));

//	send(clientfd,sendBuffer,strlen(sendBuffer),0);    
  }
dbtime_endAndShow ();

  close(clientfd);  
  fclose(fp);
  return 0;
}
