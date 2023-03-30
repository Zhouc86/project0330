
#include <json/json.h>
#include <fstream>
#include <random>
#include <time.h>
#include "EgoClient.h"
#include "api.h"
 
#include "include/EgoInterface.h"
#include "EgoWindow.h"
#include "control_sensor.h"
#include "../common/iobuffer.h"
#include "../common/sensor_socket.h"
#include "../common/peer_connection.h"
#include "car_sim.h"
#include "null_sim.h"
#include "out_sim.h"
#include "radar_sim.h"
#ifdef LIDAR_SENSOR
#include "lidar_sim.h"
#endif
CEgoClient::CEgoClient(IEgoNotify* n) :_notify(n),_u(100, 500)
{

}
void CEgoClient::Start(std::array<IRender*, RenderPosition::ALL>& ar)
{
	_carpeer = -1;
	_e.seed(time(NULL));
	Json::Value root;
	Json::Reader jsonReader;
	std::ifstream ifile("Config.json");
	std::string ip;
	int32_t can_port = 0, host_port = 0;
	std::string can_ip;
	std::string motion_ip;
	int32_t motion_port;
 
	 
	std::string host;
	if (jsonReader.parse(ifile, root))
	{
		ip = root["server"].asString();
		can_port = root["can_port"].asInt();
		host_port = root["host_port"].asInt();
		can_ip = root["can_ip"].asString();
		host = root["host_in"].asString();
		motion_ip = root["motion_ip"].asString();
		motion_port = root["motion_port"].asInt();

	}

	_client = std::make_unique<SocketClient>(this);
	_client->Start(ip.c_str());
	for (int i = 0; i < RenderPosition::ALL; i++)
	{
		auto p = std::make_unique<CEgoWindow>(ar[i], (RenderPosition)i, _client.get());
		p->Start();
		_WindowArray.push_back(std::move(p));
	}
	
	//CManipulationSensor* sensor = new CManipulationSensor(_WindowArray[RenderPosition::FRONT_BACK].get());
	//_control = std::make_unique<SensorSocket<CControlSensor>>(_WindowArray[RenderPosition::FRONT_BACK].get(), can_ip, can_port, host_port);
	_control = std::make_unique<SensorSocket<CControlSensor>>(_WindowArray[RenderPosition::FRONT].get(), can_ip, can_port, host_port);
	_control->Start(host.c_str());
	_carsim = std::make_unique<CCarSim>(this);
	_carsim->Start();
	_outsim = std::make_unique<COutSim>(motion_ip.c_str(),motion_port);
	_outsim->Start();
	_radarsim = std::make_unique<CRadarSim>(this);
	_radarsim->Start();
	OnRadarData(0, 0, 0, 0, 0, 0);

#ifdef LIDAR_SENSOR
	_lidarsim = std::make_unique<CLidarSim>(this);
	_lidarsim->Start();
#endif
	
	 
}
void CEgoClient::Login(std::string account, std::string pass)
{
	_account = account;
	_pass = pass;
	if(_connected)
		_client->WriteSign(_account.c_str(), _pass.c_str());

}
void CEgoClient::OnRobot(const RemoNet::Robot& robot)
{
	auto  users = std::make_unique<UserCamera>();
	users->uid = robot.rid();
	users->name = robot.name();
	users->type = static_cast<EgoType>(robot.type());
	users->state = static_cast<UserState>(robot.state());
	 
	_notify->OnRobot(users);
	_userInfo.push_back(std::move(users));

}
void CEgoClient::OnConnected(bool bRet)
{
	if (bRet)
	{
		_connected = true;
		if (!_account.empty())
			_client->WriteSign(_account.c_str(),_pass.c_str());
		_updateThread.start(_client.get());
	}
	else
	{
		_connected = false;
		if (_carpeer != -1)
		{
			for (int32_t i = RenderPosition::FRONT; i < RenderPosition::ALL; i++)
			{
				_WindowArray[i]->PostMessage(WM_NOTIFY_LEAVE);
			}
			_carpeer = -1;
		}
		_updateThread.stop();
		_userInfo.clear();
	}
}
void CEgoClient::OnVideoRep(int32_t index, int32_t peer)
{
	if (peer != _carpeer) return;
	for (auto& node : _userInfo)
	{
		if (node->uid == peer)
		{
			 
			_WindowArray[index]->PostMessage(WM_NOTIFY_REP, (int64_t)peer);
			 
			 
		}
	}
}
void CEgoClient::OnVideoOffer(int32_t index, const char* type, const char* sdp)
{
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	OfferDesc* desc = (OfferDesc*)pBuffer->Buffer;
	strcpy_s(desc->type, type);
	strcpy_s(desc->sdp, sdp);
	_WindowArray[index]->PostMessage(WM_NOTIFY_OFFER, (int64_t)pBuffer);
}
void CEgoClient::OnVideoAnswer(int32_t index, const char* type, const char* sdp)
{
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	AnswerDesc* p = (AnswerDesc*)(pBuffer->Buffer);
	strcpy_s(p->type, type);
	strcpy_s(p->sdp, sdp);
	_WindowArray[index]->PostMessage(WM_NOTIFY_ANSWER, (int64_t)pBuffer);
	_WindowArray[index]->DelayDataChannel();
	index++;
	if (index < RenderPosition::ALL)
	{

		_WindowArray[index]->DelayNextVideoReq();
	}
	else
	{
		_WindowArray[ChannelType::CHANNEL_CAR]->DelayStartPing();
	}
	
}
void CEgoClient::OnVideoCandidate(int32_t index, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid)
{
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	CandidateDesc* desc = (CandidateDesc*)(pBuffer->Buffer);
	strcpy_s(desc->candidate, candidate);
	strcpy_s(desc->sdp_mid, sdp_mid);
	desc->sdp_mline_index = sdp_mline_index;
	_WindowArray[index]->PostMessage(WM_NOTIFY_CANDIDATE, (int64_t)pBuffer);
}
void CEgoClient::OnVideoLeave(int32_t peer, EgoType type)
{
	if (type == EgoType::Car)
	{
		_WindowArray[ChannelType::CHANNEL_CAR]->SetControlState(ControlState::Check);
		for (int32_t i = RenderPosition::FRONT; i < RenderPosition::ALL; i++)
		{
			_WindowArray[i]->PostMessage(WM_NOTIFY_LEAVE);
		}
		_carpeer = -1;
	}
}
void CEgoClient::OnNotifyKick()
{
	_notify->OnNotifyKickOff();
}
void CEgoClient::OnMessageFrameNotify(ChannelType type, int16_t cmd, int16_t length, const void* data)
{
	 
	switch (type)
	{
	case ChannelType::CHANNEL_CAR:
	{
		_carsim->OnPeerMessage(cmd, length, data);

	}
	break;
	case ChannelType::CHANNEL_IMU:
		_outsim->OnPeerMessage(cmd, length, data);
		break;
	case ChannelType::CHANNEL_RADAR:
		_radarsim->OnPeerMessage(cmd, length, data);
		break;
#ifdef LIDAR_SENSOR
	case ChannelType::CHANNEL_LEFT_LIDAR:
	case ChannelType::CHANNEL_RIGHT_LIDAR:
		_lidarsim->OnPeerMessage(cmd, length, data);
		break;
#endif

	}
	/*
	if (type == ChannelType)
	{
		RemoNet::TestTextReq Req;
		 
		Req.ParseFromArray(data, length);

		std::cout << Req.text() << std::endl;
	}
	*/
}
 
void CEgoClient::OnSigin(bool bRet)
{
	_notify->OnSigin(bRet);
	if (bRet)
	{
		_client->WriteRobotReq();
	}
}
void CEgoClient::OnNotifyDel(int32_t peer, EgoType type)
{
	for (auto it = _userInfo.begin(); it != _userInfo.end(); ++it)
	{
		if ((*it)->uid == peer)
		{
			_userInfo.erase(it);
			break;
		}
	}
	_notify->OnNotifyDel(peer);
}
void CEgoClient::OnCarConnect(int32_t peer)
{
	mrsWebrtcCreateFactory(false);
	for (int i = 0; i < RenderPosition::ALL; i++)
	{
		_WindowArray[i]->SetPeer(peer);
	}
	for (auto& node : _userInfo)
	{
		if (node->uid == peer)
		{
			_WindowArray[RenderPosition::FRONT]->CreateVideoReq();
			_carpeer = peer;
			break;
		}
	}
}
void CEgoClient::OnCarLeave()
{
	/*RemoNet::SensorStop Req;
	CIOBuffer Buffer;
	MessageHead Head;
	Head.Command = RemoNet::CC_SensorStop;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(Buffer.Buffer);
	auto ptr = Buffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	Buffer.Length = Head.Length + MessageHead::Size();
	*/
	_WindowArray[ChannelType::CHANNEL_CAR]->SetControlState(ControlState::Check);
	_WindowArray[ChannelType::CHANNEL_CAR]->StopPing();
	_client->WriteVideoLeave(EgoType::Car, _carpeer);

	for (int32_t i = 0; i < RenderPosition::ALL; i++)
	{
		_WindowArray[i]->PostMessage(WM_NOTIFY_LEAVE);
	}
	_carpeer = -1;
	//_WindowArray[ChannelType::CHANNEL_CAR]->SendData(&Buffer);
}
void CEgoClient::OnStopAck()
{

}
void CEgoClient::OnBootstrapd(bool ret)
{
	_control->Get()->OnBootstrapd(ret);
}
void CEgoClient::OnStartupd(bool ret)
{
	_control->Get()->OnStartupd(ret);
}
void CEgoClient::OnRadarData(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5)
{
	_notify->OnNotifyRadar(r0, r1, r2, r3, r4, r5);
}
void CEgoClient::OnImuData(int32_t x, int32_t y)
{
	_notify->OnNotifyImu(x, y);
}

void CEgoClient::ReqCarList()
{
	_client->WriteRobotReq();
}
 
void CEgoClient::OnPingValue(int32_t value)
{
	_notify->OnNotifyPing(value);
	//OnRadarData(_u(_e), _u(_e), _u(_e), _u(_e), _u(_e), _u(_e)); 
}

#ifdef LIDAR_SENSOR
void CEgoClient::OnLidarData(bool isLeft, bool isDense, int32_t seq, PointXYZI* data, int32_t length)
{
	_notify->OnLidarData(isLeft, isDense, seq, data, length);
}
#endif
ControlStatus CEgoClient::CheckStatus()
{
	return _control->Get()->CheckStatus();
}