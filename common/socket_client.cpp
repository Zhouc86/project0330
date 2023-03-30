
#include "../common/iobuffer.h"
#include "protocol.pb.h"
#include "socket_client.h"  
#include <iostream>
#include <memory.h>
SocketClient::SocketClient(INativeNotify* n):_notify(n)
{
	_connected = false;
}
bool SocketClient::Start(const char * ip)
{
#ifdef WIN32
	FnMap.insert(std::make_pair(RemoNet::SC_Sign,&SocketClient::OnSigin));

	FnMap.insert(std::make_pair(RemoNet::SC_Robot, &SocketClient::OnRobotRep));
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyRep, &SocketClient::OnNotifyRep));
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyAdd, &SocketClient::OnNotifyAdd));
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyDel, &SocketClient::OnNotifyDel));
	FnMap.insert(std::make_pair(RemoNet::SC_KickOff, &SocketClient::OnNotifyKick));
#else
	FnMap.insert(std::make_pair(RemoNet::SC_Add, &SocketClient::OnAdd));
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyReq, &SocketClient::OnNotifyReq));
	
#endif
 
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyLeave, &SocketClient::OnNotifyLeave));
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyOffer, &SocketClient::OnNotifyOffer));
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyAnswer, &SocketClient::OnNotifyAnswer));
	FnMap.insert(std::make_pair(RemoNet::SC_NotifyCandidate, &SocketClient::OnNotifyCandidate));


	_ip=ip;
#ifdef WIN32
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(20916);
	sin.sin_addr.s_addr = inet_addr(ip);
	_connected = true;
	if (connect(sockfd, (const sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		
#ifdef WIN32
		DWORD error = WSAGetLastError();
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		sockfd = INVALID_SOCKET;
		//return false;
		_connected=false;

	}

	_notify->OnConnected(_connected);
// 	int flag = 1;
// 	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag,
// 		sizeof(flag));  // Disable Nagle's algorithm
#ifdef WIN32
// 	u_long on = 1;
// 	ioctlsocket(sockfd, FIONBIO, &on);
#else
//	fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif
	_thread = std::thread(&SocketClient::Run, this);
	return true;

}
void SocketClient::Run()
{
	_run = true;
	int32_t Offset = 0;
	CIOBuffer Buffer;
	while (_run)
	{
		if (_connected)
		{
			auto ret = recv(sockfd, (char*)&Buffer.Buffer[Offset], CIOBuffer::IO_BUFFER_SIZE - Offset, 0);
			if (ret <= 0)
			{
				
#ifdef WIN32
				DWORD error = WSAGetLastError();
				closesocket(sockfd);
#else
				close(sockfd);
#endif
				_connected = false;
				_notify->OnConnected(_connected);

			}
			else
			{
				Offset += ret;
				if (Offset >= MessageHead::Size())
				{
					bool bNeedMove = false;
					MessageHead head;
					int8_t* ptr = Buffer.Buffer;
					while (true)
					{
						if (MessageHead::Size() <= Offset)
						{
							head.Deserialize(ptr);
							int32_t length = MessageHead::Size() + head.Length;
							if (Offset >= length)
							{
								int8_t* Data = ptr + MessageHead::Size();

								NetProcess(head.Command, Data, head.Length);

								ptr += length;
								Offset -= length;
							}
							else
							{
								bNeedMove = Offset > 0;
								if(bNeedMove)
								{
									std::cout<<"need Move "<<Offset<<std::endl;
								}
								break;
							}
						}
						else
						{
							break;
						}

					}
					if (bNeedMove)
					{
						memmove(Buffer.Buffer, ptr, Offset);
					}
				}
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			sockaddr_in sin;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(20916);
			sin.sin_addr.s_addr = inet_addr(_ip.c_str());
			if (connect(sockfd, (const sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
			{
#ifdef WIN32
				closesocket(sockfd);
#else
				close(sockfd);
#endif
				sockfd = INVALID_SOCKET;
				continue;

			}
			_connected = true;
			_notify->OnConnected(_connected);
		}
	}
	

}
void SocketClient::Stop()
{
	_run = false;
#ifdef WIN32
	closesocket(sockfd);
#else
	close(sockfd);
#endif
	sockfd = INVALID_SOCKET;
	_thread.join();
}
void SocketClient::NetProcess(int16_t cmd, int8_t* Data, int16_t Size)
{
	 
	 
	auto it = FnMap.find(cmd);
	if (it != FnMap.end())
	{
		(this->*it->second)(Data, Size);
	}
	 
}

#ifdef WIN32
void SocketClient::OnSigin(int8_t* Data, int16_t Size)
{
	RemoNet::SCSign Rep;
	Rep.ParseFromArray(Data, Size);
	if (Rep.ret() == true)
	{
		_uid = Rep.uid();

	}
	_notify->OnSigin(Rep.ret());
}
void SocketClient::OnRobotRep(int8_t* Data, int16_t Size)
{
	RemoNet::SCRobot Rep;
	Rep.ParseFromArray(Data, Size);
	for (int32_t i = 0; i < Rep.robot_size(); i++)
	{
		auto& node = Rep.robot(i);
		_notify->OnRobot(node);
	}
}
void SocketClient::WriteRobotReq()
{
	RemoNet::CSRobot Req;
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Robot;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}

void SocketClient::WriteSign(const char* account, const char* password)
{
	RemoNet::CSSign Req;
	Req.set_account(account);
	Req.set_password(password);
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Sign;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
	
}
void SocketClient::WriteVideoLeave(EgoType type, int32_t peer)
{
	RemoNet::Leave Req;
	Req.set_peer(peer);
	Req.set_egotype(type);
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Leave;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}
#else
void SocketClient::OnAdd(int8_t* Data, int16_t Size)
{
	RemoNet::SCAdd Rep;
	Rep.ParseFromArray(Data, Size);
	if (Rep.ret() == true)
	{
		_uid = Rep.uid();
	}
	_notify->OnAdd(Rep.ret());
}
void SocketClient::WriteAddRobot(std::string& serial,std::string& name,int32_t type)
{
	RemoNet::CSAdd Req;
	Req.set_serial(serial.c_str());
	Req.set_name(name.c_str());
	Req.set_type(type);
   
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Add;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}
#endif
 
 
void SocketClient::OnNotifyAnswer(int8_t* Data, int16_t Size)
{
	 
	RemoNet::Answer Rep;
	Rep.ParseFromArray(Data, Size);
	_notify->OnVideoAnswer(Rep.index(), Rep.type().c_str(), Rep.sdp().c_str());
}
void SocketClient::OnNotifyCandidate(int8_t* Data, int16_t Size)
{
	 
	RemoNet::Candidate Rep;
	Rep.ParseFromArray(Data, Size);
	_notify->OnVideoCandidate(Rep.index(), Rep.candidate().c_str(), Rep.sdpmlineindex(), Rep.sdpmid().c_str());

}
void SocketClient::OnNotifyOffer(int8_t* Data, int16_t Size)
{
   
	RemoNet::Offer Rep;
	Rep.ParseFromArray(Data, Size);
	_notify->OnVideoOffer(Rep.index(), Rep.type().c_str(), Rep.sdp().c_str());
}
#ifdef WIN32
void SocketClient::OnNotifyRep(int8_t* Data, int16_t Size)
{
 
	RemoNet::CSRep Rep;
	Rep.ParseFromArray(Data, Size);
	 
	_notify->OnVideoRep(Rep.index(),Rep.peer());
}
void SocketClient::OnNotifyAdd(int8_t* Data, int16_t Size)
{
	RemoNet::SCAddRobot Rep;
	Rep.ParseFromArray(Data, Size);
	_notify->OnRobot(Rep.robot());
}
void SocketClient::OnNotifyDel(int8_t* Data, int16_t Size)
{
	RemoNet::SCDelRobot Rep;
	Rep.ParseFromArray(Data, Size);
	_notify->OnNotifyDel(Rep.peer(),(EgoType)(Rep.egotype()));
}
void SocketClient::OnNotifyKick(int8_t* Data, int16_t Size)
{
	_notify->OnNotifyKick();
}
#else
void SocketClient::OnNotifyReq(int8_t* Data, int16_t Size)
{
	 
	 
	RemoNet::CSReq Rep;
	Rep.ParseFromArray(Data, Size);
	_notify->OnVideoReq(Rep.index(),Rep.peer());
 

}

#endif

void SocketClient::OnNotifyLeave(int8_t* Data, int16_t Size)
{

	RemoNet::Leave Req;
	Req.ParseFromArray(Data, Size);
	int32_t peer = Req.peer();
	EgoType type = static_cast<EgoType>(Req.egotype());
	_notify->OnVideoLeave(peer,type);
}

void SocketClient::WriteVideoReq(int32_t peer,int32_t index)
{
	//_peer = peer;
	RemoNet::CSReq Req;
	Req.set_peer(peer);
	Req.set_index(index);
	Req.set_egotype(EgoType::Car);
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Req;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}
void SocketClient::WriteVideoRep(int32_t peer,RemoNet::VideoDesc desc,int32_t index)
{
//	_peer = peer;
	RemoNet::CSRep Req;
	Req.set_peer(peer);
	Req.set_desc(desc);
    Req.set_index(index);
	
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Rep;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}
void SocketClient::Write(CIOBuffer* pBuffer)
{
	if (_connected)
	{

		int32_t ret=::send(sockfd, (const char *)pBuffer->Buffer, pBuffer->Length, 0);
		if (ret <= 0)
		{
#ifdef WIN32
			closesocket(sockfd);
#else
			close(sockfd);
#endif
			_connected = false;
		}
	}
}
void SocketClient::WriteOffer(int32_t peer,int32_t index,const char* type, const char* sdp)
{
	RemoNet::Offer Req;
	Req.set_peer(peer);
	Req.set_sdp(sdp);
	Req.set_type(type);
	Req.set_index(index);
	 
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Offer;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}
void SocketClient::WriteAnswer(int32_t peer,int32_t index, const char* type, const char* sdp)
{
	RemoNet::Answer Req;
	Req.set_peer(peer);
	Req.set_sdp(sdp);
	Req.set_type(type);
	Req.set_index(index);
	
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Answer;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}
void SocketClient::WriteCandidate(int32_t peer,int32_t index, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid)
{
	RemoNet::Candidate Req;
	Req.set_peer(peer);
	Req.set_candidate(candidate);
	Req.set_index(index);
	Req.set_sdpmid(sdp_mid);
	Req.set_sdpmlineindex(sdp_mline_index);
	
	 
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_Candidate;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer.Buffer);
	auto ptr = pBuffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}
/*
void SocketClient::MessageCallback(void * user_data,const void * data,const int32_t size)
{
	SocketClient* lhs=static_cast<SocketClient*>(user_data);
	lhs->OnPeerMessage(data,size);
}*/
 
void SocketClient::OnPeerMessage(ChannelType type,int16_t cmd,int16_t length,const void * data)
{
	_notify->OnMessageFrameNotify(type,cmd,length,data);
}
 
void SocketClient::WriteKeepAlive()
{
	MessageHead Head;
	CIOBuffer pBuffer;
	Head.Command = RemoNet::CS_KeepAlive;
	Head.Length = 0;
	Head.Serialize(pBuffer.Buffer);
	pBuffer.Length = MessageHead::Size() + Head.Length;
	Write(&pBuffer);
}