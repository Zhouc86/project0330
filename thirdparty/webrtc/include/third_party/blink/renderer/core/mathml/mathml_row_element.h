// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_MATHML_MATHML_ROW_ELEMENT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_MATHML_MATHML_ROW_ELEMENT_H_

#include "third_party/blink/renderer/core/mathml/mathml_element.h"

namespace blink {

class ComputedStyle;
class Document;

class CORE_EXPORT MathMLRowElement : public MathMLElement {
 public:
  explicit MathMLRowElement(const QualifiedName&, Document&);

 private:
  LayoutObject* CreateLayoutObject(const ComputedStyle&,
                                   LegacyLayout legacy) override;
};
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_MATHML_MATHML_ROW_ELEMENT_H_
