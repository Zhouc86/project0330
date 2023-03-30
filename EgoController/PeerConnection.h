#pragma once 
#include <vector>
#include <string>
#include <functional>
#include <future>
#include <mutex>
#include "api.h"
class SocketClient;
 class PeerConnectionWrapper
 {
public:
   PeerConnectionWrapper(SocketClient* d);
   virtual ~PeerConnectionWrapper();
	void Initialize(int32_t peer,int32_t index,bool bNeedData);
    static void ConnectionCallBack(void* UserData);
	static void LocalSdpReadytoSendCallback(void* user_data,int32_t peer,int32_t view, const char* type, const char* sdp_data);
	static void IceCandidateReadytoSendCallback(void* user_data,int32_t peer,int32_t view, const char* sdp, int32_t sdp_mline_index, const char* sdp_mid);
	static void  MessageCallback(void* user_data, const void* data, const uint64_t size);
    void AddLocalArgb32VideoFrameReady(PeerConnectionARGBVideoFrameCallback fn, void* user_data);
	void AddRemoteArgb32VideoFrameReady(PeerConnectionARGBVideoFrameCallback fn, void* user_data);
	void AddDataChannel(bool ordered, bool reliable);
	void RemoveDataChannel();
    void SetRemoteDescription(const char* type, std::string sdp);
	bool CreateAnswer();
	bool CreateOffer();
	void AddIceCandidate(const char* sdp, int32_t sdp_mline_index, const char* sdp_mid);
	void Close();
//	void AddLocalVideoTrack(CaptureType type, DisplayResolution solution,int32_t fps);
	void AddLocalVideoTrack(int32_t index,const std::string& label,const std::string &videourl,int32_t width,int32_t height);
	void AddLocalAudioTrack();
	void SendData(CIOBuffer* pBuffer);
private:
	void OnConnect();

	void OnLocalSdpReadytoSend(int32_t peer,int32_t view,const char* type, const char* sdp);
	void OnIceCandidateReadytoSend(int32_t peer,int32_t view,const char* sdp, int32_t sdp_mline_index, const char* sdp_mid);
private:
	PeerConnectionHandle _nativeHandle;
    SocketClient * _client;
    bool bDataChannel;
    bool bConnected;
 };