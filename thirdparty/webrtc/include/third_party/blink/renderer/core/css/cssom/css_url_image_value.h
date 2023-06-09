// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_URL_IMAGE_VALUE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_URL_IMAGE_VALUE_H_

#include "third_party/blink/renderer/core/css/cssom/css_style_image_value.h"

namespace blink {

class CSSImageValue;

class CORE_EXPORT CSSURLImageValue final : public CSSStyleImageValue {
 public:
  explicit CSSURLImageValue(const CSSImageValue& value) : value_(value) {}
  CSSURLImageValue(const CSSURLImageValue&) = delete;
  CSSURLImageValue& operator=(const CSSURLImageValue&) = delete;

  const String& url() const;

  // CSSStyleImageValue
  base::Optional<IntSize> IntrinsicSize() const final;

  // CanvasImageSource
  ResourceStatus Status() const final;
  scoped_refptr<Image> GetSourceImageForCanvas(SourceImageStatus*,
                                               const FloatSize&) final;
  bool IsAccelerated() const final;

  // CSSStyleValue
  StyleValueType GetType() const final { return kURLImageType; }
  const CSSValue* ToCSSValue() const final;

  void Trace(Visitor*) const override;

 private:
  scoped_refptr<Image> GetImage() const;

  Member<const CSSImageValue> value_;
};

}  // namespace blink

#endif  // CSSResourceValue_h
