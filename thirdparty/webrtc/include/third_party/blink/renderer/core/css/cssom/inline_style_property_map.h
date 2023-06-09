// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_INLINE_STYLE_PROPERTY_MAP_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_INLINE_STYLE_PROPERTY_MAP_H_

#include "third_party/blink/renderer/core/css/cssom/style_property_map.h"
#include "third_party/blink/renderer/core/dom/element.h"

namespace blink {

class CORE_EXPORT InlineStylePropertyMap final : public StylePropertyMap {
 public:
  explicit InlineStylePropertyMap(Element* owner_element)
      : owner_element_(owner_element) {}
  InlineStylePropertyMap(const InlineStylePropertyMap&) = delete;
  InlineStylePropertyMap& operator=(const InlineStylePropertyMap&) = delete;

  void Trace(Visitor* visitor) const override {
    visitor->Trace(owner_element_);
    StylePropertyMap::Trace(visitor);
  }

  unsigned int size() const final;

 protected:
  const CSSValue* GetProperty(CSSPropertyID) const override;
  const CSSValue* GetCustomProperty(AtomicString) const override;
  void ForEachProperty(const IterationCallback&) override;
  void SetProperty(CSSPropertyID, const CSSValue&) override;
  bool SetShorthandProperty(CSSPropertyID,
                            const String&,
                            SecureContextMode) override;
  void SetCustomProperty(const AtomicString&, const CSSValue&) override;
  void RemoveProperty(CSSPropertyID) override;
  void RemoveCustomProperty(const AtomicString&) override;
  void RemoveAllProperties() final;

  String SerializationForShorthand(const CSSProperty&) const final;

 private:
  Member<Element> owner_element_;
};

}  // namespace blink

#endif
