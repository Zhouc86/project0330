// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <thread>
#include <functional>
#define WM_KILLPROPCESS WM_APP+1
#define  MSG_WM_KILLPROPCESS(func) if(uMsg==WM_KILLPROPCESS){ SetMsgHandled(TRUE);func();if(IsMsgHandled()) return TRUE;}
#define WM_COMPLETE WM_APP+2
#define  MSG_WM_COMPLETE(func) if(uMsg==WM_COMPLETE){ SetMsgHandled(TRUE);func();if(IsMsgHandled()) return TRUE;}
#define IDC_TIMER 2000
class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		MSG_WM_KILLPROPCESS(OnKillProcess);
    	MSG_WM_COMPLETE(OnComplete);
		MSG_WM_TIMER(OnTimer)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	void OnKillProcess();
	void OnComplete();
	void OnTimer(UINT id);
	LRESULT OnInitDialog(HWND,LPARAM);
	 
	void OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND );
private:
	void UpdateThread();
private:
	CURLcode DownloadFile(const char * url, const char* file);
	static size_t DownloadCallback(void* pBuffer, size_t nSize, size_t nMemBytes, FILE* fp);
	bool LoadConfig();
	void WriteConfig(int32_t version);
	std::thread _thread;
	int Count;
	std::string _url;
	int32_t _version;
	
};
