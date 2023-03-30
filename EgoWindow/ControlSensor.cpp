#include "stdafx.h"
#include "resource.h"
#include "Protocol.pb.h"
#include "ControlSensor.h"
#include "MainDlg.h"
#include <stdio.h>
#include "../common/iobuffer.h"
#include "WindowThread.h"

CControlSensor::CControlSensor(CThreadWindow* dlg) :_window(dlg)
{
	_collection = Sensor_Collection::Sensor_None;
	_brake = _bucket = _throttle = _steer = 0;
	_bootstrap = _startup = false;
 

}
void CControlSensor::OnBootstrapd(bool ret)
{
	_bootstrapd = ret;
}
void CControlSensor::OnStartupd(bool ret)
{
	_startupd = ret;
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
		int32_t id = ntohl(p->canid) & 0xFFF;
	//	char text[64];
	//	sprintf(text, "%x\n", id);
	////	std::string ret=std::to_string(id)
	//	OutputDebugString(text);
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
		p++;
		//std::string str = std::to_string(id) +"\n";
		
		 
	
	}
	if (_startup&&_collection == Sensor_Collection::Sensor_All)
	{
		RemoNet::CCRobotAnalog _robot;
		_robot.set_brake(_brake);
		_robot.set_arm(_arm);
		_robot.set_bucket(_bucket);
		_robot.set_steer(_steer);
		_robot.set_throttle(_throttle);
	 
	//	_robot.set_emergency(_emergeny);
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
	bool ret = (data[0] & 0x01) != 0;
	if(ret==true)
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
		if (!_startup&&_bootstrapd)
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
	_resume = (data[2]) != 0;
	 
	_throttle = data[4];
	_brake = data[5];
 	return true;
}
bool CControlSensor::OnLeftJoyStick(int8_t* data)
{
	 
	return true;
	int32_t signal = 0;
	if ((data[0] & 0x3)!=0)
	{
		
		if ((data[0] & 0x03) != 0x01)
		{
			return false;
		}
		signal = 0;
	 
	}
	if ((data[0] & (0x3<<2)) !=0)
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
	if((data[6]&0x3)==0x01)
		_steer = xvalue * signal;
	//OutputDebugString(std::to_string(_steer).c_str());
//	char buffer[64];
//	std::string ret;
//  	for (int i=0;i<8;i+=2)
//  	{
//  		int16_t value = (data[5] >> i) & 0x3;
//  		sprintf_s(buffer, "%x ", value);
//  		ret += buffer;
//  	}
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
	
//   	for (int i = 0; i < 8; i += 2)
//  	{
//  		int16_t value = (data[6] >> i) & 0x3;
//  		sprintf_s(buffer, "%x ", value);
//  		ret += buffer;
//  	}
// 	for (int i = 5; i < 8; i++)
// 	{
// 		int16_t t = (data[i]) & 0x00FF;
// 		sprintf_s(buffer, "%x ", t);
// 		ret += buffer;
// 	}
	//ret += "\n";
	//OutputDebugString(ret.c_str());
	return true;
	
}
bool CControlSensor::OnRightJoyStick(int8_t* data)
{
	_side = OpSide::Empty;
	if (((data[5] >> 2) & 0x3) == 0x01)
	{
		_side = OpSide::Arm;
	}
	else if (((data[5] >> 4) & 0x3) == 0x01)
	{
		_side = OpSide::Bucket;
	}
	 
	 
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
	if (_side == OpSide::Arm)
	{
		_arm = xvalue;
	}
	else if(_side==OpSide::Bucket)
	{
		_bucket = xvalue;
	}

	return true;
}
void CControlSensor::Start()
{

}
 
void CControlSensor::Stop()
{

}
void CControlSensor::SetSensorSocket(SensorSocket<CControlSensor>* can)
{

}
