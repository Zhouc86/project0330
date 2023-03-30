#pragma once
#include "callback.h"
using AudioFrameReadyCallback =
Callback<const void*, const int, const int, const int, const int>;

/// Audio frame observer to get notified of newly available audio frames.
class AudioFrameObserver : public webrtc::AudioTrackSinkInterface {
public:
	void SetCallback(AudioFrameReadyCallback callback) ;

protected:
	// AudioTrackSinkInterface interface
	void OnData(const void* audio_data,
		int bits_per_sample,
		int sample_rate,
		size_t number_of_channels,
		size_t number_of_frames)  override;

private:
	AudioFrameReadyCallback callback_;

	std::mutex mutex_;
};