#pragma once
#include "../common/notifier.h"
class CEgoClient;
class CRadarSim 
{
public:
	CRadarSim(CEgoClient* radar);
	void Start();
	void Stop();
	void OnPeerMessage(int16_t cmd, int16_t length, const void* data);
private:
	CEgoClient* _radar;
};

