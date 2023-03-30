#pragma once
#include<mutex>
#include<memory>
#include<string>
#include<condition_variable>
#include <vector> 
#include "protocol.pb.h"
#include "../common/iobuffer.h"
#include "../common/socket_client.h"
#include "../common/sensor_socket.h"
#include "../common/update_thread.h"
#ifdef LIDAR_SENSOR
#include "./lidar/lidar_sensor.h"
#endif
class CRadarSensor;
class CImuSensor;
class CRobotSensor;
class SocketClient;
class CPeerConnection;
class VideoRenderer;
struct ImuData;
enum MessageType:int32_t
{
    ReqVideo,
    RepVideo,
    Connected,
    Leave,
    StopSensor,
    Ping,
    AsyncMessage,
    BootStrap,
    Startup,
};
struct Message
{
    MessageType cmd;
    int64_t param_l;
    int64_t param_r;
    
};
 
struct LocalCameraInfo
{
   int32_t index;
   std::string label;
};
 

class CIOBuffer;
struct RadarData;

//20230328
struct Radar_Frame;

class CMessageQueue:public INativeNotify
{
public:
   CMessageQueue();
   virtual ~CMessageQueue();
   void Create();
   void EnQueue(CIOBuffer* pBuffer);
   void Process();
   virtual void OnAdd(bool bRet) override;
   virtual void OnConnected(bool bRet) override;
  // virtual bool IsCarId(int32_t value) override;
 //  virtual void WriteCanMessage(std::unordered_map<int32_t, cannet_frame>& node,bool islidar) override;
#ifdef WIN32
	virtual void OnVideoRep(int32_t index,int32_t peer) override;
#else
    virtual void OnVideoReq(int32_t index,int32_t peer) override;
#endif
	virtual void OnVideoOffer(int32_t index,const char* type, const char* sdp) override;
	virtual void OnVideoAnswer(int32_t index, const char* type, const char* sdp) override;
	virtual void OnVideoCandidate(int32_t index,const char* candidate,
		int32_t sdp_mline_index,
		const char* sdp_mid) override;
	/*virtual void OnVideoAddTrack(RemoteVideoTrackWrapper* ptr) = 0;*/
	//virtual void OnCancelReq(int32_t index) override;
	virtual void OnVideoLeave(int32_t peer,EgoType type) override;


   virtual void OnMessageFrameNotify(ChannelType type,int16_t cmd,int16_t length,const void * data) override;
   void  OnNotifyConnected(bool bRet);
   void OnNotifyReq(int32_t index);
   void OnNotifyRep(int32_t index);
   void OnNotifyStopSensor();
   void OnNotifyLeave();
   void OnNotifyMessage();
   void OnNotifyPing(int64_t value);
   void InitPeerConnection(int32_t peer,int32_t index);
   void WriteIMUData(ImuData* data);
   void WriteRadarData(RadarData& data);
   void WriteRobotStatus(int32_t value,int32_t desc);
   void WritePacket(ChannelType type, CIOBuffer * pBuffer);
   void SwitchCamera(bool front);
   void SetTick(long long tick);
#ifdef LIDAR_SENSOR
   void WriteLidarPoint(const PointCloudMsg<PointXYZI>& msg,ChannelType side);
#endif
   void StopCar();
 //  void StartCar();
   void CheckSignal();
private:
   std::mutex _lock;
   std::condition_variable _cv;
   CIOBuffer* Head;
   CIOBuffer* Tail;
//	std::unique_ptr<PeerConnectionWrapper> _peer_video;
	std::vector<std::unique_ptr<CPeerConnection> > _peerArray;
   std::vector<LocalCameraInfo> _cameraArray;
	//EgoType _egoType;
    //int32_t _indexOffset;
   // std::vector<std::unique_ptr<VideoRenderer> > _windowArray;
//	std::unique_ptr<VideoRenderer> _main_window;
//	std::unique_ptr<VideoRenderer> _arm_window;
	std::unique_ptr<SocketClient>  _client;
    std::unique_ptr<SensorSocket<CRadarSensor>> _radar;
    std::unique_ptr<SensorSocket<CImuSensor>> _imu;
    std::unique_ptr<SensorTCP<CRobotSensor>> _robot;
    
    
    int32_t _peerId;
  //  int32_t _canport;
  //  int32_t _hostport;
   // int32_t _lidarport;
   // std::string _canip;
	std::string _serial; 
    std::string _name;
  
    bool bStopedCar;
    CUpdateThread _updatethread;
    std::mutex _canLock;
    long long _curTick;
    ////////////////////////////////////////////////////////////////
#ifdef LIDAR_SENSOR
    std::unique_ptr<CLidarSensor> _left;
    std::unique_ptr<CLidarSensor> _right;
#endif
};
