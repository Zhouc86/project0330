#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <vector>
#include <thread>
#include <mutex>
#include "../Protocol/win/Protocol.pb.h"
#include "NetService.h"
 
#include "UserSocket.h"
#include "UserManager.h"
#include "../common/Pool.h"
 

#define  PORT 20916
CNetService& CNetService::GetInstance()
{
	static  CNetService m;
	return m;
}
void CNetService::Start()
{
	CUserSocket::InitFnDirector();
	 
	 

	 
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);
	if (::bind(m_hSocket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf(" Failed bind() \n");
		return;
	}
	listen(m_hSocket, 5);
	m_hEvent = WSACreateEvent();
	WSAEventSelect(m_hSocket, m_hEvent, FD_ACCEPT | FD_CLOSE);
	CreateIOThread();
	_bRunning = true;
	//_thread = std::thread(std::bind(&CNetService::Run, this));
}
bool CNetService::CreateIOThread()
{
	DWORD dwThreads;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	dwThreads = info.dwNumberOfProcessors << 1;
	m_hCompletePort = CreateIoCompletionPort((HANDLE)-1, NULL, NULL, dwThreads);
	for (DWORD i = 0; i < dwThreads; i++)
	{
		_walkarray.push_back(std::thread(&CIOSocket::IOThread, m_hCompletePort));
	}
	return true;
}
void CNetService::Run()
{
	while (_bRunning)
	{
		int ret = WaitForSingleObject(m_hEvent, WSA_INFINITE);
		if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
		{
			continue;
		}
		else
		{
			WSANETWORKEVENTS events;
			WSAEnumNetworkEvents(m_hSocket, m_hEvent, &events);
			if (events.lNetworkEvents & FD_ACCEPT)
			{
				if (events.iErrorCode[FD_ACCEPT_BIT] == 0)
				{
				
					sockaddr_in addr;
					int len = sizeof(sockaddr_in);
					SOCKET s = accept(m_hSocket, (sockaddr*)&addr, &len);
					//if (strcmp(inet_ntoa(addr.sin_addr), "222.71.48.226") != 0)
					{
						printf("a socket connect %s\n", inet_ntoa(addr.sin_addr));
						CUserSocket* ptr = Alloc_Pool<CUserSocket>::GetInstance().Alloc(m_hCompletePort, s);
						CUserManager::GetInstance().Add(ptr);
					}
				}
			}
		}
	}
}