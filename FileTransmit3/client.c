#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h> 

#include <netinet/in.h>
#include <unistd.h> 

#include "transmit.h"
#include "connect.h"

/********************************
/* int stoi(char *str);        */
/* int itos(int val,char *str);*/
/*******************************/

int main()
{
	char file_name[50];				//file to send
	printf("please input file_name of 1G_file\n");
	scanf("%s",file_name);
	int file_size=1073741824;
	int fd = client_connect();
    	send(fd,"sendfile",8,0);   
    	close(fd);   
	file_send(file_size,file_name);		 // send file 
    	return 0;
}

/*********

int stoi(char *str)
{
	char *p=str;
	int sum=0;
	while(*p)
	{
		sum=sum*10+(int)(*p-'0')
			p++;
	}
	return sum;
}

int itos(int val,char *str)
{
	int temp;
	char *back=str;
	int count=0;
	while(val>0)
	{
		temp=val%10;
		*back=char(temp+'0');
		back++;
		val/=10;
		count++;
	}
	*(back+1)='\0';
	char *front=str;
	char ch;
	while(front<back)
	{
		ch=*front;
		*front=*back;
		*back=ch;
	}
	return count;	
}

*******************/
