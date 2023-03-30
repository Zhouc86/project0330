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
#include <unordered_map>
#include <functional>
 #include "comm.h"
#include "notifier.h"
class CIOBuffer;

class SocketClient
{
public:
	typedef void (SocketClient::* FN)(int8_t* Data, int16_t Size);
	SocketClient(INativeNotify* n);
	bool Start(const char * ip);
	void Run();
	void Stop();
	void Write(CIOBuffer* pBuffer);
public:
#ifdef WIN32
	void WriteSign(const char* account, const char* password);
	void WriteRobotReq();
	void WriteVideoLeave(EgoType type, int32_t peer);
	 
#else
	void WriteAddRobot(std::string& serial,std::string& name,int32_t type);
#endif
	void WriteOffer(int32_t peer,int32_t index, const char* type, const char* sdp);
	void WriteAnswer(int32_t peer,int32_t index, const char* type, const char* sdp);
	void WriteCandidate(int32_t peer,int32_t index, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid);
	void WriteVideoReq(int32_t peer, int32_t index);
	void WriteVideoRep(int32_t peer, RemoNet::VideoDesc desc, int32_t index);
	void WriteKeepAlive();
	//static void MessageCallback(void * user_data,const void * data,const int32_t size);
 
	void OnPeerMessage(ChannelType type,int16_t cmd,int16_t length,const void * data);
 
private:
	void NetProcess(int16_t cmd, int8_t* Data, int16_t Size);
#ifdef WIN32
	void OnSigin(int8_t* Data, int16_t Size);
	 
	void OnNotifyRep(int8_t* Data, int16_t Size);
	void OnNotifyAdd(int8_t* Data, int16_t Size);
	void OnNotifyDel(int8_t* Data, int16_t Size);
	void OnNotifyKick(int8_t* Data, int16_t Size);
#else
	void OnAdd(int8_t* Data, int16_t Size);
	 
	void OnNotifyReq(int8_t* Data, int16_t Size);
	
#endif
	void OnNotifyLeave(int8_t* Data, int16_t Size);
	void OnNotifyOffer(int8_t* Data, int16_t Size);
	void OnNotifyAnswer(int8_t* Data, int16_t Size);
	void OnNotifyCandidate(int8_t* Data, int16_t Size);
	
#ifdef WIN32
	void OnRobotRep(int8_t* Data, int16_t Size);
#endif
    
private:
	socket_t sockfd;
	std::thread _thread;
	bool _run;
	bool _connected;
	int32_t _uid;
	std::string _ip;
	std::unordered_map<int16_t, FN> FnMap;
	INativeNotify* _notify;
};