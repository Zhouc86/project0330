// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_WIDGET_INPUT_EVENT_WITH_CALLBACK_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_WIDGET_INPUT_EVENT_WITH_CALLBACK_H_

#include <list>

#include "third_party/blink/public/common/input/web_coalesced_input_event.h"
#include "third_party/blink/public/platform/input/input_handler_proxy.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "ui/latency/latency_info.h"

namespace blink {

namespace test {
class InputHandlerProxyEventQueueTest;
}

class PLATFORM_EXPORT EventWithCallback {
 public:
  struct PLATFORM_EXPORT OriginalEventWithCallback {
    OriginalEventWithCallback(
        std::unique_ptr<WebCoalescedInputEvent> event,
        InputHandlerProxy::EventDispositionCallback callback);
    ~OriginalEventWithCallback();
    std::unique_ptr<WebCoalescedInputEvent> event_;
    InputHandlerProxy::EventDispositionCallback callback_;
  };
  using OriginalEventList = std::list<OriginalEventWithCallback>;

  EventWithCallback(std::unique_ptr<WebCoalescedInputEvent> event,
                    base::TimeTicks timestamp_now,
                    InputHandlerProxy::EventDispositionCallback callback);
  EventWithCallback(std::unique_ptr<WebCoalescedInputEvent> event,
                    base::TimeTicks creation_timestamp,
                    base::TimeTicks last_coalesced_timestamp,
                    OriginalEventList original_events);
  ~EventWithCallback();

  bool CanCoalesceWith(const EventWithCallback& other) const WARN_UNUSED_RESULT;
  void CoalesceWith(EventWithCallback* other, base::TimeTicks timestamp_now);

  void RunCallbacks(InputHandlerProxy::EventDisposition,
                    const ui::LatencyInfo& latency,
                    std::unique_ptr<InputHandlerProxy::DidOverscrollParams>,
                    const WebInputEventAttribution&);

  const WebInputEvent& event() const { return event_->Event(); }
  WebInputEvent* event_pointer() { return event_->EventPointer(); }
  const ui::LatencyInfo& latency_info() const { return event_->latency_info(); }
  ui::LatencyInfo& latency_info() { return event_->latency_info(); }
  base::TimeTicks creation_timestamp() const { return creation_timestamp_; }
  base::TimeTicks last_coalesced_timestamp() const {
    return last_coalesced_timestamp_;
  }
  void set_coalesced_scroll_and_pinch() { coalesced_scroll_and_pinch_ = true; }
  bool coalesced_scroll_and_pinch() const {
    return coalesced_scroll_and_pinch_;
  }
  size_t coalesced_count() const { return original_events_.size(); }
  OriginalEventList& original_events() { return original_events_; }
  // |first_original_event()| is used as ID for tracing.
  WebInputEvent* first_original_event() {
    return original_events_.empty()
               ? nullptr
               : original_events_.front().event_->EventPointer();
  }
  void SetScrollbarManipulationHandledOnCompositorThread();

 private:
  friend class test::InputHandlerProxyEventQueueTest;

  void SetTickClockForTesting(std::unique_ptr<base::TickClock> tick_clock);

  std::unique_ptr<WebCoalescedInputEvent> event_;
  OriginalEventList original_events_;
  bool coalesced_scroll_and_pinch_ = false;

  base::TimeTicks creation_timestamp_;
  base::TimeTicks last_coalesced_timestamp_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_WIDGET_INPUT_EVENT_WITH_CALLBACK_H_
