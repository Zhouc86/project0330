#include <random>
#include "protocol.pb.h"
#include "EgoController.h"
#include "include/EgoInterface.h"
#include "api.h"
#include "../common/peer_connection.h"

#include "control_sensor.h"
#include "EgoWindow.h"
#include "car_sim.h"
#include "out_sim.h"
#ifdef LIDAR_SENSOR
#include "lidar_sim.h"
#endif
#include "radar_sim.h"
#include "EgoClient.h"
CEgoController::CEgoController(IEgoNotify* noitfy)
{
	_client = std::make_unique<CEgoClient>(noitfy);
}
void CEgoController::OnCarConnect(int32_t peer)
{
	_client->OnCarConnect(peer);
}
void CEgoController::OnCarLeave()
{
	_client->OnCarLeave();
}
void CEgoController::Login(std::string account, std::string pass)
{
	_client->Login(account, pass);
}
void CEgoController::Start(std::array<IRender*, RenderPosition::ALL>& ar)
{
	_client->Start(ar);
}
/*
void CEgoController::ReqCarList()
{
	_client->ReqCarList();
}*/
ControlStatus CEgoController::CheckStatus()
{
	return _client->CheckStatus();
}