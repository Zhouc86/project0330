// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_MEMORY_PRESSURE_LISTENER_H_
#define THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_MEMORY_PRESSURE_LISTENER_H_

#include "third_party/blink/public/platform/web_common.h"
#include "third_party/blink/public/platform/web_memory_pressure_level.h"

namespace blink {

class WebMemoryPressureListener {
 public:
  // Called when a memory pressure notification is received.
  BLINK_PLATFORM_EXPORT static void OnMemoryPressure(WebMemoryPressureLevel);

  BLINK_PLATFORM_EXPORT static void OnPurgeMemory();
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_MEMORY_PRESSURE_LISTENER_H_
