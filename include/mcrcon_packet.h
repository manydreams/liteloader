#ifndef MCRCON_PACKET
#define MCRCON_PACKET


#define DATA_BUFFSIZE 4096

/* rcon packet structure 

Length (int), Length of remainder of packet

Request ID (int), Client-generated ID

Type (int), 3 for login, 2 to run a command, 0 for a multi-packet respons

Payload (byte[4096]), NULL-terminated ASCII text

2-byte pad (byte[2]), NULL

ignoring string2 for now
*/
typedef struct _rc_packet {
    int size;
    int id;
    int cmd;
    char data[DATA_BUFFSIZE];
    // ignoring string2 for now
} rc_packet;

rc_packet *packet_build(int id, int cmd, char *s1);
#endif