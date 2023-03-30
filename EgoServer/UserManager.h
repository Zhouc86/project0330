#pragma once
#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include <mutex>
#include "../Protocol/win/Protocol.pb.h"
#include "../common/comm.h"
class CUserSocket;
class CIOBuffer;
struct Robot
{
	int32_t uid;
	int32_t cid;
	int32_t type;
	std::string name;
	RemoNet::Robot::RobotState state;
};
class CUserManager
{
public:
	void Add(CUserSocket* ptr);
	void Check(int32_t uid);
	void Remove(CUserSocket* lhs);
	void Write(int32_t id, CIOBuffer* pBuffer);
	RemoNet::Robot::RobotState GetState(int32_t uid);
	 
	static CUserManager& GetInstance();

	RemoNet::VideoDesc ConnectPeerVideo(int32_t peer, int32_t uid, int32_t index);
	void NotifyOffer(int32_t peer, int32_t uid, int32_t video, const char* type, const char* sdp);
	void NotifyAnswer(int32_t peer, int32_t uid, int32_t video, const char* type, const char* sdp);
	void NotifyCandidate(int32_t peer, int32_t uid, int32_t video, const char* type, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid);
	void ReplyPeerVideo(int32_t peer, int32_t uid, RemoNet::VideoDesc ret, int32_t index);
	void LeavePeerVideo(int32_t peer, int32_t uid, EgoType type);
	void ClosePeerVideo(int32_t peer, int32_t uid, EgoType type,int32_t index);
	 
	void GetRobot(int32_t cid, std::vector<Robot>& ret);
	void BroadCast(CIOBuffer* pBuffer, int32_t cid);
	 
	void Start();
	void Run();

private:
	std::vector<CUserSocket*> SocketList;
	 
	std::mutex _lock;
	std::thread _thread;
	bool _run;
};