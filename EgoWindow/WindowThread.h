#pragma once
#include "../common/comm.h"
#define WM_NOTIFY_REP WM_APP+1
#define WM_NOTIFY_ANSWER WM_APP+2
#define WM_NOTIFY_LEAVE WM_APP+3
#define WM_NOTIFY_CANDIDATE WM_APP+4
#define WM_NOTIFY_OFFER  WM_APP+5
 
enum ControlState
{
	Check,
	Process,
};

//class CVideoRenderer;
class CPeerConnection;
class CThreadWindow//:public ISensorNot
{
public: 
	CThreadWindow(RenderPosition pos, SocketClient *c);
	void Start();
	void Run();
	void Stop();
	void PostMessage(UINT wMsg, WPARAM wParam=NULL, LPARAM lParam=NULL);
	 
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
	void SetPeer(int32_t peer);
	void CreateVideoReq();
	void DelayNextVideoReq();
	void DelayDataChannel();
	void DelayControlStart();
	ControlState GetControlState();
	//virtual void Notify(int8_t* buffer, int32_t size) override;
	//virtual void SetCanSocket(SocketCan* can) override;
	//virtual void WriteCanMessage(std::unordered_map<int32_t, cannet_frame>& node,bool bLidar) override;
	void SendData(CIOBuffer* pBuffer);
private:
	void InitPeerConnection(int32_t peer);
	void OnVideoRep(int32_t peer);
	void OnVideoAnswer(CIOBuffer * pBuffer);
	void OnVideoLeave();
	void OnVideoCandidate(CIOBuffer* pBuffer);
	void OnVideoOffer(CIOBuffer* pBuffer);
private:
	void SetRenderWindow(int32_t width, int32_t height);
	void StartRender(bool b);
	static   void  FrameCallback(void* user_data,
		const uint8_t* yptr, int32_t strideY,
		const uint8_t* uptr, int32_t strideU,
		const uint8_t* vptr, int32_t strideV,
		const int32_t stride, const int frame_width,
		const int frame_height);
	void OnArgb32FrameReady(const uint8_t* yptr, int32_t strideY, const uint8_t* uptr, int32_t strideU, const uint8_t* vptr, int32_t strideV, const int32_t stride, const int frame_width, const int frame_height);
	void OnPaint(HWND hWnd);
	void SetSize(int32_t width, int32_t height);
	void AskVideoReq();
	void AskDataChannel();
	void AskControlStart();
	
private:
	 
	HWND m_hWnd;
//	std::unique_ptr<CVideoRenderer> m_VideoRender;
	std::unique_ptr<CPeerConnection> m_PeerConnection;
	std::thread m_Thread;
	SocketClient* m_Client;
	bool m_bRun;
	ControlState _control;
	 
	int32_t m_Peer;
	RenderPosition m_Pos;
	BITMAPINFO bmi_;	
	bool start_;
	//bool bDataChannelCreated;
	std::mutex buffer_lock_;
	std::unique_ptr<uint8_t> image_;
	std::unique_ptr<uint8_t> _rotate;
	std::thread m_DelayThread;
	 

};
 
