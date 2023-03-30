#include <stdint.h>
#include <random>
#include "../common/comm.h"
#include "Protocol.pb.h"
#include "car_sim.h"
#include "EgoClient.h"

CCarSim::CCarSim(CEgoClient* car) :_car(car)
{

}
void CCarSim::Start()
{

}
void CCarSim::Stop()
{
	
}
void CCarSim::OnPeerMessage(int16_t cmd, int16_t length, const void* data)
{
	  switch (cmd)
	  {
	  case RemoNet::CC_Ping:
	  {
		  RemoNet::CCPing Req;
		  Req.ParseFromArray(data, length);
		  int64_t tick = GetTickCount64();
		  int32_t diff = tick - Req.tick();
		  _car->OnPingValue(diff);
	  }
	  break;
	  case RemoNet::CC_StopACK:
	  {
		  _car->OnStopAck();
	  }
	  break;
	  case RemoNet::CC_BootRep:
	  {
		  RemoNet::CCBooStrapRep rep;
		  rep.ParseFromArray(data, length);
		  _car->OnBootstrapd(rep.ret());
	  }
	  break;
	  case RemoNet::CC_StartupRep:
	  {
		  RemoNet::CCStartupRep rep;
		  rep.ParseFromArray(data, length);
		  _car->OnStartupd(rep.ret());

	  }
	  break;
	  }
}