#ifndef NET
#define NET 

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<strings.h>
#include<signal.h>
#include<errno.h>
#include<unistd.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h> 


/*
create a socket and connect server
*/
int net_init(const char *host, const char *port);
void net_close(int sd);
int net_clean_incoming(int sd, int size);


#endif