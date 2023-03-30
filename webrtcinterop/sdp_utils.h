#pragma once
#include <mutex>

#include "callback.h"

std::string SdpForceCodecs(const std::string& message,
	const std::string& audio_codec_name,
	const std::string& video_codec_name);