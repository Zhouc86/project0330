#pragma once
#include "callback.h"
enum class DataChannelState : std::int32_t {
	kConnecting = 0,
	kOpen = 1,
	kClosing = 2,
	kClosed = 3
};
using DataChannelMessageCallback = Callback<const void*, const int32_t>;
using DataChannelBufferingCallback =
Callback<const uint64_t, const uint64_t, const uint64_t>;

/// Callback fired when the data channel state changed.
using DataChannelStateCallback = Callback</*DataChannelState*/ int, int>;

class DataChannelObserver : public webrtc::DataChannelObserver {
public:
	DataChannelObserver(
		rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) ;
	webrtc::DataChannelInterface* data_channel() const {
		return data_channel_.get();
	}
	void SetMessageCallback(DataChannelMessageCallback callback)  {
		std::lock_guard<std::mutex> lock(mutex_);
		message_callback_ = callback;
	}
	void SetBufferingCallback(
		DataChannelBufferingCallback callback)  {
		std::lock_guard<std::mutex> lock(mutex_);
		buffering_callback_ = callback;
	}
	void SetStateCallback(DataChannelStateCallback callback)  {
		std::lock_guard<std::mutex> lock(mutex_);
		state_callback_ = callback;
	}

protected:
	// DataChannelObserver interface

	// The data channel state have changed.
	void OnStateChange()  override;
	//  A data buffer was successfully received.
	void OnMessage(const webrtc::DataBuffer& buffer)  override;
	// The data channel's buffered_amount has changed.
	void OnBufferedAmountChange(uint64_t previous_amount)  override;

private:
	rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel_;
	DataChannelMessageCallback message_callback_;
	DataChannelBufferingCallback buffering_callback_;
	DataChannelStateCallback state_callback_;
	std::mutex mutex_;
};
