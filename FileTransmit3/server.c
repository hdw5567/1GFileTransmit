#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "transmit.h"
#include "connect.h"

#define MAXSIZE   4096 
#define file_name   "recv"

int main()
{
    struct sockaddr_in servaddr;   
    char buf[MAXSIZE];
   // int len;
    int file_size=1073741824;   
    int listenfd = server_socket();
    listen(listenfd,10);    
    int addrlen = sizeof(servaddr);
    printf(" waiting for client connect \n");

    int newfd;
    if(-1 == (newfd = accept(listenfd,(struct sockaddr*)&servaddr,&addrlen)))
    {
        printf("accept failed\n");
        exit(1);
    }
	recv(newfd,buf,MAXSIZE,0);   
	printf("recv: %s\n",buf);                         //
    close(newfd);

    //  recieve file 
    file_recv(listenfd,file_size,file_name);

    close(listenfd);
    printf("receive finish!\n"); 
	
    return 0;
}




