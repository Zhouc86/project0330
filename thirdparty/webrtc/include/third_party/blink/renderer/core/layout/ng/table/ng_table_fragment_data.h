// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_NG_TABLE_NG_TABLE_FRAGMENT_DATA_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_NG_TABLE_NG_TABLE_FRAGMENT_DATA_H_

#include "third_party/blink/renderer/core/layout/ng/ng_layout_input_node.h"
#include "third_party/blink/renderer/core/style/computed_style.h"
#include "third_party/blink/renderer/platform/geometry/layout_unit.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class NGTableFragmentData {
 public:
  // COLGROUP/COL geometry information. Used for painting column backgrounds.
  // Only present if column has a background.
  struct ColumnGeometry {
    wtf_size_t start_column;
    wtf_size_t span;
    LayoutUnit inline_offset;
    LayoutUnit inline_size;
    NGLayoutInputNode node;
  };

  using ColumnGeometries = Vector<ColumnGeometry>;

  // Column/row location is used for collapsed border painting.
  // Only present if borders are collapsed.
  struct CollapsedBordersGeometry {
    Vector<LayoutUnit> columns;  // Column offsets from table grid border.
    Vector<LayoutUnit> rows;     // Row offsets from table grid border.
  };
};

}  // namespace blink

WTF_ALLOW_MOVE_INIT_AND_COMPARE_WITH_MEM_FUNCTIONS(
    blink::NGTableFragmentData::ColumnGeometry)

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_NG_TABLE_NG_TABLE_FRAGMENT_DATA_H_
