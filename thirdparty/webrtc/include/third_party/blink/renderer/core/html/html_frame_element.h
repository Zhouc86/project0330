/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Simon Hausmann <hausmann@kde.org>
 * Copyright (C) 2004, 2006, 2009 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_HTML_HTML_FRAME_ELEMENT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_HTML_HTML_FRAME_ELEMENT_H_

#include "third_party/blink/public/common/feature_policy/feature_policy.h"
#include "third_party/blink/public/mojom/frame/frame_owner_element_type.mojom-blink.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/html/html_frame_element_base.h"

namespace blink {

class CORE_EXPORT HTMLFrameElement final : public HTMLFrameElementBase {
  DEFINE_WRAPPERTYPEINFO();

 public:
  explicit HTMLFrameElement(Document&);

  bool HasFrameBorder() const { return frame_border_; }

  bool NoResize() const;

  ParsedFeaturePolicy ConstructContainerPolicy() const override;

  mojom::blink::FrameOwnerElementType OwnerType() const final {
    return mojom::blink::FrameOwnerElementType::kFrame;
  }

 private:
  void AttachLayoutTree(AttachContext&) override;

  bool LayoutObjectIsNeeded(const ComputedStyle&) const override;
  LayoutObject* CreateLayoutObject(const ComputedStyle&, LegacyLayout) override;

  void ParseAttribute(const AttributeModificationParams&) override;

  bool frame_border_;
  bool frame_border_set_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_HTML_HTML_FRAME_ELEMENT_H_
