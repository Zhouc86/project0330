#include "pch.h"
#include "../common/comm.h"
#include "data_channel_observer.h"
using RtcDataState = webrtc::DataChannelInterface::DataState;
using ApiDataState = DataChannelState;
inline ApiDataState apiStateFromRtcState(RtcDataState rtcState) {
	// API values have been chosen to match the current WebRTC values. If the
	// later change, this helper must be updated, as API values cannot change.
	assert((int)RtcDataState::kOpen == (int)ApiDataState::kOpen);
	assert((int)RtcDataState::kConnecting ==
		(int)ApiDataState::kConnecting);
	assert((int)RtcDataState::kClosing == (int)ApiDataState::kClosing);
	assert((int)RtcDataState::kClosed == (int)ApiDataState::kClosed);
	return (ApiDataState)rtcState;
}

DataChannelObserver::DataChannelObserver(
	rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) 
	: data_channel_(std::move(data_channel)) {}

void DataChannelObserver::OnStateChange()  {
	std::lock_guard<std::mutex> lock{ mutex_ };
	if (!state_callback_)
		return;
	auto apiState = apiStateFromRtcState(data_channel_->state());
	state_callback_((int)apiState, data_channel_->id());
}

void DataChannelObserver::OnMessage(const webrtc::DataBuffer& buffer)  {
	std::lock_guard<std::mutex> lock{ mutex_ };
	if (!message_callback_)
		return;
	message_callback_(buffer.data.data(), buffer.data.size());
}

void DataChannelObserver::OnBufferedAmountChange(
	uint64_t previous_amount)  {
	std::lock_guard<std::mutex> lock{ mutex_ };
	if (!buffering_callback_)
		return;
	uint64_t current_amount = data_channel_->buffered_amount();
	constexpr uint64_t max_capacity =
		0x1000000;  // 16MB, see DataChannelInterface
	buffering_callback_(previous_amount, current_amount, max_capacity);
}