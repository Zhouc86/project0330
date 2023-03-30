#pragma once
#include <cstdint>
#include <string>

//#ifdef WIN32
//#include "comm.h"
//#define MRS_CALL 
// #if defined(WEBRTC_WIN)
// #define  __declspec(dllexport)
// #define MRS_CALL __stdcall
// #else
// #define  __declspec(dllimport)
// #define MRS_CALL __stdcall
// #endif
// #else
// #endif
/*
enum class VideoDesc :std::int32_t {
	OK,
	Busy,
	Reject,
	NoFound,
	IsVideoing,

};
 
 
enum class CaptureType 
{
     USB,
     RealSense,
     Zed2,
     RTSP,
};
*/

using PeerConnectionHandle = void*;
 
//class INativeNotify;
extern "C" {

    //
    // Errors
    //

    using mrsResult = std::uint32_t;

    constexpr const mrsResult MRS_SUCCESS{ 0 };

    // Unknown generic error
    constexpr const mrsResult MRS_E_UNKNOWN{ 0x80000000 };

    // Peer conection (0x0xx)
    constexpr const mrsResult MRS_E_INVALID_PEER_HANDLE{ 0x80000001 };
    constexpr const mrsResult MRS_E_PEER_NOT_INITIALIZED{ 0x80000002 };

    // Data (0x3xx)
    constexpr const mrsResult MRS_E_SCTP_NOT_NEGOTIATED{ 0x80000301 };
    constexpr const mrsResult MRS_E_INVALID_DATA_CHANNEL_ID{ 0x80000302 };
    constexpr const mrsResult MRS_E_INVALID_PARAMETER{ 0x8000303 };

    //
    // Generic utilities
    //

    /// Opaque enumerator type.
    struct mrsEnumerator;

    /// Handle to an enumerator.
    /// This must be freed after use with |mrsCloseEnum|.
    using mrsEnumHandle = mrsEnumerator*;

    /// Close an enumerator previously obtained from one of the EnumXxx() calls.
     void  mrsCloseEnum(mrsEnumHandle* handleRef) ;

    //
    // Video capture enumeration
    //

    /// Callback invoked for each enumerated video capture device.
    using mrsVideoCaptureDeviceEnumCallback = void(*)(const char* id,
        const char* name,
        void* user_data);

    /// Callback invoked on video capture device enumeration completed.
    using mrsVideoCaptureDeviceEnumCompletedCallback =
        void(*)(void* user_data);

    /// Enumerate the video capture devices asynchronously.
    /// For each device found, invoke the mandatory |callback|.
    /// At the end of the enumeration, invoke the optional |completedCallback| if it
    /// was provided (non-null).
      void  mrsEnumVideoCaptureDevicesAsync(
        mrsVideoCaptureDeviceEnumCallback callback,
        void* callbackUserData,
        mrsVideoCaptureDeviceEnumCompletedCallback completedCallback,
        void* completedCallbackUserData) ;

    enum class mrsVideoProfileKind : int32_t {
        kUnspecified,
        kVideoRecording,
        kHighQualityPhoto,
        kBalancedVideoAndPhoto,
        kVideoConferencing,
        kPhotoSequence,
        kHighFrameRate,
        kVariablePhotoSequence,
        kHdrWithWcgVideo,
        kHdrWithWcgPhoto,
        kVideoHdr8,
    };
    /// Video profile info.
    struct mrsVideoProfileInfo {
        // Unique identifier of the video profile.
        const char* id;
    };
    using mrsVideoProfileEnumCallback =
        void(*)(void* user_data, const mrsVideoProfileInfo* profile_info);

    using mrsVideoProfileEnumCompletedCallback = void(*)(void* user_data);

     void  mrsEnumVideoProfilesAsync(
        const char* device_id,
        mrsVideoProfileKind profile_kind,
        mrsVideoProfileEnumCallback enumCallback,
        void* enumCallbackUserData,
        mrsVideoProfileEnumCompletedCallback completedCallback,
        void* completedCallbackUserData) ;

    /// Video capture format info.
    struct mrsVideoCaptureFormatInfo {
        // Capture width, in pixels.
        uint32_t width;

        // Capture height, in pixels.
        uint32_t height;

        // Maximum capture framerate, in frames per seconds. Video capture devices
        // commonly have adaptive framerate based on luminosity, and this value
        // reports the maximum framerate the device supports.
        float framerate;

        // Capture format as a FOURCC code.
        // https://docs.microsoft.com/en-us/windows/win32/medfound/video-fourccs
        uint32_t fourcc;
    };
    /// Callback invoked for each enumerated video capture format.
    using mrsVideoCaptureFormatEnumCallback = void(
        *)(void* user_data, const mrsVideoCaptureFormatInfo* format_info);

    /// Callback invoked on video capture format enumeration completed. If the
    /// result is not |mrsResult::kSuccess| then some or all of the device formats
    /// might not have been enumerated.
    using mrsVideoCaptureFormatEnumCompletedCallback =
        void(*)(void* user_data, mrsResult result);

      void  mrsEnumVideoCaptureFormatsAsync(
        const char* device_id,
        const char* profile_id,
        mrsVideoProfileKind profile_kind,
        mrsVideoCaptureFormatEnumCallback enumCallback,
        void* enumCallbackUserData,
        mrsVideoCaptureFormatEnumCompletedCallback completedCallback,
        void* completedCallbackUserData) ;

    //
    // Peer connection
    //

    /// Opaque handle to a PeerConnection object.

    /// Callback fired when the peer connection is connected, that is it finished
    /// the JSEP offer/answer exchange successfully.
    using PeerConnectionConnectedCallback = void(*)(void* user_data);

    /// Callback fired when a local SDP message has been prepared and is ready to be
    /// sent by the user via the signaling service.
    using PeerConnectionLocalSdpReadytoSendCallback =
        void(*)(void* user_data, int32_t peer,int32_t index, const char* type,  const char* sdp_data);

    /// Callback fired when an ICE candidate has been prepared and is ready to be
    /// sent by the user via the signaling service.
    using PeerConnectionIceCandidateReadytoSendCallback =
        void(*)(void* user_data,
            int32_t pid,
            int32_t index,
            const char* candidate,
            int sdpMlineindex,
            const char* sdpMid);

    /// Callback fired when a renegotiation of the current session needs to occur to
    /// account for new parameters (e.g. added or removed tracks).
    using PeerConnectionRenegotiationNeededCallback =
        void(*)(void* user_data);

    /// Callback fired when a remote track is added to a connection.
    using PeerConnectionTrackAddedCallback = void(*)(void* user_data);

    /// Callback fired when a remote track is removed from a connection.
    using PeerConnectionTrackRemovedCallback = void(*)(void* user_data);

    /// Callback fired when a local or remote (depending on use) video frame is
    /// available to be consumed by the caller, usually for display.
    using PeerConnectionI420VideoFrameCallback =
        void(*)(void* user_data,
            const void* yptr,
            const void* uptr,
            const void* vptr,
            const void* aptr,
            const int ystride,
            const int ustride,
            const int vstride,
            const int astride,
            const int frame_width,  //< TODO : uint?
            const int frame_height);

    using PeerConnectionARGBVideoFrameCallback =
        void(*)(void* user_data,
           
            const uint8_t* yptr, int strideY, 
            const uint8_t* uptr, int strideU,
            const uint8_t* vptr, int strideV,
			const int stride, const int frame_width,
            const int frame_height);

    using PeerConnectionDataChannelMessageCallback =
        void(*)(void* user_data, const void* data, const int32_t size);

    using PeerConnectionDataChannelBufferingCallback =
        void(*)(void* user_data,
            const uint64_t previous,
            const uint64_t current,
            const uint64_t limit);

    using PeerConnectionDataChannelStateCallback = void(*)(void* user_data,
        int state,
        int id);

 
 
 
    struct IceServer {
        const char* url = nullptr;
        const char* username = nullptr;
        const char* password = nullptr;
    };
     bool mrsWebrtcCreateFactory(bool NullCodec);
    /// Create a peer connection and return a handle to it.
    /// On UWP this must be invoked from another thread than the main UI thread.
      PeerConnectionHandle
        mrsPeerConnectionCreate() ;

    /// Register a callback fired once connected to a remote peer.
    /// To unregister, simply pass nullptr as the callback pointer.
      void  mrsPeerConnectionRegisterConnectedCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionConnectedCallback callback,
        void* user_data) ;

    /// Register a callback fired when a local message is ready to be sent via the
    /// signaling service to a remote peer.
      void  mrsPeerConnectionRegisterLocalSdpReadytoSendCallback(
        PeerConnectionHandle peerHandle,
        int32_t peer,
        int32_t index,
        PeerConnectionLocalSdpReadytoSendCallback callback,
        void* user_data) ;

    /// Register a callback fired when an ICE candidate message is ready to be sent
    /// via the signaling service to a remote peer.
     void  mrsPeerConnectionRegisterIceCandidateReadytoSendCallback(
        PeerConnectionHandle peerHandle,
         int32_t peer,
        int32_t index,
        PeerConnectionIceCandidateReadytoSendCallback callback,
        void* user_data) ;

    /// Register a callback fired when a renegotiation of the current session needs
    /// to occur to account for new parameters (e.g. added or removed tracks).
     void  mrsPeerConnectionRegisterRenegotiationNeededCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionRenegotiationNeededCallback callback,
        void* user_data) ;

    /// Register a callback fired when a remote track is added to the current peer
    /// connection.
     void  mrsPeerConnectionRegisterTrackAddedCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionTrackAddedCallback callback,
        void* user_data) ;

    /// Register a callback fired when a remote track is removed from the current
    /// peer connection.
     void  mrsPeerConnectionRegisterTrackRemovedCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionTrackRemovedCallback callback,
        void* user_data) ;

    /// Register a callback fired when a video frame is available from a local video
    /// track, usually from a local video capture device (local webcam).
      void  mrsPeerConnectionRegisterI420LocalVideoFrameCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionI420VideoFrameCallback callback,
        void* user_data) ;

    /// Register a callback fired when a video frame is available from a local video
    /// track, usually from a local video capture device (local webcam).
     void  mrsPeerConnectionRegisterARGBLocalVideoFrameCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionARGBVideoFrameCallback callback,
        void* user_data) ;

    /// Register a callback fired when a video frame from a video track was received
    /// from the remote peer.
     void  mrsPeerConnectionRegisterI420RemoteVideoFrameCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionI420VideoFrameCallback callback,
        void* user_data) ;

    /// Register a callback fired when a video frame from a video track was received
    /// from the remote peer.
      void  mrsPeerConnectionRegisterARGBRemoteVideoFrameCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionARGBVideoFrameCallback callback,
        void* user_data) ;

    /// Add a local video track from a local video capture device (webcam) to
    /// the collection of tracks to send to the remote peer.
    /// |video_device_id| specifies the unique identifier of a video capture
    /// device to open, as obtained by enumerating devices with
    /// mrsEnumVideoCaptureDevicesAsync(), or null for any device.
    /// |enable_mrc| allows enabling Mixed Reality Capture on HoloLens devices, and
    /// is otherwise ignored for other video capture devices. On UWP this must be
    /// invoked from another thread than the main UI thread.
    /* bool  mrsPeerConnectionAddLocalVideoTrack(
        PeerConnectionHandle peerHandle,
        CaptureType type,
        DisplayResolution res,
        int32_t fps) ;  */
#ifdef WEBRTC_LINUX
    
  //   bool mrsPeerConnectionAddLocalRtspTrack(PeerConnectionHandle peerHandle,const std::string& label,const std::string& videourl,
  //      int32_t index, int32_t width, int32_t height);
    bool  mrsPeerConnectionAddLocalVideoTrack(
        PeerConnectionHandle peerHandle,
        RenderPosition type,
        int32_t index);
    bool mrsPeerConnectionSwitchCapture(
       PeerConnectionHandle peerHandle,
       bool front
    );
    bool mrsPeerConnectionSetCtx(PeerConnectionHandle peerHandle,void * data);
    void * mrsPeerConnectionCurrentCtx(PeerConnectionHandle peerHandle);
#endif
    /// Add a local audio track from a local audio capture device (microphone) to
    /// the collection of tracks to send to the remote peer.
     bool  mrsPeerConnectionAddLocalAudioTrack(PeerConnectionHandle peerHandle) ;
     void  mrsPeerConnectionRegisterChannelCallback(
        PeerConnectionHandle peerHandle,
        PeerConnectionDataChannelMessageCallback message_callback,
        void* message_user_data,
        PeerConnectionDataChannelBufferingCallback buffering_callback,
        void* buffering_user_data,
        PeerConnectionDataChannelStateCallback state_callback,
        void* state_user_data) ;
    /// Add a new data channel.
/// This function has two distinct uses:
/// - If id < 0, then it adds a new in-band data channel with an ID that will be
/// selected by the WebRTC implementation itself, and will be available later.
/// In that case the channel is announced to the remote peer for it to create a
/// channel with the same ID.
/// - If id >= 0, then it adds a new out-of-band negotiated channel with the
/// given ID, and it is the responsibility of the app to create a channel with
/// the same ID on the remote peer to be able to use the channel.

     mrsResult  mrsPeerConnectionAddDataChannel(
        PeerConnectionHandle peerHandle,
        // int id,             // -1 for auto, >=0 for negotiated

        bool ordered,
        bool reliable) ;

     void  mrsPeerConnectionRemoveLocalVideoTrack(
        PeerConnectionHandle peerHandle) ;

     void  mrsPeerConnectionRemoveLocalAudioTrack(
        PeerConnectionHandle peerHandle) ;

      bool
        mrsPeerConnectionRemoveDataChannel(PeerConnectionHandle peerHandle) ;



      bool
        mrsPeerConnectionSendDataChannelMessage(PeerConnectionHandle peerHandle,

            const void* data,
            uint64_t size) ;

    /// Add a new ICE candidate received from a signaling service.
      bool
        mrsPeerConnectionAddIceCandidate(PeerConnectionHandle peerHandle,
            const char* sdp,
            const int sdp_mline_index,
            const char* sdp_mid) ;

    /// Create a new JSEP offer to try to establish a connection with a remote peer.
    /// This will generate a local offer message, then fire the
    /// "LocalSdpReadytoSendCallback" callback, which should send this message via
    /// the signaling service to a remote peer.
      bool
        mrsPeerConnectionCreateOffer(PeerConnectionHandle peerHandle) ;

    /// Create a new JSEP answer to a received offer to try to establish a
    /// connection with a remote peer. This will generate a local answer message,
    /// then fire the "LocalSdpReadytoSendCallback" callback, which should send this
    /// message via the signaling service to a remote peer.
      bool
        mrsPeerConnectionCreateAnswer(PeerConnectionHandle peerHandle) ;

    /// Set a remote description received from a remote peer via the signaling
    /// service.
      bool
          mrsPeerConnectionSetRemoteDescription(PeerConnectionHandle peerHandle,
            const char* type,
            const char* sdp) ;

    /// Close a peer connection and free all resources associated with it.
      void
        mrsPeerConnectionClose(PeerConnectionHandle* peerHandle) ;

    //
    // SDP utilities
    //

    /// Force audio and video codecs when advertizing capabilities in an SDP offer.#
    ///
    /// This is a workaround for the lack of access to codec selection. Instead of
    /// selecting codecs in code, this can be used to intercept a generated SDP
    /// offer before it is sent to the remote peer, and modify it by removing the
    /// codecs the user does not want.
    ///
    /// Codec names are compared to the list of supported codecs in the input
    /// message string, and if found then other codecs are pruned out. If the codec
    /// name is not found, the codec is assumed to be unsupported, so codecs for
    /// that type are not modified.
    ///
    /// On return the SDP offer message string to be sent via the signaler is stored
    /// into the output buffer pointed to by |buffer|.
    ///
    /// Note that because this function always return a message shorter or equal to
    /// the input message, one way to ensure this function doesn't fail is to pass
    /// an output buffer as large as the input message.
    ///
    /// |message| SDP message string to deserialize.
    /// |audio_codec_name| Optional SDP name of the audio codec to
    /// force if supported, or nullptr or empty string to leave unmodified.
    /// |video_codec_name| Optional SDP name of the video codec to force if
    /// supported, or nullptr or empty string to leave unmodified.
    /// |buffer| Output buffer of capacity *|buffer_size|.
    /// |buffer_size| Pointer to the buffer capacity on input, modified on output
    /// with the actual size of the null-terminated string, including the null
    /// terminator, so the size of the used part of the buffer, in bytes.
    /// Returns true on success or false if the buffer is not large enough to
    /// contain the new SDP message.
      bool  mrsSdpForceCodecs(const char* message,
        const char* audio_codec_name,
        const char* video_codec_name,
        char* buffer,
        int* buffer_size);

    //
    // Generic utilities
    //

    /// Optimized helper to copy a contiguous block of memory.
    /// This is equivalent to the standard malloc() function.
      void  mrsMemCpy(void* dst, const void* src, int size);

    /// Optimized helper to copy a block of memory with source and destination
    /// stride.
      void  mrsMemCpyStride(void* dst,
        int dst_stride,
        const void* src,
        int src_stride,
        int elem_size,
        int elem_count);

//     using mrsWebsocketCallback = mrsResult(*)(void* user_data,
//         const char* desc);
// 
//      mrsResult mrsWebsocketCreate(INativeNotify* data,
//         mrsWebSocketHandle* handle);
// 
//      void mrsWebsocketConnect(mrsWebSocketHandle handle,
//         const char* name,
//         const char* password);
// 
//      void mrsWebsocketClose(mrsWebSocketHandle handle);
// 
//      void mrsWebsocketVideoReq(mrsWebSocketHandle handle, int32_t peer, int32_t width, int32_t height, int32_t fps);
//      void mrsWebsocketVideoRep(mrsWebSocketHandle handle,
//         int32_t peer,
//         VideoDesc desc,
//         int32_t width,
//         int32_t height,
//         int32_t fps);

    /* void mrsWebsocketCreateOffer(mrsWebSocketHandle handle, int32_t
     * peer);*/

//      void mrsWebSocketLocalSdpReadytoSend(mrsWebSocketHandle handle,
//         const char* type,
//         const char* sdp);
//      void mrsWebSocketIceCandidateReadytoSend(mrsWebSocketHandle handle,
//         const char* sdp,
//         const int32_t sdp_mline_index,
//         const char* sdp_mid);
// 	 void mrsWebSocketHangup(mrsWebSocketHandle handle, int32_t peer);
// 	 /*class RemoteVideoTrackWrapper;*/
// 	 void mrsWebSocketCancelReq(mrsWebSocketHandle handle, int32_t peer);
}
 




