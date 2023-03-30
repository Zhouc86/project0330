#pragma once
#include<mutex>
#include<memory>
#include<string>
#include<condition_variable>
#include <vector> 
#include "protocol.pb.h"
#include "../common/socket_client.h"
#include "../common/socket_can.h"
#include "../common/update_thread.h"
#include "socket_lidar.h"
class SocketClient;
class PeerConnectionWrapper;
class VideoRenderer;
enum MessageType:int32_t
{
    ReqVideo,
    RepVideo,
    Connected,
    Leave,
    AsyncMessage
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
   std::string uri;
   DisplayResolution solution;
   
   

};


class CIOBuffer;
class CMessageQueue:public INativeNotify,public ICanNotify
{
public:
   CMessageQueue();
   virtual ~CMessageQueue();
   void Create();
   void EnQueue(CIOBuffer* pBuffer);
   void Process();
   virtual void OnAdd(bool bRet) override;
   virtual void OnConnected(bool bRet) override;
   virtual bool IsCarId(int32_t value) override;
   virtual void WriteCanMessage(std::unordered_map<int32_t, cannet_frame>& node,bool islidar) override;
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


   virtual void OnMessageFrameNotify(const void* data, const int32_t size) override;
   void  OnNotifyConnected(bool bRet);
   void OnNotifyReq(int32_t index);
   void OnNotifyRep(int32_t index);
   void OnNotifyLeave();
   void OnNotifyMessage();
   void InitPeerConnection(int32_t peer,int32_t index);
   void StopCar();
   void StartCar();
   void CheckSignal();
private:
   std::mutex _lock;
   std::condition_variable _cv;
   CIOBuffer* Head;
   CIOBuffer* Tail;
//	std::unique_ptr<PeerConnectionWrapper> _peer_video;
	std::vector<std::unique_ptr<PeerConnectionWrapper> > _peerArray;
    std::vector<LocalCameraInfo> _cameraArray;
	EgoType _egoType;
    int32_t _indexOffset;
    std::vector<std::unique_ptr<VideoRenderer> > _windowArray;
//	std::unique_ptr<VideoRenderer> _main_window;
//	std::unique_ptr<VideoRenderer> _arm_window;
	std::unique_ptr<SocketClient>  _client;
    std::unique_ptr<SocketCan> _can;
    std::unique_ptr<SocketLidar> _lidar;
    std::vector<int32_t> _lidarArray;
    std::vector<int32_t> _emergencyArray;
    std::vector<int32_t> _carArray;
    int32_t _peerId;
    int32_t _canport;
    int32_t _hostport;
    int32_t _lidarport;
    std::string _canip;
	std::string _serial; 
    std::string _name;
    bool bDataChannelCreated;
    bool bStopedCar;
    CUpdateThread _updatethread;
    std::mutex _canLock;
    long long _curTick;
};
