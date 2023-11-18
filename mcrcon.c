#include"mcrcon.h"

int net_send_packet(int sd, rc_packet *packet)
{
	int len;
	int total = 0;	// bytes we've sent
	int bytesleft;	// bytes left to send 
	int ret = -1;

	bytesleft = len = packet->size + sizeof(int);

	while (total < len) {
		ret = send(sd, (char *) packet + total, bytesleft, 0);
		if(ret == -1) break;
		total += ret;
		bytesleft -= ret;
	}

	return ret == -1 ? -1 : 1;
}

rc_packet *net_recv_packet(int sd)
{
	int psize;
	static rc_packet packet = {0, 0, 0, { 0x00 }};

	// packet.size = packet.id = packet.cmd = 0;

	int ret = recv(sd, (char *) &psize, sizeof(int), 0);

	if (ret == 0) {
		fprintf(stderr, "Connection lost.\n");
		return NULL;
	}

	if (ret != sizeof(int)) {
		fprintf(stderr, "Error: recv() failed. Invalid packet size (%d).\n", ret);
		return NULL;
	}

	// NOTE(Tiiffi): This should fail if size is out of spec!
	if (psize < 10 || psize > DATA_BUFFSIZE) {
		fprintf(stderr, "Warning: invalid packet size (%d). Must over 10 and less than %d.\n", psize, DATA_BUFFSIZE);

		if(psize > DATA_BUFFSIZE  || psize < 0) psize = DATA_BUFFSIZE;
		net_clean_incoming(sd, psize);

		return NULL;
	}

	packet.size = psize;

	int received = 0;
	while (received < psize) {
		ret = recv(sd, (char *) &packet + sizeof(int) + received, psize - received, 0);
		if (ret == 0) { /* connection closed before completing receving */
			fprintf(stderr, "Connection lost.\n");
			return NULL;
		}

		received += ret;
	}

	return &packet;
}

int rcon_auth(int sock, char *passwd)
{
	int ret;

	rc_packet *packet = packet_build(RCON_PID, ROCN_LOGIN, passwd);
	if (packet == NULL)
		return 0;

	ret = net_send_packet(sock, packet);
	if (!ret)
		return 0; // send failed

	packet = net_recv_packet(sock);
	if (packet == NULL)
		return 0;

	// return 1 if authentication OK
	return packet->id == -1 ? 0 : 1;
}

rc_packet *rcon_command(int sock, char *command)
{
	rc_packet *packet = packet_build(RCON_PID, RCON_COMMAND, command);
	if (packet == NULL) {
		return 0;
	}

	net_send_packet(sock, packet);

	packet = net_recv_packet(sock);
	if (packet == NULL)
		return 0;

	if (packet->id != RCON_PID)
		return 0;

	if (packet->id == RCON_PID)
		return packet;

	return 0;
}
