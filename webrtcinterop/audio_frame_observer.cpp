#include "pch.h"
#include <mutex>
#include "../common/comm.h"
#include "audio_frame_observer.h"
void AudioFrameObserver::SetCallback(
	AudioFrameReadyCallback callback)  {
	std::lock_guard<std::mutex> lock(mutex_);
	callback_ = std::move(callback);
}

void AudioFrameObserver::OnData(const void* audio_data,
	int bits_per_sample,
	int sample_rate,
	size_t number_of_channels,
	size_t number_of_frames)  {

	if (!callback_) {
		return;
	}
	std::lock_guard<std::mutex> lock(mutex_);
	if (!callback_)
		return;

	callback_(audio_data, bits_per_sample, sample_rate,
		static_cast<int>(number_of_channels),
		static_cast<int>(number_of_frames));
}
