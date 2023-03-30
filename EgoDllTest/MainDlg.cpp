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
#include <array>
 
#include "../common/comm.h"
#include "RenderWindow.h"
#include "MainDlg.h"
 

LRESULT CMainDlg::OnInitDialog(HWND, LPARAM)
{
	// center the dialog on the screen
	CenterWindow();


	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	_control = GetEgoController(this);
	std::array<IRender*, RenderPosition::ALL> _array;
	for (int32_t i = 0; i < RenderPosition::ALL; i++)
	{
		auto p = std::make_unique<CRenderWindow>();
		p->Start();
		_array[i] = p.get();
		_WindowArray.push_back(std::move(p));
	}
	_control->Start(_array);

	Json::Value root;
	Json::Reader jsonReader;
	std::ifstream ifile("Config.json");
	std::string ip;
	if (jsonReader.parse(ifile, root))
	{
		_name = root["name"].asString();
		_account = root["account"].asString();
		_password = root["password"].asString();
		_control->Login(_account, _password);
	}
	return TRUE;
}
void CMainDlg::OnDestroy()
{

}
void CMainDlg::OnNotifyConnected(bool bRet)
{

}
void CMainDlg::OnNotifySigin(bool ret)
{
	 
}
void CMainDlg::OnNotifyImu(int32_t x, int32_t y)
{

}
void CMainDlg::OnCarConnect(WORD, WORD, HWND)
{
	if (_carpeer != -1) return;
	CListViewCtrl box;
	box.Attach(GetDlgItem(IDC_CARLIST));

	int32_t cursel = box.GetSelectedIndex();
	if (cursel == -1) return;
	 
	_carpeer = box.GetItemData(cursel);
	_control->OnCarConnect(_carpeer);
}
void CMainDlg::OnCarLeave(WORD, WORD, HWND)
{
	
	_control->OnCarLeave();
	_carpeer = -1;
}


void CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND)
{
	
	EndDialog(wID);
 
}
void CMainDlg::OnSigin(bool bRet)
{
	PostMessage(WM_NOTIFYSIGIN, (WPARAM)bRet);
}

void CMainDlg::OnRobot(std::unique_ptr<UserCamera>& node)
{
	CListViewCtrl box;
	box.Attach(GetDlgItem(IDC_CARLIST));
	std::string info = node->name;
	switch (node->state)
	{
	case UserState::Busy:
		info += " (busy)";
		break;
	case UserState::Offline:
		info += " (offline)";
		break;
	case UserState::Online:
		info += " (online)";
		break;
	}

	int32_t index = box.AddItem(0, 0, info.c_str());
	//box.SetItemText(index, 1, robot.type() == EgoType::Car ? "Car" : "Cave");
	auto  users = std::make_unique<UserCamera>();
	users->uid = node->uid;
	users->name = info;
	users->type = static_cast<EgoType>(node->type);
	users->state = static_cast<UserState>(node->state);
	// 	for (int32_t i = 0; i < robot.info().size(); i++)
	// 	{
	// 		auto& node = robot.info(i);
	// 		CameraInfo camera;
	// 		camera.index = static_cast<RenderPosition>(node.pos());
	// 		camera.label = node.label();
	// 		camera.solution = static_cast<DisplayResolution>(node.solution());
	// 		users->info.push_back(camera);
	// 	}
	box.SetItemData(index, users->uid);
	_userInfo.push_back(std::move(users));
}
void CMainDlg::OnNotifyDel(int32_t peer)
{
	CListViewCtrl box;
	box.Attach(GetDlgItem(IDC_CARLIST));
	int32_t Count = box.GetItemCount();
	for (int32_t i = 0; i < Count; i++)
	{
		auto id = box.GetItemData(i);
		if (id == peer)
		{
			box.DeleteItem(i);
			break;
		}
	}
	for (auto it = _userInfo.begin(); it != _userInfo.end(); ++it)
	{
		if ((*it)->uid == peer)
		{
			_userInfo.erase(it);
			break;
		}
	}
}
void CMainDlg::OnNotifyPing(int32_t value)
{

}
void CMainDlg::OnNotifyRadar(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5)
{

}
void CMainDlg::OnNotifyKickOff()
{

}
