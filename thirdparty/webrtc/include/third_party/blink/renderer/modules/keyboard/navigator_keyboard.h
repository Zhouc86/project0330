// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_KEYBOARD_NAVIGATOR_KEYBOARD_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_KEYBOARD_NAVIGATOR_KEYBOARD_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/frame/navigator.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/supplementable.h"

namespace blink {

class Keyboard;

// Navigator supplement which exposes keyboard related functionality.
class NavigatorKeyboard final : public GarbageCollected<NavigatorKeyboard>,
                                public Supplement<Navigator> {
 public:
  static const char kSupplementName[];
  static Keyboard* keyboard(Navigator&);

  explicit NavigatorKeyboard(Navigator&);

  void Trace(Visitor*) const override;

 private:
  Member<Keyboard> keyboard_;

  DISALLOW_COPY_AND_ASSIGN(NavigatorKeyboard);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_KEYBOARD_NAVIGATOR_KEYBOARD_H_
