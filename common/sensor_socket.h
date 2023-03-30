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
#include <poll.h>
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
#include <iostream>
#include <unordered_map>
#include "../common/comm.h"
#ifdef _MAINDLG
class CThreadWindow;
#else
class CMessageQueue;
#endif
class CIOBuffer;
template<typename T>
class SensorSocket
{
public:
#ifdef _MAINDLG
	SensorSocket(CThreadWindow* q, std::string can_ip, int32_t can_port, int32_t host_port);
#else
	SensorSocket(CMessageQueue * q,std::string can_ip,int32_t can_port,int32_t host_port);
#endif
	bool Start(const char* ip=nullptr);
	void Run();
	void Stop();
	void Write(CIOBuffer * pBuffer);
	void Read(CIOBuffer* pBuffer);
	T* Get();
#ifndef WIN32
    void SetStartRead(bool b);
	void SetStartWrite(bool b);
#endif
private:
	socket_t _fd;
	std::thread  _thread;
	bool _run;
	std::string _canip;
	int32_t _canport;
	int32_t _hostport;
	std::mutex _lock;
	std::unordered_map<int32_t, cannet_frame> _message;
	std::unique_ptr<T> _sensorNotify;
    sockaddr_in _canaddr;
#ifndef WIN32
	bool _startRead;
	bool _startWrite;
#endif
};
template<typename T>
#ifdef _MAINDLG
SensorSocket<T>::SensorSocket(CThreadWindow* q, std::string can_ip, int32_t can_port, int32_t host_port)
#else
SensorSocket<T>::SensorSocket(CMessageQueue* q, std::string can_ip, int32_t can_port, int32_t host_port)
#endif
{
  	_sensorNotify=std::make_unique<T>(q);
	_canip=can_ip;
	_canport=can_port;
	_hostport=host_port;
#ifndef WIN32
 _startWrite=_startRead=true;
#endif
}
template<typename T>

bool SensorSocket<T>::Start(const char * ip)
{
#ifdef WIN32
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

    std::cout<<"SensorSocket<T>::Start"<<std::endl;
	// _sensorNotify->Stop();
	_sensorNotify->SetSensorSocket(this);
	_fd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(_hostport);
#ifdef WIN32
	sin.sin_addr.s_addr = inet_addr(ip);
#else
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	if (::bind(_fd, (sockaddr*)&sin, sizeof(sin)) == -1)
		return false;

	
	_canaddr.sin_family=AF_INET;
	_canaddr.sin_addr.s_addr=inet_addr(_canip.c_str());
	_canaddr.sin_port=htons(_canport);	
	_thread = std::thread(&SensorSocket::Run, this);
	_sensorNotify->Start();
	return true;
}
template<typename T>
void SensorSocket<T>::Read(CIOBuffer* pBuffer)
{
	sockaddr_in from;
	socklen_t fromlen=sizeof(sockaddr_in);
	int32_t ret = recvfrom(_fd,(char *)pBuffer->Buffer, CIOBuffer::IO_BUFFER_SIZE,0,(sockaddr*)&from,&fromlen);
	if (ret <= 0)
	{
		 return;
    }
	pBuffer->Length=ret;
}
template<typename T>
void SensorSocket<T>::Run()
{
	_run = true;
	struct pollfd fds[1];
	fds[0].fd = _fd;
    fds[0].events = POLLIN;


//	long long  k = 0;
	//long long tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	
	while (_run)
	{
#ifdef WEBRTC_LINUX
      //  _sensorNotify->PreProcess();
		if(poll(&fds[0], 1, 1000) > 0)
	   {
		    if (fds[0].revents & POLLIN)
			{
#endif
				CIOBuffer pBuffer;
				sockaddr_in from;
				socklen_t fromlen=sizeof(sockaddr_in);
				int32_t ret = recvfrom(_fd,(char *)pBuffer.Buffer, CIOBuffer::IO_BUFFER_SIZE,0,(sockaddr*)&from,&fromlen);
				if (ret <= 0||!_run)
				{
					continue;
				}
        
	   			_sensorNotify->Notify(pBuffer.Buffer,ret);
#ifdef WEBRTC_LINUX
			}
	   }
#endif
 
	}
    std::cout<<"SensorSocket<T>::Run Finished"<<std::endl;
	
}
template<typename T>
void SensorSocket<T>::Write(CIOBuffer * pBuffer)
{
	
#ifndef WIN32
	if(_startWrite==false) return;
#endif
	socklen_t len=sizeof(_canaddr);
	 
    int ret=::sendto(_fd,(char *)pBuffer->Buffer,pBuffer->Length,0,(const sockaddr *)&_canaddr,len);
	{
	//     std::cout<<"ret = "<<ret<<" size ="<<pBuffer->Length<<std::endl;
	}
 
}


template<typename T>
void SensorSocket<T>::Stop()
{
	if(!_run) return;
	_sensorNotify->Stop();
	
	_run = false;
#ifdef WIN32
	closesocket(_fd);
#else
	close(_fd);
#endif
    std::cout<<"SensorSocket<T>::Stop"<<std::endl;
	_thread.join();
	std::cout<<"SensorSocket<T>::Stop finished"<<std::endl;
}
#ifndef WIN32
template<typename T>
void SensorSocket<T>::SetStartRead(bool b)
{
	_startRead=b;
}
template<typename T>
void SensorSocket<T>::SetStartWrite(bool b)
{
	_startWrite=b;
}
#endif
template<typename T>
T* SensorSocket<T>::Get()
{
	return _sensorNotify.get();
}

template<typename T>
class SensorTCP
{
public:
#ifdef _MAINDLG
	SensorTCP(CThreadWindow* q, std::string can_ip, int32_t can_port, int32_t host_port);
#else
	SensorTCP(CMessageQueue * q,std::string can_ip,int32_t can_port,int32_t host_port);
#endif
	bool Start(const char* ip=nullptr);
	void Run();
	void Stop();
	void Write(CIOBuffer * pBuffer);
	T* Get();
#ifndef WIN32
    void SetStartRead(bool b);
	void SetStartWrite(bool b);
#endif
private:
	socket_t _fd;
	std::thread  _thread;
	bool _run;
	std::string _canip;
	int32_t _canport;
	int32_t _hostport;
	std::mutex _lock;
	std::unordered_map<int32_t, cannet_frame> _message;
	std::unique_ptr<T> _sensorNotify;
    sockaddr_in _canaddr;
#ifndef WIN32
	bool _startRead;
	bool _startWrite;
#endif
};
template<typename T>
#ifdef _MAINDLG
SensorTCP<T>::SensorTCP(CThreadWindow* q, std::string can_ip, int32_t can_port, int32_t host_port)
#else
SensorTCP<T>::SensorTCP(CMessageQueue* q, std::string can_ip, int32_t can_port, int32_t host_port)
#endif
{
  	_sensorNotify=std::make_unique<T>(q);
	_canip=can_ip;
	_canport=can_port;
	_hostport=host_port;
#ifndef WIN32
 _startWrite=_startRead=true;
#endif
}
template<typename T>

bool SensorTCP<T>::Start(const char * ip)
{
#ifdef WIN32
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

    std::cout<<"SensorSocket<T>::Start"<<_canip<<","<<_canport<<std::endl;
	// _sensorNotify->Stop();
	_sensorNotify->SetSensorSocket(this);
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(_canport);
#ifdef WIN32
	sin.sin_addr.s_addr = inet_addr(ip);
#else
	sin.sin_addr.s_addr =inet_addr(_canip.c_str());// htonl(INADDR_ANY);
#endif
	while (::connect(_fd, (sockaddr*)&sin, sizeof(sin)) == -1)
	{
		std::cout<< "connect "<<_canip<<" failed"<<std::endl;
		  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}	 
	/*_canaddr.sin_family=AF_INET;
	_canaddr.sin_addr.s_addr=inet_addr(_canip.c_str());
	_canaddr.sin_port=htons(_canport);
	*/	
	_thread = std::thread(&SensorTCP::Run, this);
	_sensorNotify->Start();
	return true;
}
template<typename T>
void SensorTCP<T>::Run()
{
	_run = true;
	struct pollfd fds[1];
	fds[0].fd = _fd;
    fds[0].events = POLLIN;
	
//	long long  k = 0;
	//long long tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	while (_run)
	{
#ifdef WEBRTC_LINUX
      //  _sensorNotify->PreProcess();
		if(poll(fds, 1, 1000) > 0)
	   {
		    if (fds[0].revents & POLLIN)
			{
#endif
				CIOBuffer pBuffer;
				sockaddr_in from;
				socklen_t fromlen=sizeof(sockaddr_in);
				int32_t ret = recv(_fd,(char *)pBuffer.Buffer, CIOBuffer::IO_BUFFER_SIZE,0);
				if (ret <= 0||!_run)
				{
					continue;
				}
        
	   			_sensorNotify->Notify(pBuffer.Buffer,ret);
#ifdef WEBRTC_LINUX
			}
	   }
#endif
 
	}
    std::cout<<"SensorSocket<T>::Run Finished"<<std::endl;
}
template<typename T>
void SensorTCP<T>::Write(CIOBuffer * pBuffer)
{
	
#ifndef WIN32
	if(_startWrite==false) return;
#endif
	socklen_t len=sizeof(_canaddr);
	 
    int ret=::send(_fd,(char *)pBuffer->Buffer,pBuffer->Length,0);
	if(ret<=0)
	{
	     std::cout<<"ret = "<<ret<<" size ="<<pBuffer->Length<<std::endl;
	}
 
}
template<typename T>
void SensorTCP<T>::Stop()
{
	if(!_run) return;
	_sensorNotify->Stop();
	
	_run = false;
#ifdef WIN32
	closesocket(_fd);
#else
	close(_fd);
#endif
    std::cout<<"SensorSocket<T>::Stop"<<std::endl;
	_thread.join();
	std::cout<<"SensorSocket<T>::Stop finished"<<std::endl;
}
#ifndef WIN32
template<typename T>
void SensorTCP<T>::SetStartRead(bool b)
{
	_startRead=b;
}
template<typename T>
void SensorTCP<T>::SetStartWrite(bool b)
{
	_startWrite=b;
}
#endif
template<typename T>
T* SensorTCP<T>::Get()
{
	return _sensorNotify.get();
}