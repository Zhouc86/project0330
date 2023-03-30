#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include "IOSocket.h"
#include <vector>
#include "../common/comm.h"
template<typename T>
class  FNRedirector
{
public:

	typedef void (T::* FN)(int8_t*, int16_t Size);
	bool Process(T* lhs, int32_t cmd, int8_t*, int16_t);

	void Insert(int32_t, FN fn);
private:
	std::unordered_map<int32_t, FN> m_fnmap;

};

template<typename T>
bool FNRedirector<T>::Process(T* lhs, int32_t cmd, int8_t* Data, int16_t Size)
{

	auto it = m_fnmap.find(cmd);
	if (it != m_fnmap.end())
	{

		(lhs->*it->second)(Data, Size);
		return true;
	}
	return false;
}

template<typename T>
void FNRedirector<T>::Insert(int32_t cmd, FN fn)
{
	m_fnmap.insert(std::make_pair(cmd, fn));
}
 
 
class  CUserSocket :public CIOSocket
{
public:
	CUserSocket(HANDLE hCom, SOCKET s);
	virtual void OnCreate() override;
	virtual void OnRead() override;
	void Process(int32_t cmd, int8_t*, int32_t Size);
	static void InitFnDirector();
	virtual void OnClose() override;
	static FNRedirector<CUserSocket> Redirector;
	int32_t uid();
	int32_t cid();
	int32_t type();
	std::string name();
	RemoNet::VideoDesc ReqVideo(int32_t peer,int32_t index);
	void RepVideo(int32_t peer, int32_t index, RemoNet::VideoDesc desc);
	void LeaveVideo(int32_t peer,EgoType type);
	void CloseVideo(int32_t peer, int32_t index, EgoType type);
	//void CancelReq(int32_t peer);
	bool IsBusy();
	void KickOff();
	ULONGLONG GetTimeTick();
	void NotifyOffer(int32_t peer, int32_t index, const char* type, const char* sdp);
	void NotifyAnswer(int32_t peer, int32_t index, const char* type, const char* sdp);
	void NotifyCandidate(int32_t peer, int32_t index, const char* type, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid);
	void OnTimeout();
	 
private:
	void OnReqVideo(int8_t* Data, int16_t Size);
	void OnRepVideo(int8_t* Data, int16_t Size);
	//void OnCancelReq(int8_t* Data, int16_t Size);
 
	void OnOffer(int8_t* Data, int16_t Size);
	void OnAnswer(int8_t* Data, int16_t Size);
	void OnCadidate(int8_t* Data, int16_t Size);

	void OnSigin(int8_t* Data, int16_t Size);
	void OnAdd(int8_t* Data, int16_t Size);
	void OnRobot(int8_t* Data, int16_t Size);
	void OnLeave(int8_t* Data, int16_t Size);
	//void OnCloseVideo(int8_t* Data,int16_t Size);
	void OnKeepAlive(int8_t* Data, int16_t Size);

private:
	bool _loginsucc;
	int32_t _cid;
	int32_t _uid;
	 
 
	EgoType _egoType;
	int32_t _userpeer;
	 
	 
	std::string _name;
	ULONGLONG TimeTick;
};

