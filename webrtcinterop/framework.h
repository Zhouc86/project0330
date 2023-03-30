#pragma once

         // Exclude rarely-used stuff from Windows headers
#include <cassert>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_set>
#if defined(MR_SHARING_WIN)

#include <SDKDDKVer.h>



#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <WinSock2.h>
#endif
 
 


// Core WebRTC
#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/data_channel_interface.h"
#include "api/peer_connection_interface.h"
#include "api/media_stream_interface.h"
#include "api/peer_connection_interface.h"
#include "api/rtp_sender_interface.h"
#include "api/stats/rtcstats_objects.h"
#include "api/transport/bitrate_settings.h"
#include "api/video/i420_buffer.h"
 
#include "media/base/adapted_video_track_source.h"
#include "media/engine/internal_decoder_factory.h"
#include "media/engine/internal_encoder_factory.h"
#include "media/engine/multiplex_codec_factory.h"
#include "media/engine/webrtc_video_engine.h"
// #include "media/engine/webrtcvideodecoderfactory.h"
// #include "media/engine/webrtcvideoencoderfactory.h"
#include "modules/audio_device/include/audio_device.h"
#include "modules/audio_mixer/audio_mixer_impl.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "modules/video_capture/video_capture_factory.h"
//#include "rtc_base/bind.h"
#include "rtc_base/memory/aligned_malloc.h"


// libyuv from WebRTC repository for color conversion
#include "libyuv.h"

 