 
#include <assert.h>
#include <iostream>
#include <memory>
#include "../common/comm.h"
#include "api.h"
#include "protocol.pb.h"
#include "../common/socket_client.h"
#include "../common/iobuffer.h"
#include "peer_connection.h"
//#include "VideoRenderer.h"
//#include "message_queue.h"
std::mutex CPeerConnection::_lock;
CPeerConnection::CPeerConnection(ChannelType type, SocketClient* d) :_client(d),_type(type)
{
	 
	_nativeHandle = nullptr;
 
	bReadyChannel=false;
 
}

CPeerConnection::~CPeerConnection()
{
	if (_nativeHandle != nullptr)
	{

	}
}
void CPeerConnection::Initialize(int32_t peer,int32_t index)
{
	std::lock_guard<std::mutex> l(_lock);
    _nativeHandle = mrsPeerConnectionCreate();
	std::cout<<"native handle"<<_nativeHandle<<std::endl;
    if (_nativeHandle == nullptr)
	{
	//	mrsPeerConnectionClose(&_nativeHandle);

	//	_nativeHandle = nullptr;
		return;
	}
	mrsPeerConnectionRegisterConnectedCallback(_nativeHandle, &CPeerConnection::ConnectionCallBack, this);

	mrsPeerConnectionRegisterLocalSdpReadytoSendCallback(_nativeHandle,peer,index, &CPeerConnection::LocalSdpReadytoSendCallback, this);
	mrsPeerConnectionRegisterIceCandidateReadytoSendCallback(_nativeHandle,peer,index, &CPeerConnection::IceCandidateReadytoSendCallback, this);
    
	mrsPeerConnectionRegisterChannelCallback(_nativeHandle, &CPeerConnection::MessageCallback, this, nullptr, nullptr, nullptr, nullptr);
}
void CPeerConnection::Close()
{
	std::lock_guard<std::mutex> l(_lock);
	if(_nativeHandle==nullptr) return;
	bReadyChannel = false;
	mrsPeerConnectionRegisterConnectedCallback(_nativeHandle, nullptr, nullptr);

	mrsPeerConnectionRegisterLocalSdpReadytoSendCallback(_nativeHandle,0,0, nullptr, nullptr);
	mrsPeerConnectionRegisterIceCandidateReadytoSendCallback(_nativeHandle,0, 0,nullptr, nullptr);
	 
	mrsPeerConnectionRemoveDataChannel(_nativeHandle);
 	mrsPeerConnectionClose(&_nativeHandle);
	 _nativeHandle=nullptr;
}

void CPeerConnection::AddDataChannel(bool ordered, bool reliable)
{
	mrsPeerConnectionAddDataChannel(_nativeHandle, ordered, reliable);
}
void CPeerConnection::RemoveDataChannel()
{
	mrsPeerConnectionRemoveDataChannel(_nativeHandle);
}
void CPeerConnection::ConnectionCallBack(void* UserData)
{
	CPeerConnection* lhs = static_cast<CPeerConnection*>(UserData);
	lhs->OnConnect();
}
void CPeerConnection::OnConnect()
{
	bConnected = true;

}
const std::string  kOffer = "offer";
const std::string kAnswer = "answer";
void CPeerConnection::LocalSdpReadytoSendCallback(void* user_data,int32_t peer,int32_t view, const char* type, const char* sdp_data)
{
	auto lhs = static_cast<CPeerConnection*>(user_data);
	lhs->OnLocalSdpReadytoSend(peer,view,type, sdp_data);
}
void CPeerConnection::OnLocalSdpReadytoSend(int32_t peer,int32_t view,const char* type, const char* sdp)
{
	//std::string newsdp = ForceSdpCodecs(sdp, PreferredAudioCodec, PreferredAudioCodecExtraParamsRemote, PreferredVideoCodec, PreferredVideoCodecExtraParamsRemote);
	//std::cout<<type<<","<<__LINE__<<std::endl;
	if (kOffer == type)
	{
		_client->WriteOffer(peer,view, type, sdp);
	}
	else if(kAnswer==type)
	{
		
		_client->WriteAnswer(peer,view, type, sdp);
	}
	//mrsWe
}
void CPeerConnection::SetRemoteDescription(const char* type, std::string sdp)
{
	//std::string newsdp = ForceSdpCodecs(sdp, PreferredAudioCodec, PreferredAudioCodecExtraParamsLocal, PreferredVideoCodec, PreferredVideoCodecExtraParamsLocal);
	auto ret = mrsPeerConnectionSetRemoteDescription(_nativeHandle, type, sdp.c_str());
	assert(ret == true);

}
void CPeerConnection::IceCandidateReadytoSendCallback(void* user_data,int32_t peer,int32_t index, const char* sdp, int32_t sdp_mline_index, const char* sdp_mid)
{
	auto lhs = static_cast<CPeerConnection*>(user_data);
	lhs->OnIceCandidateReadytoSend(peer,index,sdp, sdp_mline_index, sdp_mid);
}
void CPeerConnection::OnIceCandidateReadytoSend(int32_t peer,int32_t index,const char* candidate, int32_t sdp_mline_index, const char* sdp_mid)
{
		_client->WriteCandidate(peer,index, candidate, sdp_mline_index, sdp_mid);
	 
}
void CPeerConnection::AddIceCandidate(const char* sdp, int32_t sdp_mline_index, const char* sdp_mid)
{
	auto ret = mrsPeerConnectionAddIceCandidate(_nativeHandle, sdp, sdp_mline_index, sdp_mid);
	assert(ret == true);
}
bool CPeerConnection::CreateOffer()
{
//	std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
	auto ret = mrsPeerConnectionCreateOffer(_nativeHandle);// == mrsResult::kSuccess;
	assert(ret == true);
	return true;
}

bool CPeerConnection::CreateAnswer()
{
	//std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
	auto ret = mrsPeerConnectionCreateAnswer(_nativeHandle); ;
	assert(ret == true);
	return true;
}

void CPeerConnection::AddLocalArgb32VideoFrameReady(PeerConnectionARGBVideoFrameCallback fn, void* user_data)
{
	mrsPeerConnectionRegisterARGBLocalVideoFrameCallback(_nativeHandle, fn, user_data);
}
void CPeerConnection::AddLocalAudioTrack()
{
	mrsPeerConnectionAddLocalAudioTrack(_nativeHandle);
}
void CPeerConnection::AddRemoteArgb32VideoFrameReady(PeerConnectionARGBVideoFrameCallback fn, void* user_data)
{
	mrsPeerConnectionRegisterARGBRemoteVideoFrameCallback(_nativeHandle, fn, user_data);
}
 
#ifdef WEBRTC_LINUX
void CPeerConnection::AddLocalVideoTrack(RenderPosition type,int32_t index)
{
	 //PeerConnectionHandle peerHandle, CaptureType type, int32_t fps)
     auto ret=mrsPeerConnectionAddLocalVideoTrack(_nativeHandle, type,index);
}
 
void CPeerConnection::SwitchCapture(bool front)
{
	mrsPeerConnectionSwitchCapture(_nativeHandle, front);
}
void CPeerConnection::SetOtherCtx(void * data)
{
    mrsPeerConnectionSetCtx(_nativeHandle,data);
}
void * CPeerConnection::GetCurrentCtx()
{
	return mrsPeerConnectionCurrentCtx(_nativeHandle);
}
#else
void CPeerConnection::SetDataReady()
{
	bReadyChannel = true;
}
bool CPeerConnection::IsDataReady()
{
	return bReadyChannel;
}
#endif
void  CPeerConnection::MessageCallback(void * user_data,const void * data,const int32_t size)
{
    CPeerConnection* lhs=static_cast<CPeerConnection*>(user_data);
	lhs->OnPeerMessage(data,size);



	
}
void CPeerConnection::OnPeerMessage(const void * data,const int32_t size)






{
	 if(size<MessageHead::Size()) return;
     MessageHead Head;
     int8_t* Data=(int8_t *)data;
     Head.Deserialize(Data);
     if(size<MessageHead::Size()+Head.Length) return;
     auto ptr=Data+MessageHead::Size();
	 if(Head.Command==RemoNet::CC_ASKDATACHANNEL)
	 {
		 
			bReadyChannel=true;
			//return;
	 }
	 if(!bReadyChannel) return;
	  _client->OnPeerMessage(_type,Head.Command,Head.Length,ptr);
	 
	  
}

void CPeerConnection::SendData(CIOBuffer* pBuffer)
{
 
	if(!bReadyChannel) return;
 
	auto ret=mrsPeerConnectionSendDataChannelMessage(_nativeHandle,pBuffer->Buffer,pBuffer->Length);
	 
	if(ret==false)
	{
		printf("send failed  to Peer \n");
	}
}