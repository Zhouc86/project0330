#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "./include/EgoInterface.h"
#include "EgoWindow.h"
#include "protocol.pb.h"
#include "control_sensor.h"
#include "../common/iobuffer.h"
#include "protocol.pb.h"
CControlSensor::CControlSensor(CMessageQueue* q) 
{
	_window = static_cast<CEgoWindow*>(q);
	assert(_window != nullptr);
	_collection = Sensor_Collection::Sensor_None;
	_brake = _bucket = _throttle = _steer = 0;
	_arm = 0;
	_bucket = 0;
	_emergency= _back_on = _front_on = _wiper_on = false;
}
void CControlSensor::Notify(int8_t* buffer, int32_t size)
{
	int32_t receivedCnt = size / CAN_MSG_LEN;
	cannet_frame* p = (cannet_frame*)buffer;
	// 	char str[64];
	// 	ZeroMemory(str, sizeof(str));
	// 	std::string ret;
	for (int32_t i = 0; i < receivedCnt; i++)
	{
		int32_t id = ntohl(p->canid) & 0x0FFF;
		 
		switch (id)
		{
		case 0x181:
		{
			OnSignal(p->data);
			//if (b == true)
			_collection = (Sensor_Collection)(_collection | Sensor_Collection::Sensor_Task);
		}
		break;
		case 0x633:
		{
			bool b = OnLeftJoyStick(p->data);
			if (b == true)
				_collection = (Sensor_Collection)(_collection | Sensor_Collection::Sensor_Left);
		}
		break;
		case 0x634:
		{
			bool b = OnRightJoyStick(p->data);
			if (b == true)
				_collection = (Sensor_Collection)(_collection | Sensor_Collection::Sensor_Right);
		}
		break;

		}

		//std::string str = std::to_string(id) +"\n";



	}
	if (_window->GetControlState()== ControlState::Process&&_collection == Sensor_Collection::Sensor_All)
	{
		 
		
		RemoNet::CCRobotAnalog _robot;
		_robot.set_brake(_brake);
		_robot.set_arm(_arm);
		_robot.set_bucket(_bucket);
		_robot.set_steer(_steer);
		_robot.set_throttle(_throttle);
		_robot.set_frontlight(_front_on);
		_robot.set_backlight(_back_on);
		_robot.set_wipe(_wiper_on);
		_robot.set_emergency(_emergency);
		_robot.set_gears(_gear);
		_robot.set_resume(_resume);


		MessageHead Head;
		CIOBuffer pBuffer;
		Head.Command = RemoNet::CC_RobotAnalog;
		Head.Length = _robot.ByteSizeLong();
		Head.Serialize(pBuffer.Buffer);
		auto ptr = pBuffer.Buffer + MessageHead::Size();
		_robot.SerializeToArray(ptr, Head.Length);
		pBuffer.Length = MessageHead::Size() + Head.Length;
		_window->SendData(&pBuffer);
		
		_collection = Sensor_Collection::Sensor_None;


	}
}
bool CControlSensor::OnSignal(int8_t* data)
{
	/*std::string str;
	char buffer[128];
		for (int i = 0; i < 8; i++)
		{
			int16_t t = (data[i]) & 0x00FF;
			sprintf_s(buffer, "%2x ", t);
			str += buffer;
		}
		str += "\n";
		OutputDebugStringA(str.c_str());
		*/
	/*
	 ret = (data[0] & 0x01) != 0;
	if (ret == true)
	{
		if (_bootstrap == false)
		{
			if (_window->GetControlState() == ControlState::Process)
			{
			 


				_bootstrap = true;
				RemoNet::CCBootStrapReq Req;
				MessageHead Head;
				CIOBuffer pBuffer;
				Head.Command = RemoNet::CC_BootReq;
				Head.Length = Req.ByteSizeLong();
				Head.Serialize(pBuffer.Buffer);
				auto ptr = pBuffer.Buffer + MessageHead::Size();
				Req.SerializeToArray(ptr, Head.Length);
				pBuffer.Length = MessageHead::Size() + Head.Length;
				_window->SendData(&pBuffer);
			}
		}

	}
	ret = (data[0] & 0x02) != 0;
	if (ret == true)
	{
		if (!_startup && _bootstrapd)
		{
			if (_window->GetControlState() == ControlState::Process)
			{
				_startup = true;
				RemoNet::CCStartupReq Req;
				MessageHead Head;
				CIOBuffer pBuffer;
				Head.Command = RemoNet::CC_StartupReq;
				Head.Length = Req.ByteSizeLong();
				Head.Serialize(pBuffer.Buffer);
				auto ptr = pBuffer.Buffer + MessageHead::Size();
				Req.SerializeToArray(ptr, Head.Length);
				pBuffer.Length = MessageHead::Size() + Head.Length;
				_window->SendData(&pBuffer);
			}
		}
	}
	*/
	/*if (data[0] == 0)
	{
		if (_front_on == true || _back_on == true)
		{
			_reseted = true;
		}

	}
	*/
	bool ret = (data[0] & 0x10) != 0; //前车等
	//if (ret == true)
	{
		_front_on = (data[0] & 0x10) != 0;
		//if (_window->GetControlState() == ControlState::Process)
	/*	{
			if (_reseted == true)
			{
				_front_on = !_front_on;
				_reseted = false;
			}
			 
		}*/
	}
	ret = (data[0] & 0x8) != 0;   //后车灯
	//if (ret == true)
	{
		_back_on = (data[0] & 0x8) != 0;
	/*	if (_window->GetControlState() == ControlState::Process)
		{
			if (_reseted == true)
			{
				_back_on = !_back_on;
				_reseted = false;
			}
			 
		}
		*/
	}
	 
//	if (ret == true)
//	{
		if (_window->GetControlState()==ControlState::Process)
		{
			_wiper_on = (data[1] & 0x1) != 0;
		    /*
			RemoNet::Wiper req;
			req.set_ret(_front_on);
			CIOBuffer pBuffer;
			MessageHead Head;
			Head.Command = RemoNet::CC_Wiper;
			Head.Length = req.ByteSizeLong();
			Head.Serialize(pBuffer.Buffer);
			auto ptr = pBuffer.Buffer + MessageHead::Size();
			req.SerializeToArray(ptr, Head.Length);
			pBuffer.Length = MessageHead::Size() + Head.Length;
			_window->SendData(&pBuffer);
			*/
		}
	//}
	_resume = (data[2]) != 0;
	_emergency = data[3] != 0;
	_throttle = data[4];
	_brake = data[5];
	return true;
	 
}
bool CControlSensor::OnLeftJoyStick(int8_t* data)
{


	int32_t signal = 0;
	if ((data[0] & 0x3) != 0)
	{

		if ((data[0] & 0x03) != 0x01)
		{
			return false;
		}
		signal = 0;

	}
	if ((data[0] & (0x3 << 2)) != 0)
	{
		int32_t value = data[0] >> 2;

		if ((value & 0x03) != 0x01)
		{
			return false;
		}
		signal = -1;

	}
	if ((data[0] & (0x3 << 4)) != 0)
	{
		int32_t value = data[0] >> 4;

		if ((value & 0x03) != 0x01)
		{
			return false;
		}
		signal = 1;
	}
	uint16_t xvalue = ((uint16_t)data[1]) & 0x00FF;
	xvalue <<= 2;
	uint16_t lvalue = ((uint16_t)data[0]) & 0x00FF;
	xvalue |= (lvalue >> 6);

	_steer = xvalue * signal;
	/*
	if (signal == -1)
	{
		_steer = xvalue + 1000;
	}
	else
		_steer = xvalue;
		*/
	
	//OutputDebugString(std::to_string(_steer).c_str());
	/*
	char buffer[64];
	std::string ret;
	for (int i = 0; i < 8; i += 2)
	{
		int16_t value = (data[5] >> i) & 0x3;
		sprintf_s(buffer, "%x ", value);
		ret += buffer;
	}
	*/
	_gear = RemoNet::Gears::N;

	int32_t d = (data[5] >> 4) & 0x3;
	int32_t r = (data[5] >> 6) & 0x3;
	if (d == 0x1)
	{
		_gear = RemoNet::Gears::D;
	}
	else if (r == 0x1)
	{
		_gear = RemoNet::Gears::R;
	}

	// 	for (int i = 0; i < 8; i += 2)
	// 	{
	// 		int16_t value = (data[6] >> i) & 0x3;
	// 		sprintf_s(buffer, "%x ", value);
	// 		ret += buffer;
	// 	}
	// 	for (int i = 5; i < 8; i++)
	// 	{
	// 		int16_t t = (data[i]) & 0x00FF;
	// 		sprintf_s(buffer, "%x ", t);
	// 		ret += buffer;
	// 	}
	 
	return true;

}
bool CControlSensor::OnRightJoyStick(int8_t* data)
{
	/*
	std::string str;
	char buffer[128];
	for (int i = 0; i < 8; i++)
	{
		int16_t t = (data[i]) & 0x00FF;
		sprintf_s(buffer, "%2x ", t);
		str += buffer;
	}
	str += "\n";
	OutputDebugStringA(str.c_str());
	*/
	 /*
	_side = OpSide::Empty;
	if (((data[5] >> 2) & 0x3) == 0x01)
	{
		_side = OpSide::Arm;
	
	}
	else if (((data[5] >> 4) & 0x3) == 0x01)
	{
		
		_side = OpSide::Bucket;
	}
	*/


	int32_t signal = 0;
	/*if ((data[2] & 0x3) != 0)
	{

		if ((data[2] & 0x03) != 0x01)
		{
			return false;
		}
		signal = 0;

	}
	*/
	if ((data[2] & (0x3 << 2)) != 0)
	{
		int32_t value = data[2] >> 2;

		if ((value & 0x03) != 0x01)
		{
			return false;
		}
		signal = -1;

	}
	else if ((data[2] & (0x3 << 4)) != 0)
	{
		int32_t value = data[2] >> 4;

		if ((value & 0x03) != 0x01)
		{
			return false;
		}
		
		signal = 1;
	}
	uint16_t xvalue = ((uint16_t)data[3]) & 0x00FF;
	xvalue <<= 2;
	uint16_t lvalue = ((uint16_t)(data[2]>>6)) & 0x0003;
	xvalue |= lvalue;
	_arm = xvalue * signal;
	//if (_side == OpSide::Arm)
//	{
		
	//	_bucket = 0;
	//	OutputDebugStringA(std::to_string(_arm).c_str());
	//	OutputDebugStringA("\n");
//	}
//	else if (_side == OpSide::Bucket)
/* {
		_bucket = xvalue * signal;
		_arm = 0;
	//	OutputDebugStringA(std::to_string(_bucket).c_str());
	//	OutputDebugStringA("\n");
	}
*/
	if ((data[0] & (0x3 << 2)) != 0)
	{
		int32_t value = data[0] >> 2;

		if ((value & 0x03) != 0x01)
		{
			return false;
		}
		signal = -1;

	}
	else if ((data[0] & (0x3 << 4)) != 0)
	{
		int32_t value = data[0] >> 4;

		if ((value & 0x03) != 0x01)
		{
			return false;
		}

		signal = 1;
	}
	
	xvalue = ((uint16_t)data[1]) & 0x00FF;
	xvalue <<= 2;
	lvalue = ((uint16_t)(data[0] >> 6)) & 0x0003;
	xvalue |= lvalue;
	_bucket = xvalue * signal;
	std::string str = "arm:";
	str +=std::to_string(_arm)+", bucket :"+  std::to_string(_bucket);
	str += "\n";
	OutputDebugStringA(str.c_str());
	return true;
}
ControlStatus CControlSensor::CheckStatus()
{
	if (_gear != RemoNet::Gears::N) return ControlStatus::GearNotN;
	if (_bootstrap != false) return ControlStatus::BootStrap;
	if (_startup != false) return ControlStatus::Startup;
	//if (_emergenyd != false) return ControlStatus::Emergency;
	if (_steer != 0) return ControlStatus::Steer;
	if (abs(_throttle) > 5) return ControlStatus::Throttle;
	if (abs(_brake) > 5) return ControlStatus::Brake;
//	if (_side != OpSide::Empty) return ControlStatus::ArmTriger;
	return ControlStatus::Ok;	
}
void CControlSensor::OnBootstrapd(bool ret)
{
	_bootstrapd = ret;
}
void CControlSensor::OnStartupd(bool ret)
{
	_startupd = ret;
}
void CControlSensor::Start()
{
	cannet_frame frame;
	memset(&frame, 0, sizeof(frame));
	frame.canid = htonl(0x0601);
	frame.dlc = 3;
	frame.data[0] = 0xb3;
	frame.data[1] = 0x10;
	frame.data[2] = 0xff;
	CIOBuffer buffer;
	memcpy(buffer.Buffer, &frame, sizeof(frame));


	buffer.Length = sizeof(frame);
	_socket->Write(&buffer);

}

void CControlSensor::Stop()
{

}
void CControlSensor::SetSensorSocket(SensorSocket<CControlSensor>* can)
{
	_socket = can;
}