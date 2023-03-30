#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include "Protocol.pb.h"
#include <sstream>
#include "out_sim.h"
#include "../common/iobuffer.h"

COutSim::COutSim(const char* ip, int32_t port) :_head(nullptr),_ip(ip),_port(port)
{

}
void COutSim::Start()
{
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
	_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &sin.sin_addr);
	connect(_socket, (sockaddr*)&sin, sizeof(sin));
	_thread = std::thread(std::bind(&COutSim::Run, this));
}
void COutSim::Stop()
{
	closesocket(_socket);
	if (_run == true)
	{
		_run = false;

		Mtx.lock();
		cv.notify_all();
		Mtx.unlock();

		_thread.join();
	}
}
void COutSim::PushSimPack(CIOBuffer* pBuffer)
{
	std::unique_lock <std::mutex> lck(Mtx);
	//bool bNull = false;
	if (_head == nullptr)
	{
		_head = pBuffer;


	}
	else {
		_head->Release();
		_head = pBuffer;

	}
}
void COutSim::Run()
{
	_run = true;
	while (_run)
	{
		if (!_run) break;
		if (_head != nullptr)
			SendOutSim(_head);
		Sleep(30);
	}
}
bool COutSim::SendOutSim(CIOBuffer* pBuffer)
{
	int32_t ret = send(_socket, (const char *)pBuffer->Buffer, pBuffer->Length, 0);
	return ret == pBuffer->Length;
}
void COutSim::OnPeerMessage(int16_t cmd, int16_t length, const void* data)
{
	
	RemoNet::IMuMessage rep;
	rep.ParseFromArray(data, length);
	CIOBuffer* buffer=CIOBuffer::Alloc();
	OutSimPack p;
	//= (OutSimPack*)buffer->Buffer;
	p.rx = (rep.rx()*3.1415926) /180.f ;
	p.ry = (rep.ry()*3.1415926)/180.f;
	p.rz = 0;
	memcpy(buffer->Buffer, &p, sizeof(p));
	buffer->Length = sizeof(p);
	//std::ostringstream str;
	//str << p.rx <<","<< p.ry << std::endl;
//	OutputDebugStringA(str.str().c_str());
	buffer->Length = sizeof(OutSimPack);
	PushSimPack(buffer);

}