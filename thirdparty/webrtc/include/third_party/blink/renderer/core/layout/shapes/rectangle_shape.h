/*
 * Copyright (C) 2012 Adobe Systems Incorporated. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_SHAPES_RECTANGLE_SHAPE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_SHAPES_RECTANGLE_SHAPE_H_

#include "third_party/blink/renderer/core/layout/shapes/shape.h"
#include "third_party/blink/renderer/platform/geometry/float_point.h"
#include "third_party/blink/renderer/platform/geometry/float_rect.h"
#include "third_party/blink/renderer/platform/geometry/float_size.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

class RectangleShape final : public Shape {
 public:
  RectangleShape(const FloatRect& bounds, const FloatSize& radii)
      : Shape(), bounds_(bounds), radii_(radii) {}

  LayoutRect ShapeMarginLogicalBoundingBox() const override {
    return static_cast<LayoutRect>(ShapeMarginBounds());
  }
  bool IsEmpty() const override { return bounds_.IsEmpty(); }
  LineSegment GetExcludedInterval(LayoutUnit logical_top,
                                  LayoutUnit logical_height) const override;
  void BuildDisplayPaths(DisplayPaths&) const override;

 private:
  FloatRect ShapeMarginBounds() const;

  float Rx() const { return radii_.Width(); }
  float Ry() const { return radii_.Height(); }
  float X() const { return bounds_.X(); }
  float Y() const { return bounds_.Y(); }
  float Width() const { return bounds_.Width(); }
  float Height() const { return bounds_.Height(); }

  FloatRect bounds_;
  FloatSize radii_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_SHAPES_RECTANGLE_SHAPE_H_
