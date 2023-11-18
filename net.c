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



int net_connect(const char *host, const char *port)
{
	int sd;

	struct addrinfo hints;
	struct addrinfo *server_info, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int ret = getaddrinfo(host, port, &hints, &server_info);
	if (ret != 0) {
		fprintf(stderr, "Name resolution failed.\n");
		fprintf(stderr, "Error %d: %s\n", ret, gai_strerror(ret));

		exit(EXIT_FAILURE);
	}

	// Go through the hosts and try to connect
	for (p = server_info; p != NULL; p = p->ai_next) {
		sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (sd == -1)
			continue;

		ret = connect(sd, p->ai_addr, p->ai_addrlen);
		if (ret == -1) {
			net_close(sd);
			continue;
		}
		// Get out of the loop when connect is successful
		break;
	}

	if (p == NULL) {
		/* TODO (Tiiffi): Check why windows does not report errors */
		fprintf(stderr, "Connection failed.\n");
		#ifndef _WIN32
			fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
		#endif

		freeaddrinfo(server_info);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(server_info);
	return sd;
}

void net_close(int sd)
{
    close(sd);
}



