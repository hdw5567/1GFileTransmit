#ifndef TRANSMIT_H
#define TRANSMIT_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


void file_recv(int listenfd,int file_size,char *file_name);

void file_send(int file_size,char *file_name);

#endif
