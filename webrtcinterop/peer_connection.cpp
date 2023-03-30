#include "pch.h"
#include <memory>
#include "../common/comm.h"
#include "api.h"
#include "callback.h"
#include "data_channel_observer.h"
#include "video_frame_observer.h"
#include "audio_frame_observer.h"

#include "peer_connection.h"

#ifdef WEBRTC_LINUX
#include "capture_op.h"
#endif


struct SessionDescObserver : public webrtc::SetSessionDescriptionObserver {
 public:
  SessionDescObserver() = default;
  template <typename Closure>
  SessionDescObserver(Closure&& callback)
      : callback_(std::forward<Closure>(callback)) {}
  void OnSuccess() override {
    if (callback_)
      callback_();
  }
  void OnFailure(webrtc::RTCError error) override
  {

  }
  

 protected:
  std::function<void()> callback_;
  ~SessionDescObserver() override = default;
};
struct SetRemoteSessionDescObserver
    : public webrtc::SetRemoteDescriptionObserverInterface {
 public:
  void OnSetRemoteDescriptionComplete(webrtc::RTCError error) override {}
};

const std::string kAudioVideoStreamId("local_av_stream");

void ensureNullTerminatedCString(std::string& str) {
  if (str.empty() || (str.back() != '\0')) {
    str.push_back('\0');
  }
}
PeerConnection::PeerConnection() = default;

PeerConnection::~PeerConnection()  {
  // Ensure that observers (sinks) are removed, otherwise the media pipelines
  // will continue to try to feed them with data after they're destroyed, or
  // try to notify of some incoming data on data tracks.
  RemoveLocalVideoTrack();
  RemoveLocalAudioTrack();
  for (auto stream : remote_streams_) {
    if (auto* sink = remote_video_observer_.get()) {
      for (auto&& video_track : stream->GetVideoTracks()) {
        video_track->RemoveSink(sink);
      }
    }
    if (auto* sink = remote_audio_observer_.get()) {
      for (auto&& audio_track : stream->GetAudioTracks()) {
        audio_track->RemoveSink(sink);
      }
    }
  }
   //RemoveAllDataTracks();
}
void PeerConnection::SetPeerImpl(
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer)  {
  peer_ = std::move(peer);
  local_video_observer_.reset(new VideoFrameObserver());
  remote_video_observer_.reset(new VideoFrameObserver());
  local_audio_observer_.reset(new AudioFrameObserver());
  remote_audio_observer_.reset(new AudioFrameObserver());
}
bool PeerConnection::AddLocalVideoTrack(
    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track,const std::string& stream)  {
  if (local_video_track_) {
    return false;
  }
  auto result = peer_->AddTrack(video_track, { stream });
  if (result.ok()) {
    if (local_video_observer_) {
      rtc::VideoSinkWants sink_settings{};
      sink_settings.rotation_applied = true;
      video_track->AddOrUpdateSink(local_video_observer_.get(), sink_settings);
    }
    local_video_sender_ = result.value();
    local_video_track_ = std::move(video_track);
    return true;
  }
  return false;
}
void PeerConnection::RemoveLocalVideoTrack()  {
  if (!local_video_track_)
    return;
  if (auto* sink = local_video_observer_.get()) {
    local_video_track_->RemoveSink(sink);
  }
  peer_->RemoveTrack(local_video_sender_);
  local_video_track_ = nullptr;
  local_video_sender_ = nullptr;
}
bool PeerConnection::AddLocalAudioTrack(
    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track)  {
  if (local_audio_track_) {
    return false;
  }
  auto result = peer_->AddTrack(audio_track, {kAudioVideoStreamId});
  if (result.ok()) {
    if (auto* sink = local_audio_observer_.get()) {
      audio_track->AddSink(sink);
    }
    local_audio_sender_ = result.value();
    local_audio_track_ = std::move(audio_track);
    return true;
  }
  return false;
}

void PeerConnection::RemoveLocalAudioTrack()  {
  if (!local_audio_track_)
    return;
  if (auto* sink = local_audio_observer_.get()) {
    local_audio_track_->RemoveSink(sink);
  }
  peer_->RemoveTrack(local_audio_sender_);
  local_audio_track_ = nullptr;
  local_audio_sender_ = nullptr;
}
void PeerConnection::RegisterDataChannelCallback(
	DataChannelMessageCallback message_callback,
	DataChannelBufferingCallback buffering_callback,
	DataChannelStateCallback state_callback) {

        data_channel_state_callback_ = state_callback;
        data_channel_buffering_callback_ = buffering_callback;
        data_channel_message_callback_ = message_callback;


}
mrsResult PeerConnection::AddDataChannel(
   
    const char* label,
    bool ordered,
    bool reliable//,
//     DataChannelMessageCallback message_callback,
//     DataChannelBufferingCallback buffering_callback,
//     DataChannelStateCallback state_callback
    )  {
  webrtc::DataChannelInit config{};
  config.ordered = ordered;
  config.reliable = reliable;
  config.id = -1;
 
  if (!sctp_negotiated_) {
    // Don't try to create a data channel without SCTP negotiation, it will get
    // stuck in the kConnecting state forever.
    return MRS_E_SCTP_NOT_NEGOTIATED;
  }
  std::string labelString = label;
  
  rtc::scoped_refptr<webrtc::DataChannelInterface> dataChannel =
      peer_->CreateDataChannel(labelString, &config);
  if (dataChannel) {
   DataChannelObserver* observer{
        new DataChannelObserver(dataChannel)};
    observer->SetMessageCallback(data_channel_message_callback_);
    observer->SetBufferingCallback(data_channel_buffering_callback_);
    observer->SetStateCallback(data_channel_state_callback_);
    dataChannel->RegisterObserver(observer);
	channel_ob_server.reset(std::move(observer));
//     if (!labelString.empty()) {
//       data_channel_from_label_.emplace(
//           std::make_pair(std::move(labelString), observer));
//     }
//     if (config.id >= 0) {
//       data_channel_from_id_.try_emplace(config.id, std::move(observer));
//     }
    return MRS_SUCCESS;
  }
  return MRS_E_UNKNOWN;
}

bool PeerConnection::RemoveDataChannel()  {
	auto* data_channel = channel_ob_server->data_channel();
	data_channel->UnregisterObserver();
	data_channel->Close();
  return true;
}

bool PeerConnection::SendDataChannelMessage(const void* data,
                                            uint64_t size)  {
	if (!channel_ob_server)
		return false;
	auto* data_channel = channel_ob_server->data_channel();
	if (data_channel->buffered_amount() + size > 0x1000000uLL) {
		return false;
	}
	rtc::CopyOnWriteBuffer bufferStorage((const char*)data, (size_t)size);
	webrtc::DataBuffer buffer(bufferStorage, false);  // always binary
	return data_channel->Send(buffer);
   
}

bool PeerConnection::AddIceCandidate(const char* sdp_mid,
                                     const int sdp_mline_index,
                                     const char* candidate)  {
  if (!peer_)
    return false;
  webrtc::SdpParseError error;
  std::unique_ptr<webrtc::IceCandidateInterface> ice_candidate(
      webrtc::CreateIceCandidate(sdp_mid, sdp_mline_index, candidate, &error));
  if (!ice_candidate)
    return false;
  if (!peer_->AddIceCandidate(ice_candidate.get()))
    return false;
  return true;
}

bool PeerConnection::CreateOffer()  {
  if (!peer_)
    return false;
  webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
  /*if (mandatory_receive_)*/ {  //< TODO - This is legacy, should use
                                 // transceivers
#ifdef WIN32
    options.offer_to_receive_audio = true;
    options.offer_to_receive_video = true;
#else
    options.offer_to_receive_audio = true;
    options.offer_to_receive_video = true;
#endif
  }
//   if (data_channel_from_id_.empty()) {
//     sctp_negotiated_ = false;
//   }
  peer_->CreateOffer(this, options);
  return true;
}

bool PeerConnection::CreateAnswer()  {
  if (!peer_)
    return false;
  webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;
  /*if (mandatory_receive_)*/ {  //< TODO - This is legacy, should use
                                 // transceivers
 
    options.offer_to_receive_audio = true;
    options.offer_to_receive_video = true;
 
  }
  peer_->CreateAnswer(this, options);
  return true;
}

bool PeerConnection::SetRemoteDescription(const char* type,
                                          const char* sdp)  {
  if (!peer_)
    return false;
//   if (data_channel_from_id_.empty()) {
//     sctp_negotiated_ = false;
//   }
  std::string sdp_type_str(type);
  auto sdp_type = webrtc::SdpTypeFromString(sdp_type_str);
  if (!sdp_type.has_value())
    return false;
  std::string remote_desc(sdp);
  webrtc::SdpParseError error;
  std::unique_ptr<webrtc::SessionDescriptionInterface> session_description(
      webrtc::CreateSessionDescription(sdp_type.value(), remote_desc, &error));
  if (!session_description)
    return false;
  rtc::scoped_refptr<webrtc::SetRemoteDescriptionObserverInterface> observer =
      new rtc::RefCountedObject<SetRemoteSessionDescObserver>();
  peer_->SetRemoteDescription(std::move(session_description),
                              std::move(observer));
  return true;
}

void PeerConnection::OnSignalingChange(
    webrtc::PeerConnectionInterface::SignalingState new_state)  {
  // See https://w3c.github.io/webrtc-pc/#rtcsignalingstate-enum
  switch (new_state) {
    case webrtc::PeerConnectionInterface::kStable:
      // Transitioning *to* stable means final answer received.
      // Otherwise the only possible way to be in the stable state is at start,
      // but this callback would not be invoked then because there's no
      // transition.
      {
        std::lock_guard<std::mutex> lock{connected_callback_mutex_};
        connected_callback_();
      }
      break;
    case webrtc::PeerConnectionInterface::kHaveLocalOffer:
      break;
    case webrtc::PeerConnectionInterface::kHaveLocalPrAnswer:
      break;
    case webrtc::PeerConnectionInterface::kHaveRemoteOffer:
      break;
    case webrtc::PeerConnectionInterface::kHaveRemotePrAnswer:
      break;
  }
}

void PeerConnection::OnAddStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)  {
  remote_streams_.push_back(stream);
  if (auto* sink = remote_video_observer_.get()) {
    rtc::VideoSinkWants sink_settings{};
    sink_settings.rotation_applied =
        true;  // no exposed API for caller to handle rotation
    for (auto&& video_track : stream->GetVideoTracks()) {
      video_track->AddOrUpdateSink(sink, sink_settings);
    }
  }
  if (auto* sink = remote_audio_observer_.get()) {
    for (auto&& audio_track : stream->GetAudioTracks()) {
      audio_track->AddSink(sink);
    }
  }
}

void PeerConnection::OnRemoveStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)  {
  auto it = std::find(remote_streams_.begin(), remote_streams_.end(), stream);
  if (it == remote_streams_.end())
    return;
  if (auto* sink = remote_video_observer_.get()) {
    for (auto&& video_track : stream->GetVideoTracks()) {
      video_track->RemoveSink(sink);
    }
  }
  if (auto* sink = remote_audio_observer_.get()) {
    for (auto&& audio_track : stream->GetAudioTracks()) {
      audio_track->RemoveSink(sink);
    }
  }
  remote_streams_.erase(it);
}

void PeerConnection::OnDataChannel(
    rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)
#if defined(WINUWP)
    (false)
#else
    
#endif
{
  // If receiving a new data channel, then obviously SCTP has been negotiated so
  // it is safe to create other ones.
  sctp_negotiated_ = true;

  std::string label = data_channel->label();
  DataChannelObserver* observer{
      new DataChannelObserver(data_channel)};
  //< TODO - Need to register a message callback!!
  observer->SetMessageCallback(data_channel_message_callback_);
  observer->SetBufferingCallback(data_channel_buffering_callback_);
  observer->SetStateCallback(data_channel_state_callback_);
  data_channel->RegisterObserver(observer);
  channel_ob_server.reset(observer);
 
}  // namespace webrtc_impl

void PeerConnection::OnRenegotiationNeeded()  {
  std::lock_guard<std::mutex> lock{renegotiation_needed_callback_mutex_};
  auto cb = renegotiation_needed_callback_;
  if (cb) {
    cb();
  }
}

void PeerConnection::OnIceCandidate(
    const webrtc::IceCandidateInterface* candidate)  {
  std::lock_guard<std::mutex> lock{ice_candidate_ready_to_send_callback_mutex_};
  auto cb = ice_candidate_ready_to_send_callback_;
  if (cb) {
    std::string sdp;
    if (!candidate->ToString(&sdp))
      return;
    ensureNullTerminatedCString(sdp);
    std::string sdp_mid = candidate->sdp_mid();
    ensureNullTerminatedCString(sdp_mid);
    cb(cb.peer,cb.index,sdp.c_str(), candidate->sdp_mline_index(), sdp_mid.c_str());
  }
}

void PeerConnection::OnAddTrack(
    rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
    const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
    /*streams*/)  {
    std::lock_guard<std::mutex> lock{track_added_callback_mutex_};
  auto cb = track_added_callback_;
  if (cb) {
    cb();
  }
}

void PeerConnection::OnRemoveTrack(
    rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver)  {
  std::lock_guard<std::mutex> lock{track_removed_callback_mutex_};
  auto cb = track_removed_callback_;
  if (cb) {
    cb();
  }
}

void PeerConnection::OnSuccess(
    webrtc::SessionDescriptionInterface* desc)  {
  std::lock_guard<std::mutex> lock{local_sdp_ready_to_send_callback_mutex_};
  auto cb = local_sdp_ready_to_send_callback_;
  rtc::scoped_refptr<webrtc::SetSessionDescriptionObserver> observer;
  if (cb) {
    std::string type{SdpTypeToString(desc->GetType())};
    ensureNullTerminatedCString(type);
    std::string sdp;
    desc->ToString(&sdp);
    ensureNullTerminatedCString(sdp);
    observer = new rtc::RefCountedObject<SessionDescObserver>(
        [cb, type = std::move(type), sdp = std::move(sdp)] {
          cb(cb.peer,cb.index,type.c_str(), sdp.c_str());
        });
  }
   else {
    observer = new rtc::RefCountedObject<SessionDescObserver>();
  }
  
  // SetLocalDescription will invoke observer.OnSuccess() once done, which
  // will in turn invoke the |local_sdp_ready_to_send_callback_| registered if
  // any, or do nothing otherwise. The observer is a mandatory parameter.
  peer_->SetLocalDescription(observer, desc); 
}
#ifdef WEBRTC_LINUX
void PeerConnection::RegisterCaptureOp(std::unique_ptr<CaptureOp>& ptr)
{
   _capture=std::move(ptr);
}
void PeerConnection::SwitchCapture(bool front)
{
   _capture->SetForward(front);
}
void * PeerConnection::GetCurrentCtx()
{
   return _capture->_ctx0;
}
void PeerConnection::SetOtherCtx(void * data)
{
   _capture->_ctx1=(context_t *) data;
}

#endif