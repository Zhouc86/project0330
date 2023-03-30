#pragma once

#include <regex>

#include "VcmCapturer.h"

#include "rtspvideocapturer.h"
#include "rtspaudiocapturer.h"
 
#include "pc/video_track_source.h"

template<class T>
class TrackSource : public webrtc::VideoTrackSource {
public:
	static rtc::scoped_refptr<TrackSource> Create(const std::string & videourl,  std::unique_ptr<webrtc::VideoDecoderFactory>& videoDecoderFactory) {
		std::unique_ptr<T> capturer = absl::WrapUnique(T::Create(videourl,  videoDecoderFactory));
		if (!capturer) {
			return nullptr;
		}
		return new rtc::RefCountedObject<TrackSource>(std::move(capturer));
	}

protected:
	explicit TrackSource(std::unique_ptr<T> capturer)
		: webrtc::VideoTrackSource(/*remote=*/false), capturer_(std::move(capturer)) {}

private:
	rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override {
		return capturer_.get();
	}
	std::unique_ptr<T> capturer_;
};


class CapturerFactory {
	public:

	 

	static rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateVideoSource(const std::string & videourl, std::unique_ptr<webrtc::VideoDecoderFactory>& videoDecoderFactory) {
		rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> videoSource;
		

		videoSource = TrackSource<RTSPVideoCapturer>::Create(videourl, videoDecoderFactory);
	
		
	  
		return videoSource;
	}
    
    	static rtc::scoped_refptr<webrtc::AudioSourceInterface> CreateAudioSource(const std::string & audiourl, 
							const std::map<std::string,std::string> & opts, 
							
							rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory,
							rtc::scoped_refptr<webrtc::AudioDecoderFactory> audioDecoderfactory,
							rtc::scoped_refptr<webrtc::AudioDeviceModule>   audioDeviceModule) {
		rtc::scoped_refptr<webrtc::AudioSourceInterface> audioSource;

		 
	 
			audioDeviceModule->Terminate();
			audioSource = RTSPAudioSource::Create(audioDecoderfactory, audiourl);
 
		 
		return audioSource;
	}
};