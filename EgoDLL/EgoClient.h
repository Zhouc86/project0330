#pragma once
#include "message_queue.h"
#include "api.h"
#define  WM_NOTIFY_CONNECTED WM_APP+100
class IEgoNotify;
class CEgoWindow;
class IRender;
#include "../common/sensor_socket.h"
#include "./include/EgoInterface.h"
class CControlSensor;
class CCarSim;
class COutSim;
class CRadarSim;
#ifdef LIDAR_SENSOR
class CLidarSim;
#endif
 
class CEgoClient : public INativeNotify
{
public:
	CEgoClient(IEgoNotify * C);
	void Login(std::string account, std::string pass);
	void OnCarConnect(int32_t peer);
	void OnCarLeave();
	void ReqCarList();
	void Start(std::array<IRender*, RenderPosition::ALL>& ar);
	void OnRadarData(int32_t r0, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5);
	void OnImuData(int32_t x, int32_t y);
	void OnPingValue(int32_t value);
	void OnStopAck();
	void OnBootstrapd(bool ret);
	void OnStartupd(bool ret);
	ControlStatus CheckStatus();
	//virtual void Process(CIOBuffer* pBuffer) override;
	virtual void OnRobot(const RemoNet::Robot& robot) override;
	virtual void OnConnected(bool bRet) override;
	virtual void OnSigin(bool bRet)override;
	virtual void OnVideoRep(int32_t index, int32_t peer) override;
	virtual void OnVideoOffer(int32_t index, const char* type, const char* sdp) override;
	virtual void OnVideoAnswer(int32_t index, const char* type, const char* sdp) override;
	virtual void OnVideoCandidate(int32_t index, const char* candidate, int32_t sdp_mline_index, const char* sdp_mid) override;
	virtual void OnNotifyDel(int32_t peer, EgoType type) override;
	virtual void OnVideoLeave(int32_t peer, EgoType type) override;
	virtual void OnNotifyKick() override;
	virtual void OnMessageFrameNotify(ChannelType type, int16_t cmd, int16_t length, const void* data) override;
#ifdef LIDAR_SENSOR
	virtual void OnLidarData(bool isLeft, bool isDense, int32_t seq, PointXYZI* data, int32_t length) override;
#endif
private:
	std::unique_ptr<SocketClient>  _client;
	int32_t _carpeer;
	std::string _account;
	std::string _pass;
	IEgoNotify* _notify;
	bool  _connected;
	std::vector<std::unique_ptr<UserCamera> > _userInfo;
	CUpdateThread _updateThread;
	std::vector<std::unique_ptr<CEgoWindow>> _WindowArray;
	std::unique_ptr<SensorSocket<CControlSensor>> _control;
	std::unique_ptr<CCarSim> _carsim;
	std::unique_ptr<COutSim> _outsim;
	std::unique_ptr<CRadarSim> _radarsim;
#ifdef LIDAR_SENSOR
	std::unique_ptr<CLidarSim> _lidarsim;
#endif
	std::uniform_int_distribution<int> _u;
	std::default_random_engine _e;
};

