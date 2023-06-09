// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_WIDGET_INPUT_MAIN_THREAD_EVENT_QUEUE_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_WIDGET_INPUT_MAIN_THREAD_EVENT_QUEUE_H_

#include "base/feature_list.h"
#include "base/memory/weak_ptr.h"
#include "base/single_thread_task_runner.h"
#include "base/timer/timer.h"
#include "cc/input/touch_action.h"
#include "third_party/blink/public/common/input/web_input_event.h"
#include "third_party/blink/public/mojom/input/input_event_result.mojom-shared.h"
#include "third_party/blink/public/mojom/input/input_handler.mojom-blink.h"
#include "third_party/blink/public/platform/scheduler/web_thread_scheduler.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/widget/input/input_event_prediction.h"
#include "third_party/blink/renderer/platform/widget/input/main_thread_event_queue_task_list.h"
#include "ui/latency/latency_info.h"

namespace blink {

using HandledEventCallback =
    base::OnceCallback<void(mojom::blink::InputEventResultState ack_state,
                            const ui::LatencyInfo& latency_info,
                            mojom::blink::DidOverscrollParamsPtr,
                            base::Optional<cc::TouchAction>)>;

// All interaction with the MainThreadEventQueueClient will occur
// on the main thread.
class PLATFORM_EXPORT MainThreadEventQueueClient {
 public:
  // Handle an |event| that was previously queued (possibly coalesced with
  // another event). Returns false if the event will not be handled, and the
  // |handled_callback| will not be run.
  virtual bool HandleInputEvent(const WebCoalescedInputEvent& event,
                                HandledEventCallback handled_callback) = 0;
  // Requests a BeginMainFrame callback from the compositor.
  virtual void SetNeedsMainFrame() = 0;
};

// MainThreadEventQueue implements a queue for events that need to be
// queued between the compositor and main threads. This queue is managed
// by a lock where events are enqueued by the compositor thread
// and dequeued by the main thread.
//
// Below some example flows are how the code behaves.
// Legend: B=Browser, C=Compositor, M=Main Thread, NB=Non-blocking
//         BL=Blocking, PT=Post Task, ACK=Acknowledgement
//
// Normal blocking event sent to main thread.
//   B        C        M
//   ---(BL)-->
//         (queue)
//            ---(PT)-->
//                  (deque)
//   <-------(ACK)------
//
// Non-blocking event sent to main thread.
//   B        C        M
//   ---(NB)-->
//         (queue)
//            ---(PT)-->
//                  (deque)
//
// Non-blocking followed by blocking event sent to main thread.
//   B        C        M
//   ---(NB)-->
//         (queue)
//            ---(PT)-->
//   ---(BL)-->
//         (queue)
//            ---(PT)-->
//                  (deque)
//                  (deque)
//   <-------(ACK)------
//
class PLATFORM_EXPORT MainThreadEventQueue
    : public base::RefCountedThreadSafe<MainThreadEventQueue> {
 public:
  MainThreadEventQueue(
      MainThreadEventQueueClient* client,
      const scoped_refptr<base::SingleThreadTaskRunner>& main_task_runner,
      scheduler::WebThreadScheduler* main_thread_scheduler,
      bool allow_raf_aligned_input);

  // Type of dispatching of the event.
  enum class DispatchType { kBlocking, kNonBlocking };

  // Called once the compositor has handled |event| and indicated that it is
  // a non-blocking event to be queued to the main thread.
  void HandleEvent(std::unique_ptr<WebCoalescedInputEvent> event,
                   DispatchType dispatch_type,
                   mojom::blink::InputEventResultState ack_result,
                   const WebInputEventAttribution& attribution,
                   HandledEventCallback handled_callback);
  void DispatchRafAlignedInput(base::TimeTicks frame_time);
  void QueueClosure(base::OnceClosure closure);

  void ClearClient();
  void SetNeedsLowLatency(bool low_latency);
  void SetNeedsUnbufferedInputForDebugger(bool unbuffered);

  void HasPointerRawUpdateEventHandlers(bool has_handlers);

  // Request unbuffered input events until next pointerup.
  void RequestUnbufferedInputEvents();

  // Resampling event before dispatch it.
  void HandleEventResampling(
      const std::unique_ptr<MainThreadEventQueueTask>& item,
      base::TimeTicks frame_time);

  static bool IsForwardedAndSchedulerKnown(
      mojom::blink::InputEventResultState ack_state) {
    return ack_state == mojom::blink::InputEventResultState::kNotConsumed ||
           ack_state ==
               mojom::blink::InputEventResultState::kSetNonBlockingDueToFling;
  }

 protected:
  friend class base::RefCountedThreadSafe<MainThreadEventQueue>;
  virtual ~MainThreadEventQueue();
  void QueueEvent(std::unique_ptr<MainThreadEventQueueTask> event);
  void PostTaskToMainThread();
  void DispatchEvents();
  void PossiblyScheduleMainFrame();
  void SetNeedsMainFrame();
  // Returns false if the event can not be handled and the HandledEventCallback
  // will not be run.
  bool HandleEventOnMainThread(const WebCoalescedInputEvent& event,
                               const WebInputEventAttribution& attribution,
                               HandledEventCallback handled_callback);

  bool IsRawUpdateEvent(
      const std::unique_ptr<MainThreadEventQueueTask>& item) const;
  bool ShouldFlushQueue(
      const std::unique_ptr<MainThreadEventQueueTask>& item) const;
  bool IsRafAlignedEvent(
      const std::unique_ptr<MainThreadEventQueueTask>& item) const;
  void RafFallbackTimerFired();

  void ClearRafFallbackTimerForTesting();

  friend class QueuedWebInputEvent;
  friend class MainThreadEventQueueTest;
  friend class MainThreadEventQueueInitializationTest;
  MainThreadEventQueueClient* client_;
  bool last_touch_start_forced_nonblocking_due_to_fling_;
  bool needs_low_latency_;
  bool needs_unbuffered_input_for_debugger_;
  bool allow_raf_aligned_input_;
  bool needs_low_latency_until_pointer_up_ = false;
  bool has_pointerrawupdate_handlers_ = false;

  // Contains data to be shared between main thread and compositor thread.
  struct SharedState {
    SharedState();
    ~SharedState();

    MainThreadEventQueueTaskList events_;
    // A BeginMainFrame has been requested but not received yet.
    bool sent_main_frame_request_;
    // A PostTask to the main thread has been sent but not executed yet.
    bool sent_post_task_;
    base::TimeTicks last_async_touch_move_timestamp_;
  };

  // Lock used to serialize |shared_state_|.
  base::Lock shared_state_lock_;
  SharedState shared_state_;

  scoped_refptr<base::SingleThreadTaskRunner> main_task_runner_;
  scheduler::WebThreadScheduler* main_thread_scheduler_;

  // A safe guard timer to ensure input is always processed. A BeginMainFrame
  // signal might not always occur if our visibility changed.
  std::unique_ptr<base::OneShotTimer> raf_fallback_timer_;

  std::unique_ptr<InputEventPrediction> event_predictor_;

  DISALLOW_COPY_AND_ASSIGN(MainThreadEventQueue);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_WIDGET_INPUT_MAIN_THREAD_EVENT_QUEUE_H_
