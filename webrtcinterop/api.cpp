#include "pch.h"
 
#include "../common/comm.h"
#include "api.h"
#include "callback.h"
#include "api/create_peerconnection_factory.h"
#include "video_frame_observer.h"
#include "data_channel_observer.h"
#include "audio_frame_observer.h"
#include "peer_connection.h"
#include "sdp_utils.h"
#ifdef WEBRTC_LINUX
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/opencv.hpp>
#include "capture_op.h"
#include "gsml_capturer.h"
#endif

 

 

#ifdef WEBRTC_LINUX
#include "sanitize_string.h" 
#include "capture_op.h"
#include "gsml_capturer.h"
#include "VideoScaler.h"
#include "VideoFilter.h"
#endif
struct mrsEnumerator {
	virtual ~mrsEnumerator() = default;
	virtual void dispose() = 0;
};

rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
g_peer_connection_factory;
#ifdef WEBRTC_LINUX
std::unique_ptr<webrtc::VideoDecoderFactory> g_video_decode_factory;
#endif
/// WebRTC worker thread.
std::unique_ptr<rtc::Thread> g_worker_thread;
//  #ifdef WIN32
//  //rtc::Win32SocketServer w32_ss;
//  std::unique_ptr<rtc::Thread> g_network_thread;
//  #else
//  rtc::AutoSocketServerThread g_network_thread;
//  #endif
/// WebRTC signaling thread.
std::unique_ptr<rtc::Thread> g_signaling_thread;

std::unordered_map<
	PeerConnectionHandle,
	rtc::scoped_refptr<PeerConnection>>
	g_peer_connection_map;
/// Predefined name of the local video track.
const std::string kLocalVideoLabel("local_video");

/// Predefined name of the local audio track.
const std::string kLocalAudioLabel("local_audio");
const std::string kLocalDataChannel("local_channel");


/// WebRTC worker thread.
const std::unique_ptr<rtc::Thread>& GetWorkerThread() {
	return g_worker_thread;
}

/// WebRTC signaling thread.
const std::unique_ptr<rtc::Thread>& GetSignalingThread() {
	return g_signaling_thread;
}

void mrsCloseEnum(mrsEnumHandle* handleRef)  {
	if (handleRef) {
		if (auto& handle = *handleRef) {
			handle->dispose();
			delete handle;
			handle = nullptr;
		}
	}
}

void  mrsEnumVideoCaptureDevicesAsync(
	mrsVideoCaptureDeviceEnumCallback callback,
	void* userData,
	mrsVideoCaptureDeviceEnumCompletedCallback completedCallback,
	void* completedCallbackUserData)  {
	if (!callback) {
		return;
	}
	std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
		webrtc::VideoCaptureFactory::CreateDeviceInfo());
	if (!info) {
		if (completedCallback) {
			(*completedCallback)(completedCallbackUserData);
		}
	}
	int num_devices = info->NumberOfDevices();
	for (int i = 0; i < num_devices; ++i) {
		constexpr uint32_t kSize = 256;
		char name[kSize] = { 0 };
		char id[kSize] = { 0 };
		if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
			(*callback)(id, name, userData);
		}
	}
	if (completedCallback) {
		(*completedCallback)(completedCallbackUserData);
	}
}
std::unique_ptr<webrtc::VideoEncoderFactory> CreateEncoderFactory(bool bNullCodec)
{
		std::unique_ptr<webrtc::VideoEncoderFactory> factory;
		/*
		if (bNullCodec) {
        //       	factory=webrtc::CreateBuiltinVideoEncoderFactory(); 
#ifdef WEBRTC_LINUX
				factory=	std::make_unique<VideoEncoderFactory>(); 
#endif
 
		}
		else {
			*/
		factory=webrtc::CreateBuiltinVideoEncoderFactory(); 
 
		//}
		return factory;
}
std::unique_ptr<webrtc::VideoDecoderFactory> CreateDecoderFactory(bool nullCodec) {
	std::unique_ptr<webrtc::VideoDecoderFactory> factory;
	/*
 	if (nullCodec) {
#ifdef WEBRTC_LINUX
        //factory= webrtc::CreateBuiltinVideoDecoderFactory();  
		factory = std::make_unique<VideoDecoderFactory>();
#endif
 	} 
 	else
 	{
 		factory= webrtc::CreateBuiltinVideoDecoderFactory();  
 	}
	 */
	factory = webrtc::CreateBuiltinVideoDecoderFactory();
	return factory;
}

bool mrsWebrtcCreateFactory(bool nullCodec)
{
	 	if (g_peer_connection_factory == nullptr) {

		g_worker_thread=(rtc::Thread::Create());
		g_worker_thread->Start();
 		g_signaling_thread=(rtc::Thread::Create());
 		g_signaling_thread->Start();
#ifdef WEBRTC_LINUX
 		g_video_decode_factory=CreateDecoderFactory(nullCodec);
#endif
		  g_peer_connection_factory = webrtc::CreatePeerConnectionFactory(
			nullptr, g_worker_thread.get(), g_signaling_thread.get(),
			nullptr, webrtc::CreateBuiltinAudioEncoderFactory(),
			webrtc::CreateBuiltinAudioDecoderFactory(),
            
			CreateEncoderFactory(nullCodec),
			/*
			std::unique_ptr<webrtc::VideoEncoderFactory>(
				new webrtc::MultiplexEncoderFactory(
#ifdef WEBRTC_AARCH
					std::make_unique<webrtc::InternalEncoderFactory>()
#else
					std::make_unique<webrtc::NvVideoEncoderFactory>()
#endif
				)),
				*/
			CreateDecoderFactory(nullCodec),
			/*
			std::unique_ptr<webrtc::VideoDecoderFactory>(
				new webrtc::MultiplexDecoderFactory(
					std::make_unique<webrtc::InternalDecoderFactory>())),*/
			nullptr, nullptr);

	}
	if (!g_peer_connection_factory.get()) {
		return false;
	}
	return true;
}
PeerConnectionHandle  mrsPeerConnectionCreate()  {
	// Ensure the factory exists


	// Setup the connection configuration
	webrtc::PeerConnectionInterface::RTCConfiguration config;
	config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
	config.enable_dtls_srtp = false;
	config.enable_rtp_data_channel = true;
	webrtc::PeerConnectionInterface::IceServer server;
	server.uri = "stun:stun1.l.google.com:19302";
	config.servers.push_back(server);
 	server.uri = "turn:58.34.98.12:3478";
 	server.username = "ubuntu12";
	server.password = "ubuntu12@123";
	config.servers.push_back(server);
	
	config.enable_dtls_srtp = false;  //< TODO - Should be true/unset for security

	// Create the new peer connection
	rtc::scoped_refptr<PeerConnection> peer =
		new rtc::RefCountedObject<PeerConnection>();
	/* webrtc::PeerConnectionDependencies dependencies(peer);*/
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection =
		g_peer_connection_factory->CreatePeerConnection(config, nullptr, nullptr,
			peer.get());
	if (peer_connection.get() == nullptr)
		return {};
	peer->SetPeerImpl(peer_connection);
	const PeerConnectionHandle handle{ peer.get() };
	g_peer_connection_map.insert({ handle, std::move(peer) });
	return handle;
}
void mrsPeerConnectionRegisterConnectedCallback(
	PeerConnectionHandle peerHandle,
	PeerConnectionConnectedCallback callback,
	void* user_data)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		peer->RegisterConnectedCallback(Callback<>{callback, user_data});
	}
}
void mrsPeerConnectionRegisterLocalSdpReadytoSendCallback(
	PeerConnectionHandle peerHandle,
	int32_t pid,
	int32_t index,
	PeerConnectionLocalSdpReadytoSendCallback callback,
	void* user_data)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		peer->RegisterLocalSdpReadytoSendCallback(
			Callback<int32_t,int32_t,const char*, const char*>{callback,user_data,pid,index});
	}
}
void mrsPeerConnectionRegisterIceCandidateReadytoSendCallback(
	PeerConnectionHandle peerHandle,
	int32_t pid,
	int32_t view,
	PeerConnectionIceCandidateReadytoSendCallback callback,
	void* user_data)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		peer->RegisterIceCandidateReadytoSendCallback(
			Callback<int32_t,int32_t,const char*, int, const char*>{callback, user_data,pid,view});
	}
}
void mrsPeerConnectionRegisterARGBLocalVideoFrameCallback(
	PeerConnectionHandle peerHandle,
	 
	PeerConnectionARGBVideoFrameCallback callback,
	void* user_data)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		peer->RegisterLocalVideoFrameCallback(
			ARGBFrameReadyCallback{callback, user_data });
	}
}
void mrsPeerConnectionRegisterARGBRemoteVideoFrameCallback(
	PeerConnectionHandle peerHandle,
	PeerConnectionARGBVideoFrameCallback callback,
	void* user_data)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		peer->RegisterRemoteVideoFrameCallback(
			ARGBFrameReadyCallback{callback, user_data });
	}
}
void  mrsPeerConnectionRegisterChannelCallback(PeerConnectionHandle peerHandle,
	PeerConnectionDataChannelMessageCallback message_callback, void* message_user_data,
	PeerConnectionDataChannelBufferingCallback buffering_callback, void* buffering_user_data,
	PeerConnectionDataChannelStateCallback state_callback,
	void* state_user_data)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle))
	{
		peer->RegisterDataChannelCallback(
			DataChannelMessageCallback{ message_callback, message_user_data },
			DataChannelBufferingCallback{ buffering_callback, buffering_user_data },
			DataChannelStateCallback{ state_callback, state_user_data });
	}
}



bool  mrsPeerConnectionAddIceCandidate(PeerConnectionHandle peerHandle,
	const char* sdp,
	const int sdp_mline_index,
	const char* sdp_mid)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		return peer->AddIceCandidate(sdp_mid, sdp_mline_index, sdp);
	}
	return false;
}
bool mrsPeerConnectionCreateOffer(PeerConnectionHandle peerHandle)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		return peer->CreateOffer();
	}
	return false;
}
bool mrsPeerConnectionCreateAnswer(PeerConnectionHandle peerHandle)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		return peer->CreateAnswer();
	}
	return false;
}
bool mrsPeerConnectionSetRemoteDescription(PeerConnectionHandle peerHandle,
	const char* type,
	const char* sdp)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		return peer->SetRemoteDescription(type, sdp);
	}
	return false;
}
void mrsPeerConnectionClose(PeerConnectionHandle* peerHandlePtr)  {
        	if (peerHandlePtr) {
		if (auto peer = static_cast<PeerConnection*>(*peerHandlePtr)) {
			auto it = g_peer_connection_map.find(peer);
			if (it != g_peer_connection_map.end()) {
				// This generally removes the last reference to the PeerConnection and
				// leads to its destruction, unless some background running task is
				// still using the connection.
				g_peer_connection_map.erase(it);
				if (g_peer_connection_map.empty()) {
					// Release the factory so that the threads are stopped and the DLL can
					// be unloaded. This is mandatory to be able to unload/reload in the
					// Unity Editor and be able to Play/Stop multiple times per Editor
					// process run.
					g_peer_connection_factory = nullptr;

					g_signaling_thread.reset();
					g_worker_thread.reset();

				}
			}
		}
		*peerHandlePtr = nullptr;
	}
}
 
#ifdef WEBRTC_LINUX
 bool mrsPeerConnectionAddLocalVideoTrack( PeerConnectionHandle peerHandle,
        RenderPosition type,
        int32_t index)
	{
		if(auto peer=static_cast<PeerConnection *>(peerHandle))
		{
			if (!g_peer_connection_factory) 
			return false;
		 
            std::unique_ptr<CaptureOp > op=std::make_unique<CaptureOp>(type,index);
			auto ptr=op.get();
			peer->RegisterCaptureOp(op);
			rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source = OpenGSMLCapture(ptr);
			if(!video_source) return false;
				rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track =
				g_peer_connection_factory->CreateVideoTrack(kLocalVideoLabel+std::to_string(type),
					video_source);
			if (!video_track) {
				return false;
			}

			return peer->AddLocalVideoTrack(std::move(video_track), kLocalVideoLabel+std::to_string(type));
		}
		return false;
	}
bool mrsPeerConnectionSwitchCapture(
       PeerConnectionHandle peerHandle,
       bool front
    )
{
	if(auto peer=static_cast<PeerConnection *>(peerHandle))
		{
			peer->SwitchCapture(front);
			return true;
		}
	return false;
}
bool mrsPeerConnectionSetCtx(PeerConnectionHandle peerHandle,void * data)
{
	if(auto peer=static_cast<PeerConnection *>(peerHandle))
		{
			peer->SetOtherCtx(data);
			return true;
		}
	return false;
}
void * mrsPeerConnectionCurrentCtx(PeerConnectionHandle peerHandle)
{
	if(auto peer=static_cast<PeerConnection *>(peerHandle))
		{
			return peer->GetCurrentCtx();
		}
		return nullptr;
}
#endif
bool mrsPeerConnectionAddLocalAudioTrack(PeerConnectionHandle peerHandle)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		if (!g_peer_connection_factory)  
			return false;
		 
		rtc::scoped_refptr<webrtc::AudioSourceInterface> audio_source =
			g_peer_connection_factory->CreateAudioSource(cricket::AudioOptions());
		if (!audio_source) 
			return false;
	 
		rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track =
			g_peer_connection_factory->CreateAudioTrack(kLocalAudioLabel,
				audio_source);
		if (!audio_track) 
			return false;
		 
		return peer->AddLocalAudioTrack(std::move(audio_track));
	}
	return false;
}

mrsResult mrsPeerConnectionAddDataChannel(
	PeerConnectionHandle peerHandle,

	bool ordered,
	bool reliable//,
// 	PeerConnectionDataChannelMessageCallback message_callback,
// 	void* message_user_data,
// 	PeerConnectionDataChannelBufferingCallback buffering_callback,
// 	void* buffering_user_data,
// 	PeerConnectionDataChannelStateCallback state_callback,
// 	void* state_user_data
) 

{
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		return peer->AddDataChannel(kLocalDataChannel.c_str(), ordered, reliable);
		// 			DataChannelMessageCallback{ message_callback, message_user_data },
	// 			DataChannelBufferingCallback{ buffering_callback, buffering_user_data },
	// 			DataChannelStateCallback{ state_callback, state_user_data });
	}
	return MRS_E_INVALID_PEER_HANDLE;
}

void mrsPeerConnectionRemoveLocalVideoTrack(
	PeerConnectionHandle peerHandle)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		peer->RemoveLocalVideoTrack();
	}
}

void mrsPeerConnectionRemoveLocalAudioTrack(
	PeerConnectionHandle peerHandle)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		peer->RemoveLocalAudioTrack();
	}
}

bool mrsPeerConnectionRemoveDataChannel(PeerConnectionHandle peerHandle)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		return peer->RemoveDataChannel();
	}
	return false;
}



bool mrsPeerConnectionSendDataChannelMessage(PeerConnectionHandle peerHandle,

	const void* data,
	uint64_t size)  {
	if (auto peer = static_cast<PeerConnection*>(peerHandle)) {
		return peer->SendDataChannelMessage(data, size);
	}
	return false;
}

bool mrsSdpForceCodecs(const char* message,
	const char* audio_codec_name,
	const char* video_codec_name,
	char* buffer,
	size_t* buffer_size) {
	RTC_CHECK(message);
	RTC_CHECK(buffer);
	RTC_CHECK(buffer_size);
	std::string message_str(message);
	std::string audio_codec_name_str;
	std::string video_codec_name_str;
	if (audio_codec_name) {
		audio_codec_name_str.assign(audio_codec_name);
	}
	if (video_codec_name) {
		video_codec_name_str.assign(video_codec_name);
	}
	std::string out_message =
		SdpForceCodecs(message_str, audio_codec_name_str, video_codec_name_str);
	const size_t capacity = *buffer_size;
	const size_t size = out_message.size();
	*buffer_size = size + 1;
	if (capacity < size + 1) {
		return false;
	}
	memcpy(buffer, out_message.c_str(), size);
	buffer[size] = '\0';
	return true;
}

void mrsMemCpy(void* dst, const void* src, size_t size) {
	memcpy(dst, src, size);
}

void mrsMemCpyStride(void* dst,
	int dst_stride,
	const void* src,
	int src_stride,
	int elem_size,
	int elem_count) {
	RTC_CHECK(dst);
	RTC_CHECK(dst_stride >= elem_size);
	RTC_CHECK(src);
	RTC_CHECK(src_stride >= elem_size);
	if ((dst_stride == elem_size) && (src_stride == elem_size)) {
		// If tightly packed, do a single memcpy() for performance
		const size_t total_size = (size_t)elem_size * elem_count;
		memcpy(dst, src, total_size);
	}
	else {
		// Otherwise, copy row by row
		for (int i = 0; i < elem_count; ++i) {
			memcpy(dst, src, elem_size);
			dst = (char*)dst + dst_stride;
			src = (const char*)src + src_stride;
		}
	}
}
