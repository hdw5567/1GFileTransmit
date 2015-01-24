#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORT 8001

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	struct sockaddr_in clientaddr;
	
	//create socker
	int sockfd=socket(PF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		printf("socket created failed!\n");
		exit(1);
	}
	bzero(&servaddr,sizeof(servaddr));	
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(PORT);	
	
	// bind socket
	int bind_flag=bind(sockfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr));
	if(bind_flag<0)
	{
		printf("bind failed!\n");
		exit(1);
	}	
	
	if(listen(sockfd,20)<0)
	{
		printf("listen failed!\n");
		exit(1);
	}
	printf("listen succeed!\n");
	
	int len=sizeof(clientaddr);
	int clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
	if(clientfd<0)
	{
		printf("accept failed!\n");
		exit(1);
	}
	printf("accept succeed!\n");	


	printf("please input filename\n");
	char name[30]={0};
	scanf("%s",name);

	FILE *fp=fopen(name,"r");
	fseek(fp,0L,SEEK_SET);
		char sendBuffer[2000]={0};
	//	while(feof(fp)==0)
	while(fgets(sendBuffer,1025,fp)>0)
	{
		//char recvBuffer[2000]={0};
	
		//int dataLen=recv(clientfd,recvBuffer,sizeof(recvBuffer),0);
		//int glen=fread(sendBuffer,1,1024,fp);
		send(clientfd,sendBuffer,strlen(sendBuffer),0);
		memset(sendBuffer,0,sizeof(sendBuffer));
/*
		int rlen=recv(clientfd,recvBuffer,sizeof(recvBuffer),0);
		if(rlen<0)
		{
			printf("recv data failed!\n");
		}
		else
		{
			printf("recv data:%s\n",recvBuffer);
		}
		memset(recvBuffer,0,sizeof(recvBuffer));
	}
	*/
		
	}
	printf("finish!\n");
	close(clientfd);
	close(sockfd);
	fclose(fp);
	return 0;
}
