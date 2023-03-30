#pragma once

#ifdef EGODLL_EXPORTS
#define EGODLL_API __declspec(dllexport)
#else
#define EGODLL_API __declspec(dllimport)
#endif
#include <stdint.h>
#include <memory>
#include <array>
#include "../common/comm.h"

struct PointXYZI  ///< user defined point type
{
	float x;
	float y;
	float z;
	uint8_t intensity;
};
class IRender
{
public:
	virtual void OnRender(std::unique_ptr<uint8_t>& pBuffer,int32_t width,int32_t height) = 0;

};

class IEgoNotify
{
public:
	
	virtual void OnRobot(std::unique_ptr<UserCamera>& info) = 0;
	virtual void OnSigin(bool bRet) = 0;
	virtual void OnNotifyDel(int32_t peer) = 0;
	virtual void OnNotifyKickOff() = 0;
	virtual void OnNotifyRadar(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5) = 0;
	virtual void OnNotifyImu(int32_t x, int32_t y) = 0;
	virtual void OnNotifyPing(int32_t value) = 0;
#ifdef LIDAR_SENSOR
	virtual void OnLidarData(bool isLeft, bool isDense, int32_t seq, PointXYZI* data, int32_t length) = 0;
#endif
};
enum ControlStatus
{
	Ok,
	GearNotN,
	BootStrap,//已加电
	Startup,//已启动
	Emergency,
	Steer,
	Throttle,
	Brake,
	ArmTriger,
};
class IEgoControl
{
public:
	virtual void Login(std::string account, std::string pass) = 0;
	virtual void Start(std::array<IRender*, RenderPosition::ALL>& ar) = 0;
	//virtual void ReqCarList() = 0;
	virtual void OnCarConnect(int32_t peer) = 0;
	virtual void OnCarLeave() = 0;
	virtual ControlStatus CheckStatus() = 0;

};

EGODLL_API IEgoControl* GetEgoController(IEgoNotify* n);