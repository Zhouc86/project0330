// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_RESIZE_OBSERVER_RESIZE_OBSERVER_UTILITIES_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_RESIZE_OBSERVER_RESIZE_OBSERVER_UTILITIES_H_

#include "third_party/blink/renderer/core/resize_observer/resize_observer_box_options.h"

namespace blink {

class FloatSize;
class LayoutSize;
class DOMRectReadOnly;
class LayoutObject;
class ComputedStyle;
class LayoutRect;

// Helper functions for ResizeObserverEntry and ResizeObservation.
class ResizeObserverUtilities {
 public:
  // Given |box_option|, compute the appropriate box for use with
  // ResizeObserver. This takes the following factors into account: writing
  // mode, effective zoom (for non-device-pixel boxes) and pixel snapping for
  // device-pixel boxes.
  static FloatSize ComputeZoomAdjustedBox(ResizeObserverBoxOptions box_option,
                                          LayoutObject* layout_object,
                                          const ComputedStyle& style);

  // Compute a scaled and pixel snapped device pixel content box for svg
  // bounding boxes.
  static FloatSize ComputeSnappedDevicePixelContentBox(
      LayoutSize box_size,
      LayoutObject* layout_object,
      const ComputedStyle& style);

  static DOMRectReadOnly* ZoomAdjustedLayoutRect(LayoutRect content_rect,
                                                 const ComputedStyle& style);
};

}  // namespace blink

#endif
