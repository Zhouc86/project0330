#pragma once
class CIOBuffer;
/*
class SensorSocket;
class ISensorNotify
{
public:
 
    virtual void Notify(int8_t * buffer,int32_t size) =0;
	virtual void SetSensorSocket(SensorSocket* can) =0;
#ifndef WIN32
	virtual void Start() = 0;
	virtual void Stop() = 0;
#endif
	virtual ~ISensorNotify()=default;
};
*/
struct PointXYZI;
class INativeNotify {
public:
#ifdef WIN32
	 
	 
	virtual void OnSigin(bool bRet) = 0;
	virtual void OnRobot(const RemoNet::Robot& robot) = 0;
#else
	virtual void OnAdd(bool bRet) = 0;
#endif
	virtual void OnConnected(bool bRet) = 0;
#ifdef WIN32
	virtual void OnVideoRep(int32_t index, int32_t peer) = 0;

	virtual void OnNotifyDel(int32_t peer, EgoType type) = 0;
	virtual void OnNotifyKick() = 0;
#ifdef LIDAR_SENSOR
	virtual void OnLidarData(bool isLeft, bool isDense, int32_t seq, PointXYZI* data, int32_t length) = 0;
#endif
#else
	virtual void OnVideoReq(int32_t index, int32_t peer) = 0;
#endif
	virtual void OnVideoOffer(int32_t index, const char* type, const char* sdp) = 0;
	virtual void OnVideoAnswer(int32_t index, const char* type, const char* sdp) = 0;
	virtual void OnVideoCandidate(int32_t index, const char* candidate,
		int32_t sdp_mline_index,
		const char* sdp_mid) = 0;
	/*virtual void OnVideoAddTrack(RemoteVideoTrackWrapper* ptr) = 0;*/
//	virtual void OnCancelReq(int32_t index) = 0;
	virtual void OnVideoLeave(int32_t peer, EgoType type) = 0;


	virtual void OnMessageFrameNotify(ChannelType type,int16_t cmd,int16_t length,const void * data) = 0;
};
/*
#ifdef WIN32
class IPeerNotify
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void OnPeerMessage(int16_t cmd, int16_t length, const void* data) = 0;
};
#endif
*/