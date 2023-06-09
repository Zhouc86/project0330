// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CONTROLLER_MEMORY_USAGE_MONITOR_WIN_H_
#define THIRD_PARTY_BLINK_RENDERER_CONTROLLER_MEMORY_USAGE_MONITOR_WIN_H_

#include "third_party/blink/renderer/controller/controller_export.h"
#include "third_party/blink/renderer/controller/memory_usage_monitor.h"

namespace blink {

// Memory usage monitor implementation for Windows.
class CONTROLLER_EXPORT MemoryUsageMonitorWin : public MemoryUsageMonitor {
 public:
  MemoryUsageMonitorWin() = default;

 private:
  void GetProcessMemoryUsage(MemoryUsage&) override;
  static bool CalculateProcessMemoryFootprint(uint64_t* private_footprint);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CONTROLLER_MEMORY_USAGE_MONITOR_WIN_H_
