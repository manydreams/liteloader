#ifndef MCRCON
#define MCRCON

#include"net.h"
#include"mcrcon_packet.h"


#define ROCN_LOGIN      3
#define RCON_COMMAND    2
#define RCON_PID        0xBADC0DE


int net_send_packet(int sd, rc_packet *packet);
rc_packet *net_recv_packet(int sd);
int rcon_auth(int sock, char *passwd);
rc_packet *rcon_command(int sock, char *command);

#endif 