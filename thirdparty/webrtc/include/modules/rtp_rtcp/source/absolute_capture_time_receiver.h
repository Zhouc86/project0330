/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_RTP_RTCP_SOURCE_ABSOLUTE_CAPTURE_TIME_RECEIVER_H_
#define MODULES_RTP_RTCP_SOURCE_ABSOLUTE_CAPTURE_TIME_RECEIVER_H_

#include "api/array_view.h"
#include "api/rtp_headers.h"
#include "api/units/time_delta.h"
#include "api/units/timestamp.h"
#include "rtc_base/synchronization/mutex.h"
#include "rtc_base/thread_annotations.h"
#include "system_wrappers/include/clock.h"

namespace webrtc {

//
// Helper class for receiving the |AbsoluteCaptureTime| header extension.
//
// Supports the "timestamp interpolation" optimization:
//   A receiver SHOULD memorize the capture system (i.e. CSRC/SSRC), capture
//   timestamp, and RTP timestamp of the most recently received abs-capture-time
//   packet on each received stream. It can then use that information, in
//   combination with RTP timestamps of packets without abs-capture-time, to
//   extrapolate missing capture timestamps.
//
// See: https://webrtc.org/experiments/rtp-hdrext/abs-capture-time/
//
class AbsoluteCaptureTimeReceiver {
 public:
  static constexpr TimeDelta kInterpolationMaxInterval =
      TimeDelta::Millis(5000);

  explicit AbsoluteCaptureTimeReceiver(Clock* clock);

  // Returns the source (i.e. SSRC or CSRC) of the capture system.
  static uint32_t GetSource(uint32_t ssrc,
                            rtc::ArrayView<const uint32_t> csrcs);

  // Sets the NTP clock offset between the sender system (which may be different
  // from the capture system) and the local system. This information is normally
  // provided by passing half the value of the Round-Trip Time estimation given
  // by RTCP sender reports (see DLSR/DLRR).
  //
  // Note that the value must be in Q32.32-formatted fixed-point seconds.
  void SetRemoteToLocalClockOffset(absl::optional<int64_t> value_q32x32);

  // Returns a received header extension, an interpolated header extension, or
  // |absl::nullopt| if it's not possible to interpolate a header extension.
  absl::optional<AbsoluteCaptureTime> OnReceivePacket(
      uint32_t source,
      uint32_t rtp_timestamp,
      uint32_t rtp_clock_frequency,
      const absl::optional<AbsoluteCaptureTime>& received_extension);

 private:
  friend class AbsoluteCaptureTimeSender;

  static uint64_t InterpolateAbsoluteCaptureTimestamp(
      uint32_t rtp_timestamp,
      uint32_t rtp_clock_frequency,
      uint32_t last_rtp_timestamp,
      uint64_t last_absolute_capture_timestamp);

  bool ShouldInterpolateExtension(Timestamp receive_time,
                                  uint32_t source,
                                  uint32_t rtp_timestamp,
                                  uint32_t rtp_clock_frequency) const
      RTC_EXCLUSIVE_LOCKS_REQUIRED(mutex_);

  absl::optional<int64_t> AdjustEstimatedCaptureClockOffset(
      absl::optional<int64_t> received_value) const
      RTC_EXCLUSIVE_LOCKS_REQUIRED(mutex_);

  Clock* const clock_;

  Mutex mutex_;

  absl::optional<int64_t> remote_to_local_clock_offset_ RTC_GUARDED_BY(mutex_);

  Timestamp last_receive_time_ RTC_GUARDED_BY(mutex_);

  uint32_t last_source_ RTC_GUARDED_BY(mutex_);
  uint32_t last_rtp_timestamp_ RTC_GUARDED_BY(mutex_);
  uint32_t last_rtp_clock_frequency_ RTC_GUARDED_BY(mutex_);
  uint64_t last_absolute_capture_timestamp_ RTC_GUARDED_BY(mutex_);
  absl::optional<int64_t> last_estimated_capture_clock_offset_
      RTC_GUARDED_BY(mutex_);
};  // AbsoluteCaptureTimeReceiver

}  // namespace webrtc

#endif  // MODULES_RTP_RTCP_SOURCE_ABSOLUTE_CAPTURE_TIME_RECEIVER_H_
