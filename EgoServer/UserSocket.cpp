#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <mutex>
#include "../common/pool.h"
#include "../common/IOBuffer.h"
#include "../webrtcinterop/include/api.h"
#include "../Protocol/win/Protocol.pb.h"
#include "UserManager.h"
#include "DBConnect.h"
#include "scoped_ptr.h"
#include "UserSocket.h"
 

FNRedirector<CUserSocket> CUserSocket::Redirector;
CUserSocket::CUserSocket(HANDLE hCom, SOCKET s) :CIOSocket(hCom, s)
{
	_uid = -1;
	_loginsucc = false;
	_egoType = EgoType::None;
	_userpeer = -1;
	Init(hCom);
	TimeTick = GetTickCount64();
}
void CUserSocket::InitFnDirector()
{
	Redirector.Insert(RemoNet::CS_Answer, &CUserSocket::OnAnswer);
  
	Redirector.Insert(RemoNet::CS_Candidate, &CUserSocket::OnCadidate);
	Redirector.Insert(RemoNet::CS_KeepAlive, &CUserSocket::OnKeepAlive);
	Redirector.Insert(RemoNet::CS_Add, &CUserSocket::OnAdd);
	Redirector.Insert(RemoNet::CS_Robot, &CUserSocket::OnRobot);
	Redirector.Insert(RemoNet::CS_Offer, &CUserSocket::OnOffer);
	Redirector.Insert(RemoNet::CS_Rep, &CUserSocket::OnRepVideo);
	Redirector.Insert(RemoNet::CS_Req, &CUserSocket::OnReqVideo);
	Redirector.Insert(RemoNet::CS_Sign, &CUserSocket::OnSigin);
	Redirector.Insert(RemoNet::CS_Leave, &CUserSocket::OnLeave);
	//Redirector.Insert(RemoNet::CS_CloseVideo, &CUserSocket::OnCloseVideo);
}
void CUserSocket::OnAnswer(int8_t* Data, int16_t Size)
{
	std::cout << __FUNCTION__ << std::endl;
	RemoNet::Answer Req;
	Req.ParseFromArray(Data, Size);
	int32_t peer = Req.peer();
	std::cout << Req.sdp().c_str() << std::endl;
	CUserManager::GetInstance().NotifyAnswer(peer, _uid, Req.index(), Req.type().c_str(), Req.sdp().c_str());
// 	Req.set_peer(_uid);
// 	CIOBuffer* pBuffer = CIOBuffer::Alloc();
// 	MessageHead Head;
// 	Head.Command = RemoNet::SC_Answer;
// 	Head.Length = Req.ByteSizeLong();
// 	Head.Serialize(pBuffer->Buffer);
// 	auto ptr = pBuffer->Buffer + MessageHead::Size();
// 	Req.SerializeToArray(ptr, Head.Length);
// 	pBuffer->Length = MessageHead::Size() + Head.Length;
// 	CUserManager::GetInstance().Write(peer, pBuffer);
// 	pBuffer->Release();
}
void CUserSocket::OnLeave(int8_t* Data, int16_t Size)
{
	
	RemoNet::Leave Req;
	Req.ParseFromArray(Data, Size);
	auto type = Req.egotype();
	if (_egoType != EgoType::User||type==EgoType::User) return;
	int32_t peer = Req.peer();
	if (type == EgoType::Car)
	{
		if (peer != _userpeer) return;
		_userpeer = -1;
	
	}
	 
	CUserManager::GetInstance().LeavePeerVideo(peer, _uid, _egoType);

}
// 
// void CUserSocket::OnCloseVideo(int8_t* Data, int16_t Size)
// {
// 	std::cout << "OnCloseVideo" << std::endl;
// 	RemoNet::Close Req;
// 	Req.ParseFromArray(Data, Size);
// 	auto type = Req.egotype();
// 	if (_egoType != EgoType::User || type == EgoType::User) return;
// 	int32_t peer = Req.peer();
// 	int32_t index = Req.index();
// 	if (type == EgoType::Car)
// 	{
// 		if (peer != _carpeer) return;
// 		
// 
// 	}
// 	else if (type == EgoType::Cave)
// 	{
// 		if (peer != _cavepeer) return;
// 		
// 	}
// 	CUserManager::GetInstance().ClosePeerVideo(peer, _uid, _egoType, index);
// }
void CUserSocket::OnCadidate(int8_t* Data, int16_t Size)
{
	RemoNet::Candidate Req;
	Req.ParseFromArray(Data, Size);
	int32_t peer = Req.peer();
	CUserManager::GetInstance().NotifyCandidate(peer, _uid, Req.index(), Req.type().c_str(), Req.candidate().c_str(), Req.sdpmlineindex(), Req.sdpmid().c_str());
// 	Req.set_peer(_uid);
// 	CIOBuffer* pBuffer = CIOBuffer::Alloc();
// 	MessageHead Head;
// 	Head.Command = RemoNet::SC_Candidate;
// 	Head.Length = Req.ByteSizeLong();
// 	Head.Serialize(pBuffer->Buffer);
// 	auto ptr = pBuffer->Buffer + MessageHead::Size();
// 	Req.SerializeToArray(ptr, Head.Length);
// 	pBuffer->Length = MessageHead::Size() + Head.Length;
// 	CUserManager::GetInstance().Write(peer, pBuffer);
// 	pBuffer->Release();

}
// void CUserSocket::OnCancelReq(int8_t* Data, int16_t Size)
// {
// 	RemoNet::CSCancelReq Req;
// 	Req.ParseFromArray(Data, Size);
// 	if (_peer != Req.peer()) return;
// 	CUserManager::GetInstance().CancelReq(_peer, _uid);
// 	_peer = -1;
// 
// 
// }
void CUserSocket::OnClose()
{
	 
	if (_egoType == EgoType::Car)
	{
		RemoNet::SCDelRobot rot;
		rot.set_peer(_uid);
		CIOBuffer* pBuffer = CIOBuffer::Alloc();
		MessageHead Head;
		Head.Command = RemoNet::SC_NotifyDel;
		Head.Length = rot.ByteSizeLong();
		Head.Serialize(pBuffer->Buffer);
		auto ptr = pBuffer->Buffer + MessageHead::Size();
		rot.SerializeToArray(ptr, Head.Length);
		pBuffer->Length = MessageHead::Size() + Head.Length;
		CUserManager::GetInstance().BroadCast(pBuffer, _cid);
		pBuffer->Release();
		
	}
	if (_userpeer != -1)
		CUserManager::GetInstance().LeavePeerVideo(_userpeer, _uid, _egoType);
	printf("a socket closed %d", _uid);
	CUserManager::GetInstance().Remove(this);
	Alloc_Pool<CUserSocket>::GetInstance().Free(this);
}
void CUserSocket::OnCreate()
{
	Read(0);
}
void CUserSocket::OnKeepAlive(int8_t* Data, int16_t Size)
{
	TimeTick = GetTickCount64();
}
 
void CUserSocket::OnOffer(int8_t* Data, int16_t Size)
{
	std::cout << __FUNCTION__ << std::endl;
	RemoNet::Offer Req;
	Req.ParseFromArray(Data, Size);
	int32_t peer = Req.peer();
	CUserManager::GetInstance().NotifyOffer(peer,_uid,Req.index(),Req.type().c_str(),Req.sdp().c_str());
// 	Req.set_peer(_uid);
// 	CIOBuffer* pBuffer = CIOBuffer::Alloc();
// 	MessageHead Head;
// 	Head.Command = RemoNet::SC_Offer;
// 	Head.Length = Req.ByteSizeLong();
// 	Head.Serialize(pBuffer->Buffer);
// 	auto ptr = pBuffer->Buffer + MessageHead::Size();
// 	Req.SerializeToArray(ptr, Head.Length);
// 	pBuffer->Length = MessageHead::Size() + Head.Length;
// 	CUserManager::GetInstance().Write(peer, pBuffer);
// 	pBuffer->Release();

}
void CUserSocket::OnRead()
{
	int16_t length = m_pBuffer->Length;
	int8_t* ptr = m_pBuffer->Buffer;
	while (true)
	{
		if (length < MessageHead::Size())
		{
			Read(length);
			return;
		}
		MessageHead Head;
		Head.Deserialize(ptr);// = reinterpret_cast<MessageHead *>(ptr);


		int16_t size = Head.Length + MessageHead::Size();
		if (length < size)
		{
			Read(length);
			return;
		}
		int8_t* Data = ptr + MessageHead::Size();

		Process(Head.Command, Data, Head.Length);
		length -= size;
		ptr += size;

	}
}
void CUserSocket::OnRepVideo(int8_t* Data, int16_t Size)
{
	std::cout << __FUNCTION__ << std::endl;
	RemoNet::CSRep Req;
	Req.ParseFromArray(Data, Size);
	int32_t peer = Req.peer();
	if (Req.desc() == RemoNet::VideoDesc::OK)
	    _userpeer = peer;
	CUserManager::GetInstance().ReplyPeerVideo(peer, _uid, Req.desc(), Req.index());

	
	
}
void CUserSocket::OnReqVideo(int8_t* Data, int16_t Size)
{
	std::cout << __FUNCTION__ << std::endl;
	RemoNet::CSReq Req;
	Req.ParseFromArray(Data, Size);
	int32_t peer = Req.peer();
	int32_t index = Req.index();
	int32_t type = Req.egotype();
	//bool master=Req.master();
	if (type==EgoType::Car&&index==RenderPosition::FRONT_BACK)
	{
		if (_userpeer == _uid || peer == _userpeer) return;
		_userpeer = peer;
	}
 
 
	auto state = CUserManager::GetInstance().ConnectPeerVideo(peer, _uid,index);
	 
		 
	RemoNet::SCReq Rep;
	Rep.set_peer(peer);
	Rep.set_desc(state);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_Req;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
}
 
void CUserSocket::OnSigin(int8_t* Data, int16_t Size)
{
	RemoNet::CSSign Req;
	Req.ParseFromArray(Data, Size);
	std::string account = Req.account();
	std::string password = Req.password();
	CQPtr<CConnectionPtr<sql::Connection>> Conn = CDBConnectPool::GetInstance().QueryConnect();

	scoped_ptr<sql::Statement> stmt = (*Conn.get())->createStatement();
	char sql[1024];
	 

	sprintf_s(sql, "select id,name,cid from user where account=\'%s\' and password=MD5(\'%s\')", account.c_str(), password.c_str());
	scoped_ptr<sql::ResultSet>  resultSet = stmt->executeQuery(sql);
	 
	bool bRet = false;
	if (resultSet->next())
	{
		int32_t uid = resultSet->getInt(1);
		CUserManager::GetInstance().Check(uid);
		_uid = uid;
		_name = resultSet->getString(2).c_str();
		_cid = resultSet->getInt(3);
		_loginsucc = true;
		_egoType = EgoType::User;
		
		bRet = true;
	}

	RemoNet::SCSign Rep;
	Rep.set_ret(bRet);
	Rep.set_uid(_uid);
	Rep.set_cid(_cid);
	Rep.set_name(_name);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_Sign;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
	

}
void CUserSocket::OnAdd(int8_t* Data, int16_t Size)
{
	RemoNet::CSAdd Req;
	Req.ParseFromArray(Data, Size);
	std::string serial = Req.serial();
	_name = Req.name();
	CQPtr<CConnectionPtr<sql::Connection>> Conn = CDBConnectPool::GetInstance().QueryConnect();

	scoped_ptr<sql::Statement> stmt = (*Conn.get())->createStatement();
	char sql[1024];


	sprintf_s(sql, "select id,cid from robot where serial=\'%s\'", serial.c_str());
	scoped_ptr<sql::ResultSet>  resultSet = stmt->executeQuery(sql);
	bool bRet = false;
	if (resultSet->next())
	{
		_uid = resultSet->getInt(1);
		_cid = resultSet->getInt(2);
		
		
		_loginsucc = true;
		_egoType =static_cast<EgoType>(Req.type());
		
		bRet = true;
	}
	if (bRet)
	{
		//_egoType = static_cast<EgoType>(Req.type());
// 		int32_t size = Req.info().size();
// 		for (int32_t i = 0; i < size; i++)
// 		{
// 			auto& node = Req.info(i);
// 			CameraInfo info;
// 			info.index = node.pos();
// 			info.label = node.label();
// 
// 			info.solution = static_cast<DisplayResolution>(node.solution());
// 			_cameraArray.push_back(info);
// 		}
		RemoNet::SCAddRobot robot;
		auto r=robot.mutable_robot();
		r->set_name(_name);
		r->set_rid(_uid);
		r->set_type(_egoType);
		r->set_state(RemoNet::Robot_RobotState::Robot_RobotState_Online);
// 		for (auto& node : _cameraArray)
// 		{
// 			auto info = r->add_info();
// 			info->set_pos(node.index);
// 			info->set_label(node.label);
// 			info->set_solution(node.solution);
// 		}
		CIOBuffer* pBuffer = CIOBuffer::Alloc();
		MessageHead Head;
		Head.Command = RemoNet::SC_NotifyAdd;
		Head.Length = robot.ByteSizeLong();
		Head.Serialize(pBuffer->Buffer);
		auto ptr = pBuffer->Buffer + MessageHead::Size();
		robot.SerializeToArray(ptr, Head.Length);
		pBuffer->Length = MessageHead::Size() + Head.Length;
		CUserManager::GetInstance().BroadCast(pBuffer, _cid);
		pBuffer->Release();
	}
	RemoNet::SCAdd Rep;
	Rep.set_ret(bRet);
	Rep.set_uid(_uid);
	Rep.set_cid(_cid);
	Rep.set_name(_name);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_Add;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
	 
}
void CUserSocket::Process(int32_t cmd, int8_t* Data, int32_t Size)
{
	if (!_loginsucc)
	{
		if ((cmd != RemoNet::CS_Sign && cmd != RemoNet::CS_Add)) return;
	}
	Redirector.Process(this, cmd, Data, Size);
}
RemoNet::VideoDesc CUserSocket::ReqVideo(int32_t peer, int32_t index)
{
	if (_egoType == EgoType::User) return RemoNet::VideoDesc::NoFound;
	//if(index!=RenderPosition::FRONT_BACK)
	if (_userpeer!=-1&&_userpeer != peer) return RemoNet::VideoDesc::Busy;
	
	RemoNet::CSReq Rep;
	Rep.set_peer(peer);
 
	Rep.set_index(index);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_NotifyReq;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();

	return RemoNet::VideoDesc::OK;
}

void CUserSocket::RepVideo(int32_t peer, int32_t index,RemoNet::VideoDesc desc)
{
	if (desc != RemoNet::VideoDesc::OK)
	{
		_userpeer = -1;
	}

	RemoNet::CSRep Rep;
	Rep.set_desc(desc);
	Rep.set_peer(peer);
	 
	Rep.set_index(index);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_NotifyRep;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
}
int32_t CUserSocket::uid()
{
	return _uid;
}
int32_t CUserSocket::cid()
{
	return _cid;
}
std::string CUserSocket::name()
{
	return _name;
}
int32_t CUserSocket::type()
{
	return _egoType;
}
void CUserSocket::CloseVideo(int32_t peer, int32_t index, EgoType type)
{
	if (type == EgoType::User)
	{
		if (_userpeer != peer) return;
		 
	}
	RemoNet::Close Rep;
	Rep.set_egotype(type);
	Rep.set_index(index);
	Rep.set_peer(peer);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_NotifyCloseVideo;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
}
void CUserSocket::LeaveVideo(int32_t peer,EgoType type)
{
	if (_userpeer != peer) return;
	_userpeer = -1;
	RemoNet::Leave Rep;
	Rep.set_egotype(type);
	Rep.set_peer(peer);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_NotifyLeave;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
}
 
void CUserSocket::KickOff()
{
	RemoNet::SCKickOff Rep;
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_KickOff;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
	_loginsucc = false;
	_uid = -1;
}
void CUserSocket::OnRobot(int8_t* Data, int16_t Size)
{
	RemoNet::CSRobot Req;
	Req.ParseFromArray(Data, Size);
	std::vector<Robot> ret;
	CUserManager::GetInstance().GetRobot(_cid, ret);
	RemoNet::SCRobot Rep;
	for (auto& node : ret)
	{
		auto robot = Rep.add_robot();
		robot->set_name(node.name.c_str());
		robot->set_rid(node.uid);
		robot->set_type(node.type);
		robot->set_state(node.state);;
	 
	
	}
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_Robot;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
}
bool CUserSocket::IsBusy()
{
	return _userpeer != -1;
}
void CUserSocket::NotifyOffer(int32_t peer, int32_t index,const char * type,const char * sdp)
{
//	std::cout << sdp << std::endl;
	RemoNet::Offer  Rep;
	Rep.set_peer(peer);
	Rep.set_index(index);
	Rep.set_type(type);
	Rep.set_sdp(sdp);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_NotifyOffer;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
}
void CUserSocket::NotifyAnswer(int32_t peer, int32_t index, const char* type, const char* sdp)
{
	//std::cout << sdp << std::endl;
	RemoNet::Answer Rep;
	Rep.set_peer(peer);
	Rep.set_index(index);
	Rep.set_type(type);
	Rep.set_sdp(sdp);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_NotifyAnswer;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();
}
void CUserSocket::NotifyCandidate(int32_t peer, int32_t index, const char* type, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid)
{
	RemoNet::Candidate Rep;
	Rep.set_peer(peer);
	Rep.set_index(index);
	Rep.set_type(type);
	Rep.set_sdpmid(sdp_mid);
	Rep.set_candidate(candidate);
	Rep.set_sdpmlineindex(sdp_mline_index);
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	MessageHead Head;
	Head.Command = RemoNet::SC_NotifyCandidate;
	Head.Length = Rep.ByteSizeLong();
	Head.Serialize(pBuffer->Buffer);
	auto ptr = pBuffer->Buffer + MessageHead::Size();
	Rep.SerializeToArray(ptr, Head.Length);
	pBuffer->Length = MessageHead::Size() + Head.Length;
	Write(pBuffer);
	pBuffer->Release();

}
void CUserSocket::OnTimeout()
{
	PostQueuedCompletionStatus(m_hComplete, -1, (DWORD_PTR)this, nullptr);
}
ULONGLONG CUserSocket::GetTimeTick()
{
	return TimeTick;
}