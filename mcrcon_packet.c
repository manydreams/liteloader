#include"include/mcrcon_packet.h"

rc_packet *packet_build(int id, int cmd, char *s1)
{
	static rc_packet packet = {0, 0, 0, { 0x00 }};

	// size + id + cmd + s1 + s2 NULL terminator
	int len = strlen(s1);
	if (len >= DATA_BUFFSIZE) {
		fprintf(stderr, "Warning: Command string too long (%d). Maximum allowed: %d.\n", len, DATA_BUFFSIZE - 1);
		return NULL;
	}

	packet.size = sizeof(int) * 2 + len + 2;
	packet.id = id;
	packet.cmd = cmd;
	strncpy(packet.data, s1, DATA_BUFFSIZE - 1);

	return &packet;
}