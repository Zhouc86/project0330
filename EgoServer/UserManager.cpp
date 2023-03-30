#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <mutex>
#include <functional>
#include "../Protocol/win/Protocol.pb.h"
// #include "rapidjson/rapidjson.h"
// #include "rapidjson/document.h"
// #include "rapidjson/istreamwrapper.h"
// #include "rapidjson/stringbuffer.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/document.h"
//#include "WebHandler.h"
#include "UserSocket.h"
#include "UserManager.h"
#include "../common/IOBuffer.h"
//#include "WebServer.h"

const ULONGLONG  MAX_DELAY_TIME = 30000;

CUserManager& CUserManager::GetInstance()
{
	static CUserManager m;
	return m;
}
void CUserManager::Check(int32_t uid)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto it = SocketList.begin(); it != SocketList.end(); ++it)
	{
		CUserSocket* User = *it;
		if (User->uid() == uid)
		{
			User->KickOff();
			break;
		}
	}
}
void CUserManager::Add(CUserSocket* ptr)
{

	std::lock_guard<std::mutex> l(_lock);
	SocketList.push_back(ptr);

// 	rapidjson::StringBuffer strBuf;
// 	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
// 	root.StartObject();
// 	root.Key("type");
// 	root.String(kNotify);
// 	root.Key("uid");
// 	root.Int(ptr->GetId());
// 	root.Key("state");
// 	root.Bool(true);
// 	root.EndObject();
// 
// 	BroadCastUserState(ptr->GetId(), strBuf.GetString());



}

void CUserManager::Remove(CUserSocket* lhs)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto it = SocketList.begin(); it != SocketList.end(); ++it)
	{
		CUserSocket* User = *it;
		if (User == lhs)
		{
			SocketList.erase(it);
			break;
		}
	}
}
void CUserManager::Write(int32_t uid, CIOBuffer* pBuffer)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == uid)
		{
			a->Write(pBuffer);
			break;
		}
	}
}


RemoNet::VideoDesc CUserManager::ConnectPeerVideo(int32_t peer, int32_t uid, int32_t index)
 {
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == peer)
		{
			return a->ReqVideo(uid, index);
			
		}
	}
	 
	return RemoNet::VideoDesc::NoFound;
}

 
void CUserManager::LeavePeerVideo(int32_t peer, int32_t uid,EgoType type)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == peer)
		{

			a->LeaveVideo(uid,type);
			break;
		}
	}
}
void CUserManager::ClosePeerVideo(int32_t peer, int32_t uid, EgoType type, int32_t index)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == peer)
		{
			a->CloseVideo(uid, index, type);
			break;
		}
	}
}
void CUserManager::Start()
{
	_thread = std::thread(std::bind(&CUserManager::Run, this));
}
void CUserManager::Run()
{
	
	_run = true;
	
	while (_run)
	{		
		ULONGLONG tick = GetTickCount64();
		{
			std::unique_lock<std::mutex> l(_lock);

				for (auto it = SocketList.begin(); it != SocketList.end();)
				{
					CUserSocket* lhs = *it;
					if (tick - lhs->GetTimeTick() > MAX_DELAY_TIME)
					{

						lhs->OnTimeout();
						it = SocketList.erase(it);

					}
					else
					{
						++it;
					}
				}
			}
			Sleep(5000);
		}
 
	 

}
void CUserManager::ReplyPeerVideo(int32_t peer, int32_t uid, RemoNet::VideoDesc ret, int32_t index)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == peer)
		{

			a->RepVideo(uid, index, ret);
			break;
		}
	}

}
 
RemoNet::Robot::RobotState CUserManager::GetState(int32_t uid)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == uid)
		{
			return a->IsBusy() ? RemoNet::Robot::RobotState::Robot_RobotState_Busy : RemoNet::Robot::RobotState::Robot_RobotState_Online;
		}
	}
	return RemoNet::Robot::RobotState::Robot_RobotState_Offline;
}
 
void CUserManager::NotifyOffer(int32_t peer, int32_t uid, int32_t index, const char* type, const char* sdp)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == peer)
		{
			a->NotifyOffer(uid, index, type, sdp);
			break;
		}
	}
}
void CUserManager::NotifyAnswer(int32_t peer, int32_t uid, int32_t index, const char* type, const char* sdp)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == peer)
		{
			a->NotifyAnswer(uid, index, type, sdp);
			break;
		}
	}
}
void CUserManager::NotifyCandidate(int32_t peer, int32_t uid, int32_t index, const char* type, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->uid() == peer)
		{
			a->NotifyCandidate(uid, index, type, candidate, sdp_mline_index, sdp_mid);
			break;
		}
	}
}
void CUserManager::GetRobot(int32_t cid, std::vector<Robot>& ret)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->type() != EgoType::User&&a->cid()==cid)
		{
			Robot m;
			m.cid = a->cid();
			m.name = a->name();
			m.uid = a->uid();
			m.type = a->type();
			m.state= a->IsBusy() ? RemoNet::Robot::RobotState::Robot_RobotState_Busy : RemoNet::Robot::RobotState::Robot_RobotState_Online;
			ret.push_back(m);
		}
	}
}
void CUserManager::BroadCast(CIOBuffer* pBuffer,  int32_t cid)
{
	std::lock_guard<std::mutex> l(_lock);
	for (auto& a : SocketList)
	{
		if (a->type() == EgoType::User && a->cid() == cid)
		{
			a->Write(pBuffer);
		}
	}

}
/*
void CUserManager::BroadCastUserState(int32_t uid, const char* content)
{
	//std::shared_lock<std::shared_mutex> l(lock);
	for (auto& node : UserMap)
	{
		if (auto p = node.second.lock())
		{
			if (p->GetUID() != uid)
			{
				p->Notify(content);
			}
		}

	}
}
 */