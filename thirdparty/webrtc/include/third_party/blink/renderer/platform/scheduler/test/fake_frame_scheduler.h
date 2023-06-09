// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_SCHEDULER_TEST_FAKE_FRAME_SCHEDULER_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_SCHEDULER_TEST_FAKE_FRAME_SCHEDULER_H_

#include "third_party/blink/renderer/platform/scheduler/main_thread/frame_scheduler_impl.h"
#include "third_party/blink/renderer/platform/scheduler/main_thread/main_thread_task_queue.h"
#include "third_party/blink/renderer/platform/scheduler/public/frame_scheduler.h"
#include "third_party/blink/renderer/platform/scheduler/worker/worker_scheduler_proxy.h"
#include "third_party/blink/renderer/platform/wtf/allocator/allocator.h"

namespace blink {
namespace scheduler {

class MainThreadTaskQueueForTest : public MainThreadTaskQueue {
 public:
  using MainThreadTaskQueue::SetFrameSchedulerForTest;

  explicit MainThreadTaskQueueForTest(
      QueueTraits::PrioritisationType prioritisation_type)
      : MainThreadTaskQueue(
            nullptr,
            Spec(MainThreadTaskQueue::NameForQueueType(
                MainThreadTaskQueue::QueueType::kTest)),
            QueueCreationParams(MainThreadTaskQueue::QueueType::kTest)
                .SetQueueTraits(
                    QueueTraits().SetPrioritisationType(prioritisation_type)),
            nullptr) {}
  explicit MainThreadTaskQueueForTest(QueueType queue_type)
      : MainThreadTaskQueue(nullptr,
                            Spec(MainThreadTaskQueue::NameForQueueType(
                                MainThreadTaskQueue::QueueType::kTest)),
                            QueueCreationParams(queue_type),
                            nullptr) {}
  ~MainThreadTaskQueueForTest() override = default;
};

// A dummy FrameScheduler for tests.
class FakeFrameScheduler : public FrameSchedulerImpl {
 public:
  FakeFrameScheduler()
      : page_scheduler_(nullptr),
        is_page_visible_(false),
        is_frame_visible_(false),
        frame_type_(FrameScheduler::FrameType::kSubframe),
        is_cross_origin_to_main_frame_(false),
        is_exempt_from_throttling_(false) {}

  FakeFrameScheduler(PageScheduler* page_scheduler,
                     bool is_page_visible,
                     bool is_frame_visible,
                     FrameScheduler::FrameType frame_type,
                     bool is_cross_origin_to_main_frame,
                     bool is_exempt_from_throttling,
                     FrameScheduler::Delegate* delegate)
      : FrameSchedulerImpl(/*main_thread_scheduler=*/nullptr,
                           /*parent_page_scheduler=*/nullptr,
                           /*delegate=*/delegate,
                           /*blame_context=*/nullptr,
                           /*frame_type=*/frame_type),
        page_scheduler_(page_scheduler),
        is_page_visible_(is_page_visible),
        is_frame_visible_(is_frame_visible),
        frame_type_(frame_type),
        is_cross_origin_to_main_frame_(is_cross_origin_to_main_frame),
        is_exempt_from_throttling_(is_exempt_from_throttling) {
    DCHECK(frame_type_ != FrameType::kMainFrame ||
           !is_cross_origin_to_main_frame);
  }
  ~FakeFrameScheduler() override = default;

  class Builder {
    USING_FAST_MALLOC(Builder);

   public:
    Builder() = default;

    std::unique_ptr<FakeFrameScheduler> Build() {
      return std::make_unique<FakeFrameScheduler>(
          page_scheduler_, is_page_visible_, is_frame_visible_, frame_type_,
          is_cross_origin_to_main_frame_, is_exempt_from_throttling_,
          delegate_);
    }

    Builder& SetPageScheduler(PageScheduler* page_scheduler) {
      page_scheduler_ = page_scheduler;
      return *this;
    }

    Builder& SetIsPageVisible(bool is_page_visible) {
      is_page_visible_ = is_page_visible;
      return *this;
    }

    Builder& SetIsFrameVisible(bool is_frame_visible) {
      is_frame_visible_ = is_frame_visible;
      return *this;
    }

    Builder& SetFrameType(FrameScheduler::FrameType frame_type) {
      frame_type_ = frame_type;
      return *this;
    }

    Builder& SetIsCrossOriginToMainFrame(bool is_cross_origin_to_main_frame) {
      is_cross_origin_to_main_frame_ = is_cross_origin_to_main_frame;
      return *this;
    }

    Builder& SetIsExemptFromThrottling(bool is_exempt_from_throttling) {
      is_exempt_from_throttling_ = is_exempt_from_throttling;
      return *this;
    }

    Builder& SetDelegate(FrameScheduler::Delegate* delegate) {
      delegate_ = delegate;
      return *this;
    }

   private:
    PageScheduler* page_scheduler_ = nullptr;
    bool is_page_visible_ = false;
    bool is_frame_visible_ = false;
    FrameScheduler::FrameType frame_type_ =
        FrameScheduler::FrameType::kMainFrame;
    bool is_cross_origin_to_main_frame_ = false;
    bool is_exempt_from_throttling_ = false;
    FrameScheduler::Delegate* delegate_ = nullptr;
  };

  // FrameScheduler implementation:
  void SetFrameVisible(bool) override {}
  bool IsFrameVisible() const override { return is_frame_visible_; }
  bool IsPageVisible() const override { return is_page_visible_; }
  void SetPaused(bool) override {}
  void SetCrossOriginToMainFrame(bool) override {}
  bool IsCrossOriginToMainFrame() const override {
    return is_cross_origin_to_main_frame_;
  }
  void TraceUrlChange(const String&) override {}
  FrameScheduler::FrameType GetFrameType() const override {
    return frame_type_;
  }
  scoped_refptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType) override {
    return nullptr;
  }
  PageScheduler* GetPageScheduler() const override { return page_scheduler_; }
  WebScopedVirtualTimePauser CreateWebScopedVirtualTimePauser(
      const WTF::String& name,
      WebScopedVirtualTimePauser::VirtualTaskDuration duration) override {
    return WebScopedVirtualTimePauser();
  }
  void DidStartProvisionalLoad(bool is_main_frame) override {}
  void DidCommitProvisionalLoad(
      bool is_web_history_inert_commit,
      FrameScheduler::NavigationType navigation_type) override {}
  void OnFirstMeaningfulPaint() override {}
  void OnStartedUsingFeature(SchedulingPolicy::Feature feature,
                             const SchedulingPolicy& policy) override {}
  void OnStoppedUsingFeature(SchedulingPolicy::Feature feature,
                             const SchedulingPolicy& policy) override {}
  bool IsExemptFromBudgetBasedThrottling() const override {
    return is_exempt_from_throttling_;
  }
  std::unique_ptr<WorkerSchedulerProxy> CreateWorkerSchedulerProxy() {
    return nullptr;
  }
  std::unique_ptr<blink::mojom::blink::PauseSubresourceLoadingHandle>
  GetPauseSubresourceLoadingHandle() override {
    return nullptr;
  }

 private:
  PageScheduler* page_scheduler_;  // NOT OWNED

  bool is_page_visible_;
  bool is_frame_visible_;
  FrameScheduler::FrameType frame_type_;
  bool is_cross_origin_to_main_frame_;
  bool is_exempt_from_throttling_;
  DISALLOW_COPY_AND_ASSIGN(FakeFrameScheduler);
};

}  // namespace scheduler
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_SCHEDULER_TEST_FAKE_FRAME_SCHEDULER_H_
