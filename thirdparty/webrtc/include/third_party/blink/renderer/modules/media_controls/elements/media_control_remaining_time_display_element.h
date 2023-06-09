// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIA_CONTROLS_ELEMENTS_MEDIA_CONTROL_REMAINING_TIME_DISPLAY_ELEMENT_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIA_CONTROLS_ELEMENTS_MEDIA_CONTROL_REMAINING_TIME_DISPLAY_ELEMENT_H_

#include "third_party/blink/renderer/modules/media_controls/elements/media_control_time_display_element.h"

namespace blink {

class MediaControlsImpl;

class MediaControlRemainingTimeDisplayElement final
    : public MediaControlTimeDisplayElement {
 public:
  explicit MediaControlRemainingTimeDisplayElement(MediaControlsImpl&);

 private:
  int EstimateElementWidth() const override;

  String FormatTime() const override;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIA_CONTROLS_ELEMENTS_MEDIA_CONTROL_REMAINING_TIME_DISPLAY_ELEMENT_H_
