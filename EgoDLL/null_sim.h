#pragma once
#include <stdint.h>
#include "../common/notifier.h"
class CNullSim 
{
public:
	void Start() {}
	void Stop() {}
	void OnPeerMessage(int16_t cmd, int16_t length, const void* data) {}
};

