#pragma once
#include "../common/notifier.h"
#ifdef LIDAR_SENSOR

class CEgoClient;
class CLidarSim
{
public:
	CLidarSim(CEgoClient* car);
	void Start();
	void Stop();
	void OnPeerMessage(int16_t cmd, int16_t length, const void* data);
private:
	CEgoClient* _car;

};
#endif

