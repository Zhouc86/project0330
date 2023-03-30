// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <thread>
#include <functional>
#include <vector>
#include "EgoInterface.h"
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
//#define WM_STOP_ACK WM_APP+106
//#define MSG_STOP_ACK(func) if(uMsg==WM_STOP_ACK){SetMsgHandled(TRUE); func(); lResult = 0; if(IsMsgHandled()) return TRUE;}
class CRenderWindow;
class CMainDlg : public CDialogImpl<CMainDlg>,public IEgoNotify
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)


		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_NOTIFYCONNECTED(OnNotifyConnected)
		MSG_WM_NOTIFYSIGIN(OnNotifySigin)
		 
		//MSG_STOP_ACK(OnStopAck)
		//MSG_WM_NOTIFYREADY(OnNotifyReady)
		//MSG_WM_NOTIFYCONNECT(OnNotifyConnect)
		//MSG_WM_NOTIFYDISCONNECT(OnNotifyDisconnect)
		//MSG_WM_NOTIFYSTOP(OnNotifyStop)
		//MSG_WM_NOTIFY_STATE(OnNotifyState)
		//MSG_WM_NOTIFY_REP(OnNotifyRep)

		COMMAND_ID_HANDLER_EX(IDC_CARDISCONNECT, OnCarLeave)
		 
		COMMAND_ID_HANDLER_EX(IDC_CARCONNECT, OnCarConnect)
	 
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	 
	LRESULT OnInitDialog(HWND,LPARAM);
	 
	void OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND );
	virtual void OnRobot(std::unique_ptr<UserCamera>& info) override;
	virtual void OnSigin(bool bRet) override;
	virtual void OnNotifyDel(int32_t peer) override;
	virtual void OnNotifyKickOff() override;
	virtual void OnNotifyRadar(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5) override;
	void OnNotifySigin(bool bRet);
	virtual void OnNotifyPing(int32_t value) override;
	void OnDestroy(void);
	virtual void OnNotifyImu(int32_t x, int32_t y) override;
	//void OnStopAck(void);
 
	void OnNotifyConnected(bool bRet);
	void OnCarConnect(WORD, WORD, HWND);
	void OnCarLeave(WORD, WORD, HWND);
private:
	std::vector<std::unique_ptr<UserCamera> > _userInfo;
	std::vector<std::unique_ptr<CRenderWindow>> _WindowArray;
	IEgoControl* _control;
	std::string _name;
	std::string _account;
	std::string _password;
	int32_t _carpeer = -1;
	
};
