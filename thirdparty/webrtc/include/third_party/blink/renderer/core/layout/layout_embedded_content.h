/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Simon Hausmann <hausmann@kde.org>
 * Copyright (C) 2006, 2009 Apple Inc. All rights reserved.
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

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_LAYOUT_EMBEDDED_CONTENT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_LAYOUT_EMBEDDED_CONTENT_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/html/html_frame_owner_element.h"
#include "third_party/blink/renderer/core/layout/layout_replaced.h"

namespace ui {
class Cursor;
}

namespace blink {

class EmbeddedContentView;
class FrameView;
class WebPluginContainerImpl;

// LayoutObject for frames via LayoutFrame and LayoutIFrame, and plugins via
// LayoutEmbeddedObject.
class CORE_EXPORT LayoutEmbeddedContent : public LayoutReplaced {
 public:
  explicit LayoutEmbeddedContent(HTMLFrameOwnerElement*);
  ~LayoutEmbeddedContent() override;

  bool ContentDocumentIsCompositing() const;

  bool NodeAtPoint(HitTestResult&,
                   const HitTestLocation&,
                   const PhysicalOffset& accumulated_offset,
                   HitTestAction) override;

  void AddRef() {
    NOT_DESTROYED();
    ++ref_count_;
  }
  void Release();

  // LayoutEmbeddedContent::ChildFrameView returns the LocalFrameView associated
  // with the current Node, if Node is HTMLFrameOwnerElement. This is different
  // to LayoutObject::GetFrameView which returns the LocalFrameView associated
  // with the root Document Frame.
  FrameView* ChildFrameView() const;
  LayoutView* ChildLayoutView() const;
  WebPluginContainerImpl* Plugin() const;
  EmbeddedContentView* GetEmbeddedContentView() const;

  PhysicalRect ReplacedContentRect() const final;

  void UpdateOnEmbeddedContentViewChange();
  void UpdateGeometry(EmbeddedContentView&);

  bool IsLayoutEmbeddedContent() const final {
    NOT_DESTROYED();
    return true;
  }

  bool IsThrottledFrameView() const;

 protected:
  PaintLayerType LayerTypeRequired() const override;

  void StyleDidChange(StyleDifference, const ComputedStyle* old_style) final;
  void UpdateLayout() override;
  void PaintReplaced(const PaintInfo&,
                     const PhysicalOffset& paint_offset) const override;
  void InvalidatePaint(const PaintInvalidatorContext&) const final;
  CursorDirective GetCursor(const PhysicalOffset&, ui::Cursor&) const final;

  bool CanBeSelectionLeafInternal() const final {
    NOT_DESTROYED();
    return true;
  }

  HTMLFrameOwnerElement* GetFrameOwnerElement() const {
    NOT_DESTROYED();
    return To<HTMLFrameOwnerElement>(GetNode());
  }

 private:
  bool CanHaveAdditionalCompositingReasons() const override {
    NOT_DESTROYED();
    return true;
  }
  CompositingReasons AdditionalCompositingReasons() const override;

  void WillBeDestroyed() final;
  void DeleteThis() final;

  bool NodeAtPointOverEmbeddedContentView(
      HitTestResult&,
      const HitTestLocation&,
      const PhysicalOffset& accumulated_offset,
      HitTestAction);

  int ref_count_;
};

DEFINE_LAYOUT_OBJECT_TYPE_CASTS(LayoutEmbeddedContent,
                                IsLayoutEmbeddedContent());

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_LAYOUT_EMBEDDED_CONTENT_H_
