// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_NG_LAYOUT_NG_TEXT_CONTROL_MULTI_LINE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_NG_LAYOUT_NG_TEXT_CONTROL_MULTI_LINE_H_

#include "third_party/blink/renderer/core/layout/ng/layout_ng_block_flow.h"

namespace blink {

// LayoutNGTextControlSingleLine is a LayoutObject for <textarea>.
class LayoutNGTextControlMultiLine final : public LayoutNGBlockFlow {
 public:
  explicit LayoutNGTextControlMultiLine(Element* element);

 private:
  bool IsOfType(LayoutObjectType) const override;

  const char* GetName() const override {
    NOT_DESTROYED();
    return "LayoutNGTextControlMultiLine";
  }

  bool CreatesNewFormattingContext() const override {
    NOT_DESTROYED();
    return true;
  }
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_NG_LAYOUT_NG_TEXT_CONTROL_MULTI_LINE_H_
