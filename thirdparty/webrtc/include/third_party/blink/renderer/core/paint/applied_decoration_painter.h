// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_APPLIED_DECORATION_PAINTER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_APPLIED_DECORATION_PAINTER_H_

#include "third_party/blink/renderer/core/paint/text_decoration_info.h"
#include "third_party/blink/renderer/platform/wtf/allocator/allocator.h"

namespace blink {

class GraphicsContext;

// Helper class for painting a text decorations. Each instance paints a single
// decoration.
class AppliedDecorationPainter final {
  STACK_ALLOCATED();

 public:
  AppliedDecorationPainter(GraphicsContext& context,
                           const TextDecorationInfo& decoration_info,
                           TextDecoration line)
      : context_(context), decoration_info_(decoration_info), line_(line) {}

  void Paint();

 private:
  void StrokeWavyTextDecoration();

  GraphicsContext& context_;
  const TextDecorationInfo& decoration_info_;
  TextDecoration line_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_APPLIED_DECORATION_PAINTER_H_
