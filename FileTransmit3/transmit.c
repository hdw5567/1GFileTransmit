#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#include "transmit.h"
#include "connect.h"

#define MAXSIZE  4096 
#define PTHREAD_NUM  4

typedef struct pthread_param   
{
    char *file_name;
    int fd;
    int offset;   
    int size;   
}pthread_param;   
 
void pthread_recv(pthread_param *args);
void pthread_send(pthread_param *args);


void file_recv(int listenfd,int file_size,char *file_name)
{
	// create file to receive data
	FILE *fp=fopen(file_name,"wb");
    if(NULL == fp)
    {
    	printf(" create file failed\n");
    	exit(1);
    } 
    fclose(fp);

    struct sockaddr_in client_addr;    
    struct pthread_param pthread_args[10];		//MAX_PTHREAD_NUM=10
    pthread_t pid[10];										
    time_t	 t_start, t_end;

    int addrlen = sizeof(client_addr);	
	int block = file_size / PTHREAD_NUM;

	printf("Data Transmiting...\n");
	
	//timing 
    t_start = time(NULL);   

    // initialize pthread args & wait client connect
	int i;
	for(i=0;i<PTHREAD_NUM;i++)
	{
		pthread_args[i].fd=accept(listenfd,(struct sockaddr*)&client_addr,&addrlen);
	}
    for(i=0;i<PTHREAD_NUM;i++)
    {
		pthread_args[i].file_name = file_name;
        pthread_args[i].offset = block * i; 
        pthread_args[i].size = block;  		
		if(i == PTHREAD_NUM-1)  
		{
            pthread_args[i].size += (file_size % PTHREAD_NUM);
        }
//		pthread_args[i].fd = accept(listenfd,(struct sockaddr*)&client_addr,&addrlen);
		pthread_create(&pid[i], NULL, (void *)pthread_recv, (void *)&pthread_args[i]);  
    }
   

    for(i=0;i<PTHREAD_NUM;i++)
    {   
		printf("start:%d\n",i);
        pthread_join(pid[i],NULL);
		printf("end:%d\n",i);
    }

    //  dbtime_endAndShow(); 
    t_end = time(NULL);
    printf("The total_time is %0.fs\n",difftime(t_end,t_start));
}


void pthread_recv(pthread_param *args)
{     
	FILE *fp = fopen(args->file_name,"rb+");
    if(-1 == fseek(fp,args->offset,SEEK_SET))   
    {
        printf("fseek error\n");
	    exit(1);
    }

    send(args->fd,"hello",5,0);
	int len;
	char recvbuff[MAXSIZE];	
	
    int recv_size =0;
    while(recv_size<args->size)
    {
        while((len = recv(args->fd,recvbuff,MAXSIZE,0))<=0);
		recv_size+=len;
		fwrite(recvbuff,len,1,fp);      	
		send(args->fd,"hello",5,0);
    }
	printf("this recv pthread end\n");
    fclose(fp);								// close file
    close(args->fd);				// close socket
    pthread_exit(NULL);			// end pthread	
}


void file_send(int file_size,char *file_name)
{
    struct pthread_param pthread_args[10]; // MAX_PTHREAD_NUM
    pthread_t pid[10];   
 
    /* initialize pthread args  & create pthread */ 
    int block = file_size / PTHREAD_NUM;
	int i;	
    for(i=0;i<PTHREAD_NUM;i++)
    {   
		pthread_args[i].file_name = file_name;
        pthread_args[i].offset = block * i; 
        pthread_args[i].size = block;
        if(i == PTHREAD_NUM-1)   /* the last pthread */
        {
            pthread_args[i].size += (file_size % PTHREAD_NUM);
        }
        pthread_args[i].fd = client_connect();   //ask for connect
        pthread_create(&pid[i], NULL, (void *)pthread_send, (void *)&pthread_args[i]);   
    }       
	

    /* end pthread */
    for(i=0;i<PTHREAD_NUM;i++)
    {   
        pthread_join(pid[i],NULL);   
    }
}


void pthread_send(pthread_param *args)
{
    FILE *fp = fopen(args->file_name,"rb+");     
    if(-1 == fseek(fp,args->offset,SEEK_SET)) 
    {
        printf("in this pthread:  fseek error\n");
        exit(1);
    }

    char sendbuff[MAXSIZE];
    char recvbuff[MAXSIZE];

    /* wait for server request  */
    while(recv(args->fd,recvbuff,MAXSIZE,0) != 5);
	
	int total_size=args->size;
	int len;	
    while(total_size>0)                    
    {
		if(total_size>= MAXSIZE)
		{
			len = fread(sendbuff,1,MAXSIZE,fp);
		}
		else
		{
			len = fread(sendbuff,1,total_size,fp);
		}
		if(-1 == send(args->fd,sendbuff,len,0))
		{
           printf("send file faild\n");
           close(args->fd);
           exit(1);
   		}
		total_size-=len;
		while(recv(args->fd,recvbuff,MAXSIZE,0) != 5);
    }

    fclose(fp);
    close(args->fd);
    pthread_exit(NULL);
}


