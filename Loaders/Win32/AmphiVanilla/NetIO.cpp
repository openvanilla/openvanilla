#include "NetIO.h"
#include <cstring>
#include <iostream>
using namespace std;

UDPSocket::UDPSocket()
{
#ifdef WIN32
	WSADATA wsaData;
	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
	if ( iResult != NO_ERROR )
		cerr << "Error at WSAStartup()" << endl;
#endif
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#ifdef WIN32
	if ( sockfd == INVALID_SOCKET ) {
		cerr << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
#endif
}

UDPSocket::~UDPSocket()
{
#ifdef WIN32
	WSACleanup();
#endif
}

void UDPSocket::listen(char *addr, int port)
{
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(addr);

	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

int UDPSocket::recv(cmd_in *c)
{
	int l = sizeof(clntaddr);
	int readbytes = recvfrom(sockfd, (char*)c, sizeof(cmd_in), 0, (struct sockaddr *)&clntaddr, &l);
	if(readbytes == -1)
		memset(&clntaddr, 0, sizeof(clntaddr));
	return readbytes;
}

int UDPSocket::send(int cmd, char *buf)
{
	cmd_in c;
	c.cmd = cmd;
	strncpy(c.buf, buf, sizeof(c.buf));
	return sendto(sockfd, (char*)&c, sizeof(c), 0, (struct sockaddr *)&clntaddr, sizeof(clntaddr));
}

int UDPSocket::send(char *addr, int port, int cmd, char *buf)
{
	memset(&clntaddr, 0, sizeof(clntaddr));
	clntaddr.sin_family = AF_INET;
	clntaddr.sin_port = htons(port);
	clntaddr.sin_addr.s_addr = inet_addr(addr);
	return send(cmd, buf);
}
