 
#include <assert.h>
#include <iostream>
#include <memory>
#include "../common/comm.h"
#include "api.h"
#include "protocol.pb.h"
#include "../common/socket_client.h"
#include "../common/iobuffer.h"
#include "PeerConnection.h"
#include "VideoRenderer.h"
#include "message_queue.h"

PeerConnectionWrapper::PeerConnectionWrapper(SocketClient* d) :_client(d)
{
	 
	_nativeHandle = nullptr;
}

PeerConnectionWrapper::~PeerConnectionWrapper()
{
	if (_nativeHandle != nullptr)
	{

	}
}
void PeerConnectionWrapper::Initialize(int32_t peer,int32_t index ,bool bNeedData)
{
	bDataChannel=bNeedData;
    _nativeHandle = mrsPeerConnectionCreate();
	std::cout<<"native handle"<<_nativeHandle<<std::endl;
    if (_nativeHandle == nullptr)
	{
		mrsPeerConnectionClose(&_nativeHandle);

		_nativeHandle = nullptr;
		return;
	}
	mrsPeerConnectionRegisterConnectedCallback(_nativeHandle, &PeerConnectionWrapper::ConnectionCallBack, this);

	mrsPeerConnectionRegisterLocalSdpReadytoSendCallback(_nativeHandle,peer,index, &PeerConnectionWrapper::LocalSdpReadytoSendCallback, this);
	mrsPeerConnectionRegisterIceCandidateReadytoSendCallback(_nativeHandle,peer,index, &PeerConnectionWrapper::IceCandidateReadytoSendCallback, this);
    if(bNeedData)
		mrsPeerConnectionRegisterChannelCallback(_nativeHandle, &SocketClient::MessageCallback, _client, nullptr, nullptr, nullptr, nullptr);
}
void PeerConnectionWrapper::Close()
{
	if(_nativeHandle==nullptr) return;
	mrsPeerConnectionRegisterConnectedCallback(_nativeHandle, nullptr, nullptr);

	mrsPeerConnectionRegisterLocalSdpReadytoSendCallback(_nativeHandle,0,0, nullptr, nullptr);
	mrsPeerConnectionRegisterIceCandidateReadytoSendCallback(_nativeHandle,0, 0,nullptr, nullptr);
	if(bDataChannel)
	  mrsPeerConnectionRemoveDataChannel(_nativeHandle);
 	mrsPeerConnectionClose(&_nativeHandle);
	 _nativeHandle=nullptr;
}

void PeerConnectionWrapper::AddDataChannel(bool ordered, bool reliable)
{
	mrsPeerConnectionAddDataChannel(_nativeHandle, ordered, reliable);
}
void PeerConnectionWrapper::RemoveDataChannel()
{
	mrsPeerConnectionRemoveDataChannel(_nativeHandle);
}
void PeerConnectionWrapper::ConnectionCallBack(void* UserData)
{
	PeerConnectionWrapper* lhs = static_cast<PeerConnectionWrapper*>(UserData);
	lhs->OnConnect();
}
void PeerConnectionWrapper::OnConnect()
{
	bConnected = true;

}
const std::string  kOffer = "offer";
const std::string kAnswer = "answer";
void PeerConnectionWrapper::LocalSdpReadytoSendCallback(void* user_data,int32_t peer,int32_t view, const char* type, const char* sdp_data)
{
	auto lhs = static_cast<PeerConnectionWrapper*>(user_data);
	lhs->OnLocalSdpReadytoSend(peer,view,type, sdp_data);
}
void PeerConnectionWrapper::OnLocalSdpReadytoSend(int32_t peer,int32_t view,const char* type, const char* sdp)
{
	//std::string newsdp = ForceSdpCodecs(sdp, PreferredAudioCodec, PreferredAudioCodecExtraParamsRemote, PreferredVideoCodec, PreferredVideoCodecExtraParamsRemote);
	//std::cout<<type<<","<<__LINE__<<std::endl;
	if (kOffer == type)
	{
		_client->WriteOffer(EgoType::User,peer,view, type, sdp);
	}
	else if(kAnswer==type)
	{
		
		_client->WriteAnswer(EgoType::User,peer,view, type, sdp);
	}
	//mrsWe
}
void PeerConnectionWrapper::SetRemoteDescription(const char* type, std::string sdp)
{
	//std::string newsdp = ForceSdpCodecs(sdp, PreferredAudioCodec, PreferredAudioCodecExtraParamsLocal, PreferredVideoCodec, PreferredVideoCodecExtraParamsLocal);
	auto ret = mrsPeerConnectionSetRemoteDescription(_nativeHandle, type, sdp.c_str());
	assert(ret == true);

}
void PeerConnectionWrapper::IceCandidateReadytoSendCallback(void* user_data,int32_t peer,int32_t index, const char* sdp, int32_t sdp_mline_index, const char* sdp_mid)
{
	auto lhs = static_cast<PeerConnectionWrapper*>(user_data);
	lhs->OnIceCandidateReadytoSend(peer,index,sdp, sdp_mline_index, sdp_mid);
}
void PeerConnectionWrapper::OnIceCandidateReadytoSend(int32_t peer,int32_t index,const char* candidate, int32_t sdp_mline_index, const char* sdp_mid)
{
		_client->WriteCandidate(EgoType::User,peer,index, candidate, sdp_mline_index, sdp_mid);
	 
}
void PeerConnectionWrapper::AddIceCandidate(const char* sdp, int32_t sdp_mline_index, const char* sdp_mid)
{
	auto ret = mrsPeerConnectionAddIceCandidate(_nativeHandle, sdp, sdp_mline_index, sdp_mid);
	assert(ret == true);
}
bool PeerConnectionWrapper::CreateOffer()
{
//	std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
	auto ret = mrsPeerConnectionCreateOffer(_nativeHandle);// == mrsResult::kSuccess;
	assert(ret == true);
	return true;
}

bool PeerConnectionWrapper::CreateAnswer()
{
	//std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
	auto ret = mrsPeerConnectionCreateAnswer(_nativeHandle); ;
	assert(ret == true);
	return true;
}

void PeerConnectionWrapper::AddLocalArgb32VideoFrameReady(PeerConnectionARGBVideoFrameCallback fn, void* user_data)
{
	mrsPeerConnectionRegisterARGBLocalVideoFrameCallback(_nativeHandle, fn, user_data);
}
void PeerConnectionWrapper::AddRemoteArgb32VideoFrameReady(PeerConnectionARGBVideoFrameCallback fn, void* user_data)
{
	mrsPeerConnectionRegisterARGBRemoteVideoFrameCallback(_nativeHandle, fn, user_data);
}
/*
void PeerConnectionWrapper::AddLocalVideoTrack(CaptureType type, DisplayResolution solution,int32_t fps)
{
	auto ret = mrsPeerConnectionAddLocalVideoTrack(_nativeHandle,type,solution,fps);
	assert(ret == true);
}
*/
void PeerConnectionWrapper::AddLocalVideoTrack(int32_t rotation,const std::string& label,const std::string &videourl,std::int32_t width,std::int32_t height)
{
	 
     auto ret=mrsPeerConnectionAddLocalRtspTrack(_nativeHandle,label,videourl,rotation,width,height);
}
void PeerConnectionWrapper::AddLocalAudioTrack()
{
	auto ret = mrsPeerConnectionAddLocalAudioTrack(_nativeHandle);
	assert(ret == true);
}
void  PeerConnectionWrapper::MessageCallback(void* user_data, const void* data, const uint64_t size)
{
    
}
void PeerConnectionWrapper::SendData(CIOBuffer* pBuffer)
{
	auto ret=mrsPeerConnectionSendDataChannelMessage(_nativeHandle,pBuffer->Buffer,pBuffer->Length);
	//assert(ret==true);
	if(ret==false)
	{
		printf("send failed  to Peer \n");
	}
}