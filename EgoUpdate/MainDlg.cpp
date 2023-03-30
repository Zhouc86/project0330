// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

 
#include <vector>
#include <string>

#include <json/json.h>
#include <json/writer.h>
#include <fstream>
#include "7z/LzmaDec.h"
#include "7z/7ZItf.h"
#include <curl/curl.h>
#include "../common/comm.h"
#include <TlHelp32.h>
#include "MainDlg.h"
 

LRESULT CMainDlg::OnInitDialog(HWND, LPARAM )
{
	// center the dialog on the screen
	CenterWindow();
	Count=1;
	
	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	_thread = std::thread(std::bind(&CMainDlg::UpdateThread, this));
	SetTimer(IDC_TIMER, 500);
	return TRUE;
}
void CMainDlg::OnTimer(UINT id)
{
	std::string text;
	for (int i = 0; i < Count; i++)
	{
		text += ".";
	}
	Count++;
	if (Count == 4)
		Count = 0;
	SetDlgItemText(IDC_PREFIX, text.c_str());
}
void CMainDlg::UpdateThread()
{
	LoadConfig();
	char szurl[256];
	sprintf(szurl, "%s/iego.json", _url.c_str());
	char szPath[MAX_PATH],szfile[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	 
	DownloadFile(szurl,"iego.json");
	Json::Value root;
	Json::Reader jsonReader;
	std::ifstream ifile("iego.json");
	int32_t version = -1;
	if (jsonReader.parse(ifile, root))
	{
		 
		version = root["host_ver"].asInt();
		 
	}
	ifile.close();
	DeleteFile("iego.json");
	if (_version < version)
	{
		std::string file = root["host_file"].asString();

		char szurl[256];
		sprintf(szurl, "%s/%s", _url.c_str(), file.c_str());
		DownloadFile(szurl, file.c_str());
		Decompress(file.c_str());
		DeleteFile(file.c_str());
		WriteConfig(version);


	}
	sprintf_s(szfile, "%s\\EgoWindow.exe", szPath);
	//LPPROCESS_INFORMATION info = NULL;
	STARTUPINFO si = { sizeof(si) };
	ShellExecute(NULL,"open",szfile, NULL, szPath,SW_SHOW);
	PostMessage(WM_COMPLETE);
 
}

 
void CMainDlg::OnKillProcess()
{
	int * nCount = 0;
	 
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32进程快照的结构体  
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hSnapShot, &pe))
	{
		while (Process32Next(hSnapShot, &pe))
		{
			if (stricmp(pe.szExeFile, "EgoWindow.exe") == 0)
			{
				DWORD dwProcessID = pe.th32ProcessID;
				HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
				::TerminateProcess(hProcess, 0);  //杀死进程
				CloseHandle(hProcess);
				 
			}
		}
		CloseHandle(hSnapShot);
	}
	

// 	do{
// 		nCount = 0;
// 
// 	EnumWindows(lpEnumFunc, (LPARAM)&nCount);
// 	if (nCount > 0)
// 		Sleep(100);
// 	} while (nCount > 0);


}

void CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND)
{
	EndDialog(wID);
 
}
void CMainDlg::OnComplete()
{
	_thread.join();
	EndDialog(0);

}

CURLcode CMainDlg::DownloadFile(const char * url,const char * file)
{
	FILE* fp = fopen(file, "wb+");
	CURL* curl = curl_easy_init();

	if (curl == NULL)
	{
		//抛出错误异常
		throw std::runtime_error("create easy_handle fail");
	}
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CMainDlg::DownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

	//开始进行请求
	CURLcode retCode = curl_easy_perform(curl);

	//清理curl_easy
	curl_easy_cleanup(curl);
	fclose(fp);
	return retCode;
}

bool  CMainDlg::LoadConfig()
{
	Json::Value root;
	Json::Reader jsonReader;
	std::ifstream ifile("./Config.json");
	if (jsonReader.parse(ifile, root))
	{
		_url = root["url"].asString();
		 
		_version = root["version"].asInt();
		return true;

	}
	return false;
}

void CMainDlg::WriteConfig(int32_t version)
{
	Json::Value root;
	Json::Reader jsonReader;
	std::ifstream ifile("Config.json");
	if (jsonReader.parse(ifile, root))
	{
		ifile.close();
		root["version"] = version;
		Json::StreamWriterBuilder b;
		std::unique_ptr<Json::StreamWriter> sw(b.newStreamWriter());
		//sw.Write(root);
		std::ofstream ofile;
		ofile.open("Config.json", std::ios::out);
		sw->write(root, &ofile);
		ofile.close();

	}
}
size_t CMainDlg::DownloadCallback(void* pBuffer, size_t nSize, size_t nMemBytes, FILE* fp)
{
	size_t nWrite = fwrite(pBuffer, nSize, nMemBytes, fp);

	return nWrite;
}