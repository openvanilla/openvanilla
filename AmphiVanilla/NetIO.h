#ifndef NetIO_h
#define NetIO_h
#ifdef WIN32
#include <winsock2.h>
#endif
typedef enum _cmdlist {
	KEYCODE, /* buf offset 0 = IM#, 1 = key, 2 = capslock, 3 = shift*/
	SETBUFSTRING,
	SENDBUF,
	SETCANDISTRING,
	SHOWBUF,
	SHOWCANDI,
	SHOWNOTIFY,
	HIDENOTIFY,
	SETCURSORPOS,
	SETMARKFROM,
	SETMARKTO,
	PROCESS,
	PASS,
	END,
	GETNAME,
	RELOADCONFIG,
	RETURN,
} cmd_list;
typedef struct _cmd {
	int cmd;
	char buf[256];
} cmd_in;
class UDPSocket {
	public:
	UDPSocket();
	~UDPSocket();
	void UDPSocket::listen(char *addr, int port);
	int recv(cmd_in*);
	int send(int cmd, char *buf);
	int send(char *addr, int port, int cmd, char *buf);
	protected:
	int sockfd;
	struct sockaddr_in servaddr, clntaddr;
};
#endif // NetIO_h
