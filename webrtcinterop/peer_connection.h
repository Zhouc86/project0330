#pragma once
class CaptureOp;
class PeerConnection : public webrtc::PeerConnectionObserver,
                       public webrtc::CreateSessionDescriptionObserver {
 public:
  PeerConnection();
  ~PeerConnection()  override;
  void SetPeerImpl(
      rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer) ;

  using ConnectedCallback = Callback<>;
  void RegisterConnectedCallback(ConnectedCallback&& callback)  {
    std::lock_guard<std::mutex> lock{connected_callback_mutex_};
    connected_callback_ = std::move(callback);
  }

  using LocalSdpReadytoSendCallback = Callback<int32_t,int32_t,const char*, const char*>;
  void RegisterLocalSdpReadytoSendCallback(
      LocalSdpReadytoSendCallback&& callback)  {
    std::lock_guard<std::mutex> lock{local_sdp_ready_to_send_callback_mutex_};
    local_sdp_ready_to_send_callback_ = std::move(callback);
  }

  using IceCandidateReadytoSendCallback =
      Callback<int32_t,int32_t,const char*, int, const char*>;
  void RegisterIceCandidateReadytoSendCallback(
      IceCandidateReadytoSendCallback&& callback)  {
    std::lock_guard<std::mutex> lock{ice_candidate_ready_to_send_callback_mutex_};
    ice_candidate_ready_to_send_callback_ = std::move(callback);
  }

  using RenegotiationNeededCallback = Callback<>;
  void RegisterRenegotiationNeededCallback(
      RenegotiationNeededCallback&& callback)  {
    std::lock_guard<std::mutex> lock{renegotiation_needed_callback_mutex_};
    renegotiation_needed_callback_ = std::move(callback);
  }

  using TrackAddedCallback = Callback<>;
  void RegisterTrackAddedCallback(TrackAddedCallback&& callback)  {
    std::lock_guard<std::mutex> lock{track_added_callback_mutex_};
    track_added_callback_ = std::move(callback);
  }

  using TrackRemovedCallback = Callback<>;
  void RegisterTrackRemovedCallback(TrackRemovedCallback&& callback)  {
    std::lock_guard<std::mutex> lock{track_removed_callback_mutex_};
    track_removed_callback_ = std::move(callback);
  }

  void RegisterLocalVideoFrameCallback(
      I420FrameReadyCallback callback)  {
    if (local_video_observer_) {
      local_video_observer_->SetCallback(std::move(callback));
    }
  }

  void RegisterLocalVideoFrameCallback(
      ARGBFrameReadyCallback callback)  {
    if (local_video_observer_) {
      local_video_observer_->SetCallback(std::move(callback));
    }
  }

  void RegisterRemoteVideoFrameCallback(
      I420FrameReadyCallback callback)  {
    if (remote_video_observer_) {
      remote_video_observer_->SetCallback(std::move(callback));
    }
  }

  void RegisterRemoteVideoFrameCallback(
      ARGBFrameReadyCallback callback)  {
    if (remote_video_observer_) {
      remote_video_observer_->SetCallback(std::move(callback));
    }
  }
  void RegisterDataChannelCallback(
      DataChannelMessageCallback message_callback,
      DataChannelBufferingCallback buffering_callback,
      DataChannelStateCallback state_callback);
  bool AddLocalVideoTrack(
      rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track,const std::string& stream) ;
  void RemoveLocalVideoTrack() ;
  bool AddLocalAudioTrack(
      rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track) ;
  void RemoveLocalAudioTrack() ;

  mrsResult AddDataChannel(
                           const char* label,
                           bool ordered,
                           bool reliable//,
//                            DataChannelMessageCallback message_callback,
//                            DataChannelBufferingCallback buffering_callback,
//                            DataChannelStateCallback state_callba                        
						   ) ;
  bool RemoveDataChannel() ;
/*  bool RemoveDataChannel(const char* label) ;*/

  /// Remove all the existing data tracks from the peer connection.
/*  void RemoveAllDataTracks() ;*/

  bool SendDataChannelMessage(const void* data, uint64_t size) ;

  bool AddIceCandidate(const char* sdp_mid,
                       const int sdp_mline_index,
                       const char* candidate) ;
  bool CreateOffer() ;
  bool CreateAnswer() ;
  bool SetRemoteDescription(const char* type, const char* sdp) ;
#ifdef WEBRTC_LINUX
  void RegisterCaptureOp(std::unique_ptr<CaptureOp>& op);
  void SwitchCapture(bool front);
  void SetOtherCtx(void * data);
  void* GetCurrentCtx();

#endif // WEBRTC_LINUX


 protected:
  // PeerConnectionObserver interface

  // Triggered when the SignalingState changed.
  void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState
                             new_state)  override;

  // Triggered when media is received on a new stream from remote peer.
  void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>
                       stream)  override;

  // Triggered when a remote peer closes a stream.
  void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>
                          stream)  override;

  // Triggered when a remote peer opens a data channel.
  void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)  override;

  // Triggered when renegotiation is needed. For example, an ICE restart
  // has begun.
  void OnRenegotiationNeeded()  override;

  // Called any time the IceConnectionState changes.
  //
  // Note that our ICE states lag behind the standard slightly. The most
  // notable differences include the fact that "failed" occurs after 15
  // seconds, not 30, and this actually represents a combination ICE + DTLS
  // state, so it may be "failed" if DTLS fails while ICE succeeds.
  void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState
                             /*new_state*/)  override {}

  // Called any time the IceGatheringState changes.
  void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState
                            /*new_state*/)  override {}

  // A new ICE candidate has been gathered.
  void OnIceCandidate(
      const webrtc::IceCandidateInterface* candidate)  override;

  // Callback on track added.
  void OnAddTrack(
      rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
      const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
          streams)  override;

  // Callback on track removed.
  void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface>
                         receiver)  override;

 protected:
  // CreateSessionDescriptionObserver interface

  // This callback transfers the ownership of the |desc|.
  // TODO(deadbeef): Make this take an std::unique_ptr<> to avoid confusion
  // around ownership.
  void OnSuccess(webrtc::SessionDescriptionInterface* desc)  override;

  // The OnFailure callback takes an RTCError, which consists of an
  // error code and a string.
  // RTCError is non-copyable, so it must be passed using std::move.
  // Earlier versions of the API used a string argument. This version
  // is deprecated; in order to let clients remove the old version, it has a
  // default implementation. If both versions are unimplemented, the
  // result will be a runtime error (stack overflow). This is intentional.
  void OnFailure(webrtc::RTCError error)  override {}

 protected:
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_;
  ConnectedCallback connected_callback_;
  LocalSdpReadytoSendCallback local_sdp_ready_to_send_callback_;
  IceCandidateReadytoSendCallback ice_candidate_ready_to_send_callback_;
  RenegotiationNeededCallback renegotiation_needed_callback_;
  TrackAddedCallback track_added_callback_;
  TrackRemovedCallback track_removed_callback_;
  DataChannelMessageCallback data_channel_message_callback_;
  DataChannelBufferingCallback data_channel_buffering_callback_;
  DataChannelStateCallback data_channel_state_callback_;
  std::mutex connected_callback_mutex_;
  std::mutex local_sdp_ready_to_send_callback_mutex_;
  std::mutex ice_candidate_ready_to_send_callback_mutex_;
  std::mutex renegotiation_needed_callback_mutex_;
  std::mutex track_added_callback_mutex_;
  std::mutex track_removed_callback_mutex_;
  rtc::scoped_refptr<webrtc::VideoTrackInterface> local_video_track_;
  rtc::scoped_refptr<webrtc::AudioTrackInterface> local_audio_track_;
  rtc::scoped_refptr<webrtc::RtpSenderInterface> local_video_sender_;
  rtc::scoped_refptr<webrtc::RtpSenderInterface> local_audio_sender_;
  std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>> remote_streams_;
#ifdef WEBRTC_LINUX
  std::unique_ptr<CaptureOp> _capture;
#endif
  /// Collection of data channels from their unique ID.
  /// This contains only data channels pre-negotiated or opened by the remote
  /// peer, as data channels opened locally won't have immediately a unique ID.
//   std::unordered_map<int, std::shared_ptr<DataChannelObserver>>
//       data_channel_from_id_;
// 
//   /// Collection of data channels from their label.
//   /// This contains only data channels with a non-empty label.
//   std::unordered_multimap<std::string, std::shared_ptr<DataChannelObserver>>
//       data_channel_from_label_;
  std::unique_ptr<DataChannelObserver> channel_ob_server;
  //< TODO - Clarify lifetime of those, for now same as this PeerConnection
  std::unique_ptr<AudioFrameObserver> local_audio_observer_;
  std::unique_ptr<AudioFrameObserver> remote_audio_observer_;
  std::unique_ptr<VideoFrameObserver> local_video_observer_;
  std::unique_ptr<VideoFrameObserver> remote_video_observer_;

  /// Flag to indicate if SCTP was negotiated during the initial SDP handshake
  /// (m=application), which allows subsequently to use data channels. If this
  /// is false then data channels will never connnect. This is set to true if a
  /// data channel is created before the connection is established, which will
  /// force the connection to negotiate the necessary SCTP information. See
  /// https://stackoverflow.com/questions/43788872/how-are-data-channels-negotiated-between-two-peers-with-webrtc
  bool sctp_negotiated_ = true;

 private:
  PeerConnection(const PeerConnection&) = delete;
  PeerConnection& operator=(const PeerConnection&) = delete;
};

