// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_RESIZE_OBSERVER_RESIZE_OBSERVER_SIZE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_RESIZE_OBSERVER_RESIZE_OBSERVER_SIZE_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

namespace blink {

class CORE_EXPORT ResizeObserverSize final : public ScriptWrappable {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static ResizeObserverSize* Create(double inline_size, double block_size);

  ResizeObserverSize(const double inline_size, const double block_size);
  ResizeObserverSize();

  double inlineSize() const { return inline_size_; }
  double blockSize() const { return block_size_; }

 private:
  double inline_size_;
  double block_size_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_RESIZE_OBSERVER_RESIZE_OBSERVER_SIZE_H_
