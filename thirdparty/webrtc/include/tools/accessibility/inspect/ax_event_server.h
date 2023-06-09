// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef AX_EVENT_SERVER_H_
#define AX_EVENT_SERVER_H_

#include "base/process/process_handle.h"
#include "build/build_config.h"
#include "content/browser/accessibility/accessibility_event_recorder.h"

#if defined(OS_WIN)
#include "base/win/scoped_com_initializer.h"
#endif

namespace tools {

class AXEventServer final {
 public:
  // Dumps events into console for application identified either by process id
  // or tree selector.
  explicit AXEventServer(
      base::ProcessId pid,
      const content::AccessibilityTreeFormatter::TreeSelector& selector);
  ~AXEventServer();

 private:
  void OnEvent(const std::string& event) const;

#if defined(OS_WIN)
  // Only one COM initializer per thread is permitted.
  base::win::ScopedCOMInitializer com_initializer_;
#endif
  std::unique_ptr<content::AccessibilityEventRecorder> recorder_;

  DISALLOW_COPY_AND_ASSIGN(AXEventServer);
};

}  // namespace tools

#endif  // AX_EVENT_SERVER_H_
