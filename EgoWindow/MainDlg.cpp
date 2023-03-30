// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <memory>
#include <mutex> 
#include "api.h"
#include "Protocol.pb.h"
#include "../common/peer_connection.h"
#include "VideoRenderer.h"
#include "WindowThread.h"
#include "../common/sensor_socket.h"
#include "ControlSensor.h"
#include "MainDlg.h"
#include <fstream>
#include <json/json.h>
#include "../common/iobuffer.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(HWND, LPARAM)
{
	// center the dialog on the screen
	CenterWindow();
	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
	Json::Value root;
	Json::Reader jsonReader;
	std::ifstream ifile("Config.json");
	std::string ip;
	int32_t can_port = 0, host_port = 0;
	std::string can_ip;
	 
 
	if (jsonReader.parse(ifile, root))
	{
		_name = root["name"].asString();
		_account = root["account"].asString();
		_password = root["password"].asString();
		ip = root["ip"].asString();
		can_port = root["can_port"].asInt();
		host_port = root["host_port"].asInt();
		can_ip = root["can_ip"].asString();
		 
	}
	
	_client = std::make_unique<SocketClient>(this);
	_client->Start(ip.c_str());

	for (int i = 0; i < RenderPosition::ALL; i++)
	{
		 
		auto p = std::make_unique<CThreadWindow>((RenderPosition)i, _client.get());
		p->Start();
		_WindowArray.push_back(std::move(p));

	}
	_control = std::make_unique <SensorSocket<CControlSensor>>(_WindowArray[ChannelType::CHANNEL_CAR].get(), can_ip, can_port, host_port);
	_control->Start();
	//_can = std::make_unique<SocketCan>(_WindowArray[ChannelType::CHANNEL_CAR].get());
	//_can->Start(can_ip, can_port, host_port);

// 	for (int i = 0; i < RadarPosition::RADAR_ALL; i++)
// 	{
// 		auto p = std::make_unique<CRadarWindow>((RadarPosition)i);
// 		p->Start();
// 		_RadarArray.push_back(std::move(p));
// 	}

//	_monitor = std::make_unique<MonitorThread>(this);
//	_monitor->Start(monitor_ip.c_str(), mon_remote, mon_local);

	_carpeer = -1;
	SetTimer(IDC_STATETIMER, 300); //Radar Bmp refresh
	return TRUE;
}

void CMainDlg::OnDestroy(void)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
}
void CMainDlg::OnFront(WORD, WORD, HWND)
{
	RemoNet::CCSwitch Req;
	Req.set_front(true);
	CIOBuffer Buffer;
	MessageHead Head;
	Head.Command = RemoNet::CC_Switch;
	Head.Length = Req.ByteSize();
	Head.Serialize(Buffer.Buffer);
	auto ptr = Buffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	Buffer.Length = Head.Length + MessageHead::Size();
	_WindowArray[ChannelType::CHANNEL_CAR]->SendData(&Buffer);
}
void CMainDlg::OnBack(WORD, WORD, HWND)
{
	RemoNet::CCSwitch Req;
	Req.set_front(false);
	CIOBuffer Buffer;
	MessageHead Head;
	Head.Command = RemoNet::CC_Switch;
	Head.Length = Req.ByteSize();
	Head.Serialize(Buffer.Buffer);
	auto ptr = Buffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	Buffer.Length = Head.Length + MessageHead::Size();
	_WindowArray[ChannelType::CHANNEL_CAR]->SendData(&Buffer);
}
void CMainDlg::OnSend(WORD, WORD, HWND)
{
	char text[1024];
	GetDlgItemText(IDC_EDIT, text, sizeof(text));
	RemoNet::TestTextReq Req;
	Req.set_text(text);
	CIOBuffer Buffer;
	MessageHead Head;
	Head.Command = RemoNet::CC_Text;
	Head.Length = Req.ByteSize();
	Head.Serialize(Buffer.Buffer);
	auto ptr = Buffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	Buffer.Length = Head.Length + MessageHead::Size();
	_WindowArray[ChannelType::CHANNEL_CAR]->SendData(&Buffer);
}

void CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	_client->Stop();
	//_can->Stop();
	//_monitor->Stop();
	for (int i = 0; i < RenderPosition::ALL; i++)
	{
	 
			_WindowArray[i]->Stop();
			_WindowArray[i].reset();
		 
	}
	 

	CloseDialog(wID);
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::OnMessageFrameNotify(ChannelType type, int16_t cmd, int16_t length, const void* data)
{
	 
	 
	if (cmd == RemoNet::CC_Text)
	{
		RemoNet::TestTextReq Req;
	 
		Req.ParseFromArray(data,length);

		std::cout << Req.text() << std::endl;
	}
	if (cmd == RemoNet::CC_StopACK)
	{
		PostMessage(WM_STOP_ACK);
	}
	if (cmd == RemoNet::CC_BootRep)
	{
		RemoNet::CCBooStrapRep Rep;
		Rep.ParseFromArray(data, length);
		_control->Get()->OnBootstrapd(Rep.ret());
	}
	if (cmd == RemoNet::CC_StartupRep)
	{
		RemoNet::CCStartupRep Rep;
		Rep.ParseFromArray(data, length);
		_control->Get()->OnStartupd(Rep.ret());
	}
 
}

void CMainDlg::OnVideoOffer(int32_t index, const char* type, const char* sdp)
{
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	OfferDesc* desc = (OfferDesc*)pBuffer->Buffer;
	strcpy_s(desc->type, type);
	strcpy_s(desc->sdp, sdp);
	_WindowArray[index]->PostMessage(WM_NOTIFY_OFFER, (WPARAM)pBuffer);
}

void CMainDlg::OnVideoAnswer(int32_t index, const char* type, const char* sdp)
{
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	AnswerDesc* p = (AnswerDesc*)(pBuffer->Buffer);
	strcpy_s(p->type, type);
	strcpy_s(p->sdp, sdp);
	_WindowArray[index]->PostMessage(WM_NOTIFY_ANSWER, (WPARAM)pBuffer);
	_WindowArray[index]->DelayDataChannel();
	index++;
 	if (index < RenderPosition::ALL)
 	{
 		
 		_WindowArray[index]->DelayNextVideoReq();
 	}
	else
	{
		_WindowArray[ChannelType::CHANNEL_CAR]->DelayControlStart();
	}
 
}

void CMainDlg::OnVideoCandidate(int32_t index, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid)
{
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	CandidateDesc* desc = (CandidateDesc*)(pBuffer->Buffer);
	strcpy_s(desc->candidate, candidate);
	strcpy_s(desc->sdp_mid, sdp_mid);
	desc->sdp_mline_index = sdp_mline_index;
	_WindowArray[index]->PostMessage(WM_NOTIFY_CANDIDATE, (WPARAM)pBuffer);
}

void CMainDlg::OnSigin(bool bRet)
{
	PostMessage(WM_NOTIFYSIGIN, (WPARAM)bRet);
}

void CMainDlg::OnNotifySigin(bool bRet)
{
	if (bRet)
	{
		_client->WriteRobotReq();
	}
}

void CMainDlg::OnNotifyDel(int32_t peer, EgoType type)
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

void CMainDlg::OnRobot(const RemoNet::Robot& robot)
{
	CListViewCtrl box;
	box.Attach(GetDlgItem(IDC_CARLIST));
	std::string info = robot.name();
	switch (robot.state())
	{
	case RemoNet::Robot_RobotState::Robot_RobotState_Busy:
		info += " (busy)";
		break;
	case RemoNet::Robot_RobotState::Robot_RobotState_Offline:
		info += " (offline)";
		break;
	case RemoNet::Robot_RobotState::Robot_RobotState_Online:
		info += " (online)";
		break;
	}

	int32_t index = box.AddItem(0, 0, info.c_str());
	//box.SetItemText(index, 1, robot.type() == EgoType::Car ? "Car" : "Cave");
	auto  users = std::make_unique<UserCamera>();
	users->uid = robot.rid();
	users->name = info;
	users->type = static_cast<EgoType>(robot.type());
	users->state = static_cast<UserState>(robot.state());
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
/*
void CMainDlg::OnNotifyConnect(int32_t uid)
{
	for (auto& node : _userInfo)
	{
		if (node->uid == uid)
		{
 			if (node->type == EgoType::Car)
			{
				if (_carpeer != -1) return;
				mrsWebrtcCreateFactory(false);
				_carpeer = uid;
				for (int i = 0; i < RenderPosition::ALL; i++)
				{
					_WindowArray[i]->SetPeer(_carpeer);
				}
				for (auto& node : _userInfo)
				{
					if (node->uid == _carpeer)
					{
						_WindowArray[RenderPosition::FRONT_BACK]->CreateVideoReq();

						break;;
					}
				}
			}
			
			break;
		}
	}
}
*/
void CMainDlg::OnCarConnect(WORD, WORD, HWND)
{
	if (_carpeer != -1) return;
	CListViewCtrl box;
	box.Attach(GetDlgItem(IDC_CARLIST));

	int32_t cursel = box.GetSelectedIndex();
	if (cursel == -1) return;
	mrsWebrtcCreateFactory(false);
	_carpeer = box.GetItemData(cursel);
	for (int i = 0; i < RenderPosition::ALL; i++)
	{
		_WindowArray[i]->SetPeer(_carpeer);
	}
	for (auto& node : _userInfo)
	{
		if (node->uid == _carpeer)
		{
			_WindowArray[RenderPosition::FRONT_BACK]->CreateVideoReq();

			break;;
		}
	}
}
/*
void CMainDlg::OnNotifyDisconnect(int32_t uid)
{
	for (auto& node : _userInfo)
	{
		if (node->uid == uid)
		{
			if (node->type == EgoType::Car)
			{
				if (_carpeer == -1) return;
				_client->WriteVideoLeave(EgoType::Car, _carpeer);

				for (int32_t i = 0; i < RenderPosition::ALL; i++)
				{
					_WindowArray[i]->PostMessage(WM_NOTIFY_LEAVE);
				}
				_carpeer = -1;
			}
			 
			break;
		}
	}
}
*/
void CMainDlg::OnCarLeave(WORD, WORD, HWND)
{
	RemoNet::SensorStop Req;
	CIOBuffer Buffer;
	MessageHead Head;
	Head.Command = RemoNet::CC_SensorStop;
	Head.Length = Req.ByteSize();
	Head.Serialize(Buffer.Buffer);
	auto ptr = Buffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	Buffer.Length = Head.Length + MessageHead::Size();
	_WindowArray[ChannelType::CHANNEL_CAR]->SendData(&Buffer);

	
}
void CMainDlg::OnStopAck()
{
	_client->WriteVideoLeave(EgoType::Car, _carpeer);

	for (int32_t i = 0; i < RenderPosition::ALL; i++)
	{
		_WindowArray[i]->PostMessage(WM_NOTIFY_LEAVE);
	}
	_carpeer = -1;
}
 
void CMainDlg::OnConnected(bool bRet)
{
	PostMessage(WM_NOTIFYCONNECTED, (WPARAM)bRet);

}
void CMainDlg::OnNotifyConnected(bool bRet)
{
	if (bRet)
	{
		if (!_account.empty())
			_client->WriteSign(_account.c_str(), _password.c_str());
		_updateThread.start(_client.get());
	}
	else
	{
		if (_carpeer != -1)
		{
			for (int32_t i = RenderPosition::FRONT_BACK; i < RenderPosition::ALL; i++)
			{
				_WindowArray[i]->PostMessage(WM_NOTIFY_LEAVE);
			}
			_carpeer = -1;
		}
		 
		_updateThread.stop();
		 
		CListViewCtrl car;
		car.Attach(GetDlgItem(IDC_CARLIST));
		car.DeleteAllItems();
		_userInfo.clear();

	}
}
 

void CMainDlg::OnTimer(UINT id)
{
	// FOR TEST ONLY
	//float x = rand() % 10;
	//float y = rand() % 10;
	//Coord p;
	//p.x = x;
	//p.y = y;
	//_ptArray[0].push_back(p);
	//_RadarArray[0]->SetRadarPoint(_ptArray[0]);
	//_ptArray[0].clear();
	//_RadarArray[0]->Invalidate();
	// FOR TEST ONLY

	if (!bUpdated) return;
	 
	bUpdated = false;
	/*
	int32_t temp = feedback.feedBackData.feedback.water_temp_alarm;

	SetDlgItemText(IDC_WATER_TEMP_ALARM, warnMsg(temp).c_str());

	temp = feedback.feedBackData.feedback.oil_pressure_low_alarm;
	SetDlgItemText(IDC_OIL_PRESSURE_LOW_ALARM, warnMsg(temp).c_str());

	temp = feedback.feedBackData.feedback.air_filter_alarm;
	SetDlgItemText(IDC_AIR_FILTER_ALARM, warnMsg(temp).c_str());

	temp = feedback.feedBackData.feedback.left_oil_filter_alarm;
	SetDlgItemText(IDC_LEFT_OIL_FILTER_ALARM, warnMsg(temp).c_str());

	temp = feedback.feedBackData.feedback.right_oil_filter_alarm;
	SetDlgItemText(IDC_RIGHT_OIL_FILTER_ALARM, warnMsg(temp).c_str());

	temp = feedback.feedBackData.feedback.oil_loop_filter_alarm;
	SetDlgItemText(IDC_OIL_LOOP_FILTER_ALARM, warnMsg(temp).c_str());

	temp = feedback.feedBackData.feedback.hydr_pressure_low_alarm;
	SetDlgItemText(IDC_HYDR_PRESSURE_LOW_ALARM, warnMsg(temp).c_str());

	temp = feedback.feedBackData.feedback.engine_rpm;
	SetDlgItemInt(IDC_ENGINE_RPM, temp);

	temp = feedback.feedBackData.feedback.oil_pressure;
	SetDlgItemInt(IDC_OIL_PRESSURE, temp);

	temp = feedback.feedBackData.feedback.water_temperature;
	SetDlgItemInt(IDC_WATER_TEMPERATURE, temp);

	temp = feedback.feedBackData.feedback.left_MA_pressure;
	SetDlgItemInt(IDC_LEFT_MA_PRESSURE, temp);

	temp = feedback.feedBackData.feedback.left_MB_pressure;
	SetDlgItemInt(IDC_LEFT_MB_PRESSURE, temp);

	temp = feedback.feedBackData.feedback.right_MA_pressure;
	SetDlgItemInt(IDC_RIGHT_MA_PRESSURE, temp);

	temp = feedback.feedBackData.feedback.right_MB_pressure;
	SetDlgItemInt(IDC_RIGHT_MB_PRESSURE, temp);

	temp = feedback.feedBackData.feedback.pump_pressure;
	SetDlgItemInt(IDC_PUMP_PRESSURE, temp);

	temp = feedback.feedBackData.feedback.left_motor_rpm;
	SetDlgItemInt(IDC_LEFT_MOTOR_RPM, temp);

	temp = feedback.feedBackData.feedback.right_motor_rpm;
	SetDlgItemInt(IDC_RIGHT_MOTOR_RPM, temp);

	temp = feedback.feedBackData.feedback.hydr_oil_height;
	SetDlgItemInt(IDC_HYDR_OIL_HEIGHT, temp);

	temp = feedback.feedBackData.feedback.disel_oil_height;
	SetDlgItemInt(IDC_DISEL_OIL_HEIGHT, temp);

	temp = feedback.feedBackData.feedback.up_time;
	SetDlgItemInt(IDC_UP_TIME, temp);

	temp = feedback.feedBackData.feedback.votage;
	SetDlgItemInt(IDC_VOTAGE, temp);
	*/
}

void CMainDlg::OnVideoRep(int32_t index, int32_t peer)
{
	if ( peer != _carpeer) return;

	for (auto& node : _userInfo)
	{
		if (node->uid == peer)
		{
		 
			_WindowArray[index]->PostMessage(WM_NOTIFY_REP, (WPARAM)peer, (LPARAM)index);
			return;
			 
		}
	}
}
void CMainDlg::OnNotifyKick()
{

}
void CMainDlg::OnVideoLeave(int32_t peer, EgoType type)
{
	if (type == EgoType::Car)
	{

		for (int32_t i = RenderPosition::FRONT_BACK; i <  RenderPosition::ALL; i++)
		{
			_WindowArray[i]->PostMessage(WM_NOTIFY_LEAVE);
		}
		_carpeer = -1;
	}
	 
}
 
/*
void CMainDlg::OnNotifyReady()
{
	if (bNotifyReady) return;
	
	CIOBuffer Buffer;
	HM_CarList* l = reinterpret_cast<HM_CarList*>(Buffer.Buffer);
	///	HM_CarList * list+-
	l->Head.Command = HM_CARLIST;
	l->count = _userInfo.size();
	Buffer.Length = offsetof(HM_CarList, carArray);
	int32_t index = 0;
	for (auto& node : _userInfo)
	{
		strcpy_s(l->carArray[index].name, node->name.c_str());
		l->carArray[index].state = node->state;
		l->carArray[index].type = node->type;
		l->carArray[index].uid = node->uid;
		index++;
		Buffer.Length += sizeof(CarState);
	}
	l->Head.Length = Buffer.Length;
	//_monitor->Write(&Buffer);
	bNotifyReady = true;
}
void CMainDlg::OnNotifyStop()
{

}
*/