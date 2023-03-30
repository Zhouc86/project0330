#pragma once
#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WS2tcpip.h>
#include <WinSock2.h>
#define socketerrno WSAGetLastError()
#define SOCKET_EAGAIN_EINPROGRESS WSAEINPROGRESS
#define SOCKET_EWOULDBLOCK WSAEWOULDBLOCK
#ifndef _SSIZE_T_DEFINED
typedef int ssize_t;
#define _SSIZE_T_DEFINED
#endif
#ifndef _SOCKET_T_DEFINED
typedef SOCKET socket_t;
#define _SOCKET_T_DEFINED
#endif
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <fcntl.h>
#define socketerrno errno
#define SOCKET_EAGAIN_EINPROGRESS EAGAIN
#define SOCKET_EWOULDBLOCK EWOULDBLOCK
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#ifndef _SOCKET_T_DEFINED
typedef int socket_t;
#define _SOCKET_T_DEFINED
#endif
#endif
#include <thread>
#include <mutex>
#include <unordered_map>
#include "../common/comm.h"
//#include "../common/radar_deal.h"
class CMessageQueue;
class SocketLidar
{
public:
	SocketLidar(CMessageQueue* n);
	bool Start(int32_t port);
	void Run();
	void Stop();
	RadarDeal radarDeal;
private:
    socket_t _fd;
	std::thread  _thread;
	bool _run;
	std::mutex _lock;
	std::unordered_map<int32_t, cannet_frame> _message;
	CMessageQueue* _canNotify;
    sockaddr_in _canaddr;
};