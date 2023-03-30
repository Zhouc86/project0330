#include <stdint.h>
#include <random>
#include "protocol.pb.h"
#include "../common/comm.h"
#include "./include/EgoInterface.h"
#include "EgoClient.h"
#include "radar_sim.h"
CRadarSim::CRadarSim(CEgoClient* radar) :_radar(radar)
{

}
void CRadarSim::Start()
{

}
void CRadarSim::Stop()
{

}
void CRadarSim::OnPeerMessage(int16_t cmd, int16_t length, const void* data)
{
	RemoNet::CCRadarMessage Req;
	Req.ParseFromArray(data, length);
	
 
	Req.ParseFromArray(data, length);
	_radar->OnRadarData(Req.radar0(), Req.radar1(), Req.radar2(), Req.radar3(), Req.radar4(), Req.radar5());
}