#pragma once
#include "../common/comm.h"
#include "../common/notifier.h"
#include "../common/sensor_socket.h"
#include <thread>
#include <mutex>
class CEgoWindow;
class CMessageQueue;
enum  Sensor_Collection :int32_t
{
	Sensor_None = 0x0,
	Sensor_Task = 0x1,
	Sensor_Left = 0x2,
	Sensor_Right = 0x4,
	Sensor_All = 0x7
};
enum  OpSide
{
	Empty,
	Arm,
	Bucket
};
class CControlSensor//:public ICanNotify
{
public:
	CControlSensor(CMessageQueue * window);
	void Start();
	void Stop();
	void Notify(int8_t* buffer, int32_t size);
	void SetSensorSocket(SensorSocket<CControlSensor>* can);
	ControlStatus CheckStatus();
	void OnBootstrapd(bool ret);
	void OnStartupd(bool ret);
private:
	bool OnSignal(int8_t* data);
	bool OnLeftJoyStick(int8_t* data);
	bool OnRightJoyStick(int8_t* data);

private:
	 
	CEgoWindow* _window;
	int32_t _steer;
	int32_t _arm;
	int32_t _bucket;
	int32_t _brake;
	int32_t _throttle;
	Sensor_Collection _collection;
	volatile bool _bootstrap = false;
	volatile bool _bootstrapd = false;
	volatile bool _oping = false;
	volatile bool _startup = false;
	volatile bool _startupd = false;
	volatile bool _emergenyd = false; //¼±Í£

	volatile bool _resume = false;
	volatile bool _resumed = false;
	RemoNet::Gears _gear =RemoNet::Gears::N;
	//OpSide _side;
	bool    _front_on;
	bool _back_on;
	bool _wiper_on;
	//bool _reseted = true;
	bool _emergency = false;
	SensorSocket<CControlSensor>* _socket;

};

 
