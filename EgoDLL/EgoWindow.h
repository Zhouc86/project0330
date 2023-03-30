#pragma once
#include "api.h"
#include "message_queue.h"

 
#define WM_NOTIFY_REP WM_APP+1
#define WM_NOTIFY_ANSWER WM_APP+2
#define WM_NOTIFY_LEAVE WM_APP+3
#define WM_NOTIFY_CANDIDATE WM_APP+4
#define WM_NOTIFY_OFFER  WM_APP+5
class CPeerConnection;
class IRender;
enum ControlState
{
	Check,
	Process,
};
 
class CEgoWindow : public CMessageQueue 
{
public:
	CEgoWindow(IRender * render,RenderPosition pos, SocketClient* c);
	virtual void Start() override;
	
	virtual void Process(CIOBuffer* pBuffer) override;
	virtual void Stop() override;
	virtual void OnIdle() override;
	void PostMessage(int32_t type, int64_t l = 0, int64_t r = 0);
	void SetPeer(int32_t peer);
	void CreateVideoReq();
	void DelayNextVideoReq();
	void DelayDataChannel();
	void DelayStartPing();
	void SendData(CIOBuffer* pBuffer);
	void SetPeerNotify();
	//virtual void WriteCanMessage(std::unordered_map<int32_t, cannet_frame>& node, bool bLidar) override;
	 
	void OnNotifyRep(int32_t peer);
	void OnNotifyAnswer(CIOBuffer* pBuffer);
	void OnNotifyLeave();
	void OnNotifyCandidate(CIOBuffer* pBuffer);
	void OnNotifyOffer(CIOBuffer* pBuffer);
	static  void FrameCallback(void* user_data,
		const uint8_t* yptr, int32_t strideY,
		const uint8_t* uptr, int32_t strideU,
		const uint8_t* vptr, int32_t strideV,
		const int32_t stride, const int frame_width,
		const int frame_height);
	void OnArgb32FrameReady(const uint8_t* yptr, int32_t strideY, const uint8_t* uptr, int32_t strideU, const uint8_t* vptr, int32_t strideV, const int32_t stride, const int frame_width, const int frame_height);
	void SetSize(int32_t width, int32_t height);
	void AskVideoReq();
	void AskDataChannel();
	void StartPing();
	void StopPing();
	ControlState GetControlState();
	void SetControlState(ControlState state);
private:
	void InitPeerConnection(int32_t peer);
	void SetRenderWindow(int32_t width, int32_t height);
	void StartRender(bool b);
	

private:
	 
	std::unique_ptr<CPeerConnection> _peerconnection;
	int32_t _peer;
	RenderPosition _pos;
	BITMAPINFO bmi_;
	SocketClient* _client;
	bool start_;
	//bool _channelcreated;
	std::mutex buffer_lock_;
	std::unique_ptr<uint8_t> image_;
	std::unique_ptr<uint8_t> _rotate;
	std::thread _delayThread;
	IRender* _render;
	 
	ControlState _control;
	//std::unique_ptr<IPeerNotify> _peerNotify;
};

