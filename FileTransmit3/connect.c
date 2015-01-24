#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "connect.h"

#define PORT 8888

int server_socket(void)
{
    int listenfd;
    struct sockaddr_in serveraddr;    
    if(-1 == (listenfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("create socket failed .\n");
        exit(1);
    } 
	
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);
	
    if(-1 == bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)))
    {
        printf("bind failure.Please run again.\n");
        exit(1);
    }
    return listenfd;
}



int client_connect(void)
{
    struct sockaddr_in clientaddr;
    int sockfd;

    if(-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("create socket failed.\n");
        exit(1);
    }

    bzero(&clientaddr, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(PORT);
    if(-1 == (inet_aton("127.0.0.1",&clientaddr.sin_addr)))
    {
        printf("set server address failed\n");
        close(sockfd);
        exit(1);
    }

    if(-1 == (connect(sockfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr))))
    {
        printf("connect failed\n");
        close(sockfd);
        exit(1);
    }

    return sockfd;
}
