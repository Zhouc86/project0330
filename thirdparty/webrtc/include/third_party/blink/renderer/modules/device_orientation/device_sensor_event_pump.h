// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_DEVICE_ORIENTATION_DEVICE_SENSOR_EVENT_PUMP_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_DEVICE_ORIENTATION_DEVICE_SENSOR_EVENT_PUMP_H_

#include "mojo/public/cpp/bindings/pending_remote.h"
#include "services/device/public/mojom/sensor_provider.mojom-blink.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_remote.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_wrapper_mode.h"
#include "third_party/blink/renderer/platform/timer.h"

namespace blink {

class LocalFrame;

class MODULES_EXPORT DeviceSensorEventPump : public GarbageCollectedMixin {
 public:
  // Default rate for firing events.
  static constexpr int kDefaultPumpFrequencyHz = 60;
  static constexpr int kDefaultPumpDelayMicroseconds =
      base::Time::kMicrosecondsPerSecond / kDefaultPumpFrequencyHz;

  // The pump is a tri-state automaton with allowed transitions as follows:
  // STOPPED -> PENDING_START
  // PENDING_START -> RUNNING
  // PENDING_START -> STOPPED
  // RUNNING -> STOPPED
  enum class PumpState { STOPPED, RUNNING, PENDING_START };

  void HandleSensorProviderError();

  void SetSensorProviderForTesting(
      mojo::PendingRemote<device::mojom::blink::SensorProvider>
          sensor_provider);
  PumpState GetPumpStateForTesting();

  void Trace(Visitor* visitor) const override;

 protected:
  friend class DeviceSensorEntry;

  explicit DeviceSensorEventPump(LocalFrame&);
  virtual ~DeviceSensorEventPump();

  // Manage PumpState and call SendStartMessage.
  void Start(LocalFrame& frame);

  // This method is expected to send an IPC to the browser process to let it
  // know that it should start observing.
  virtual void SendStartMessage(LocalFrame&) = 0;

  // Manage PumpState and call SendStopMessage.
  void Stop();

  // This method is expected to send an IPC to the browser process to let it
  // know that it should start observing.
  virtual void SendStopMessage() = 0;

  // Even though the TimerBase* parameter is not used, it is required by
  // TaskRunnerTimer class
  virtual void FireEvent(TimerBase*) = 0;

  virtual void DidStartIfPossible();

  HeapMojoRemote<device::mojom::blink::SensorProvider,
                 HeapMojoWrapperMode::kWithoutContextObserver>
      sensor_provider_;

  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;

 private:
  virtual bool SensorsReadyOrErrored() const = 0;

  PumpState state_;
  TaskRunnerTimer<DeviceSensorEventPump> timer_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_DEVICE_ORIENTATION_DEVICE_SENSOR_EVENT_PUMP_H_
