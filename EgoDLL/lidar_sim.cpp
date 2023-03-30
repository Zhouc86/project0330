
#ifdef LIDAR_SENSOR
#include <stdint.h>
#include "../common/comm.h"
#include "Protocol.pb.h"
#include "lidar_sim.h"
#include "EgoClient.h"
#include "./include/EgoInterface.h"

CLidarSim::CLidarSim(CEgoClient* car) :_car(car)
{

}
void CLidarSim::Start()
{

}
void CLidarSim::Stop()
{

}

void CLidarSim::OnPeerMessage(int16_t cmd, int16_t length, const void* data)
{
	RemoNet::LidarPoint Req;
	Req.ParseFromArray(data, length);
	PointXYZI* Items = (PointXYZI*)alloca(sizeof(PointXYZI) * Req.data_size());
	PointXYZI* ptr = Items;
	for (int32_t i = 0; i < Req.data_size(); i+=4)
	{
		ptr->x = Req.data(i);
		ptr->y = Req.data(i + 1);
		ptr->z = Req.data(i + 2);
		ptr->intensity = Req.data(i + 3);
		++ptr;
	}
	_car->OnLidarData(Req.is_left(), Req.is_dense(), Req.seq(), Items, Req.data_size());
    
}
#endif