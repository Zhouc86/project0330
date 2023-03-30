// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "api.h"
#include "Protocol.pb.h"
#include "../common/socket_client.h"
 
#include "../common/update_thread.h"
//#include "../common/FeedBack.h"


 
//#include "MonitorThread.h"
//#define WM_NOTIFY_STATE WM_APP+10
//#define MSG_WM_NOTIFY_STATE(func) if(uMsg==WM_NOTIFY_STATE){SetMsgHandled(TRUE); func((FeedbackData::Feedback *)wParam); lResult = 0; if(IsMsgHandled()) return TRUE;}
//#define WM_NOTIFY_REP WM_APP+2
//#define MSG_WM_NOTIFY_REP(func) if(uMsg==WM_NOTIFY_REP){SetMsgHandled(TRUE); func((int32_t)wParam); lResult = 0; if(IsMsgHandled()) return TRUE;}
//#define WM_NOTIFY_ADDTRACK WM_APP+3
//#define MSG_WM_NOTIFY_ADDTRACK(func) if(uMsg==WM_NOTIFY_ADDTRACK){SetMsgHandled(TRUE); func(); lResult = 0; if(IsMsgHandled()) return TRUE;}
// #define WM_NOTIFY_ANSWER WM_APP+4
// #define MSG_WM_NOTIFY_ANSWER(func) if(uMsg==WM_NOTIFY_ANSWER){SetMsgHandled(TRUE); func((IOBuffer *)wParam); lResult = 0; if(IsMsgHandled()) return TRUE;}
// #define WM_NOTIFY_INITWEBRTC  WM_APP+5
// #define MSG_WM_NOTIFY_INITWEBRTC  if(uMsg==WM_NOTIFY_INITWEBRTC){SetMsgHandled(TRUE); func(); lResult = 0; if(IsMsgHandled()) return TRUE;}
#define WM_NOTIFYCONNECTED WM_APP+100
#define MSG_WM_NOTIFYCONNECTED(func) if(uMsg==WM_NOTIFYCONNECTED){SetMsgHandled(TRUE); func((bool)wParam); lResult = 0; if(IsMsgHandled()) return TRUE;}
#define WM_NOTIFYSIGIN WM_APP+101
#define MSG_WM_NOTIFYSIGIN(func) if(uMsg==WM_NOTIFYSIGIN){SetMsgHandled(TRUE); func((bool)wParam); lResult = 0; if(IsMsgHandled()) return TRUE;}
#define WM_NOTIFYREADY WM_APP+102
#define MSG_WM_NOTIFYREADY(func) if(uMsg==WM_NOTIFYREADY){SetMsgHandled(TRUE); func(); lResult = 0; if(IsMsgHandled()) return TRUE;}
#define WM_NOTIFYSTOP WM_APP+103
#define MSG_WM_NOTIFYSTOP(func) if(uMsg==WM_NOTIFYSTOP){SetMsgHandled(TRUE); func(); lResult = 0; if(IsMsgHandled()) return TRUE;}
#define WM_NOTIFYCONNECT WM_APP+104
#define MSG_WM_NOTIFYCONNECT(func) if(uMsg==WM_NOTIFYCONNECT){SetMsgHandled(TRUE); func((int32_t)wParam); lResult = 0; if(IsMsgHandled()) return TRUE;}
#define WM_NOTIFYDISCONNECT WM_APP+105
#define MSG_WM_NOTIFYDISCONNECT(func) if(uMsg==WM_NOTIFYDISCONNECT){SetMsgHandled(TRUE); func((int32_t)wParam); lResult = 0; if(IsMsgHandled()) return TRUE;}
//#define  IDC_STATETIMER  0x1002
#define WM_STOP_ACK WM_APP+106
#define MSG_STOP_ACK(func) if(uMsg==WM_STOP_ACK){SetMsgHandled(TRUE); func(); lResult = 0; if(IsMsgHandled()) return TRUE;}
 
class CThreadWindow;
class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler,public INativeNotify
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	virtual BOOL OnIdle() override;

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)


		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_NOTIFYCONNECTED(OnNotifyConnected)
		MSG_WM_NOTIFYSIGIN(OnNotifySigin)
		MSG_WM_TIMER(OnTimer)
		MSG_STOP_ACK(OnStopAck)
		//MSG_WM_NOTIFYREADY(OnNotifyReady)
		//MSG_WM_NOTIFYCONNECT(OnNotifyConnect)
		//MSG_WM_NOTIFYDISCONNECT(OnNotifyDisconnect)
		//MSG_WM_NOTIFYSTOP(OnNotifyStop)
		//MSG_WM_NOTIFY_STATE(OnNotifyState)
		//MSG_WM_NOTIFY_REP(OnNotifyRep)

		COMMAND_ID_HANDLER_EX(IDC_CARDISCONNECT, OnCarLeave)
		COMMAND_ID_HANDLER_EX(IDC_FRONT, OnFront)
		COMMAND_ID_HANDLER_EX(IDC_BACK, OnBack)
		COMMAND_ID_HANDLER_EX(IDC_CARCONNECT,OnCarConnect)
		COMMAND_ID_HANDLER_EX(IDC_SEND,OnSend)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(HWND, LPARAM);
	void OnFront(WORD, WORD, HWND);
	void OnBack(WORD, WORD, HWND);
	void OnCarConnect(WORD, WORD, HWND);
	void OnCarLeave(WORD, WORD, HWND);
	void OnSend(WORD, WORD, HWND);
	void OnDestroy(void);
	void OnStopAck(void);
	void OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/);
 	void CloseDialog(int nVal);
// 	void OnNotifyReady();
// 	void OnNotifyStop();
// 	void OnNotifyConnect(int32_t uid);
// 	void OnNotifyDisconnect(int32_t uid);
	virtual void OnSigin(bool bRet) override;
	void OnNotifySigin(bool bRet);
	virtual void OnRobot(const RemoNet::Robot& robot) override;
	virtual void OnConnected(bool bRet) override;
	 
	void OnNotifyConnected(bool bRet);
 

	void OnTimer(UINT id);
	//void OnNotifyState(FeedbackData::Feedback* data);
#ifndef WIN32
	virtual void OnVideoReq(int32_t index, int32_t peer) override;
#endif
	virtual void OnVideoRep(int32_t index, int32_t peer) override;
	virtual void OnVideoOffer(int32_t index, const char* type, const char* sdp) override;
	virtual void OnVideoAnswer(int32_t index, const char* type, const char* sdp) override;
	virtual void OnVideoCandidate(int32_t index, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid) override;
	//virtual void OnCancelReq(int32_t index) override;
	virtual void OnVideoLeave(int32_t peer,EgoType type) override;
	virtual void OnMessageFrameNotify(ChannelType type, int16_t cmd, int16_t length, const void* data) override;
	virtual void OnNotifyKick() override;
	virtual void OnNotifyDel(int32_t peer, EgoType type) override;

	

private:
	std::shared_ptr<UserCamera> _cameraInfo;
	std::shared_ptr<SocketClient>  _client;
	//std::unique_ptr<SocketCan> _can;
//	std::unique_ptr<MonitorThread> _monitor;
	std::string _name;
	std::string _account;
	std::string _password;
	std::vector<std::unique_ptr<CThreadWindow>> _WindowArray;
	/*std::vector<std::unique_ptr<CRadarWindow>> _RadarArray;*/
	//std::vector<Coord> _ptArray[3];
	bool bUpdated = false;
	//bool bNotifyReady = false;
	std::unique_ptr<SensorSocket<CControlSensor>> _control;
private:
	
	int32_t _carpeer;
	std::vector<std::unique_ptr<UserCamera> > _userInfo;
	CUpdateThread _updateThread;
};