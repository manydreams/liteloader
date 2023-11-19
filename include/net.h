#ifndef NET
#define NET 


/*
create a socket and connect server
*/
int net_init(const char *host, const char *port);
void net_close(int sd);
int net_clean_incoming(int sd, int size);


#endif