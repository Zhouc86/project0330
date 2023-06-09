// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_GRAPHICS_LAYER_TREE_AS_TEXT_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_GRAPHICS_LAYER_TREE_AS_TEXT_H_

#include <memory>

#include "third_party/blink/renderer/platform/graphics/compositing/layers_as_json.h"
#include "third_party/blink/renderer/platform/platform_export.h"

namespace blink {

class GraphicsLayer;
class JSONObject;

PLATFORM_EXPORT std::unique_ptr<JSONObject> GraphicsLayerTreeAsJSON(
    const GraphicsLayer*,
    LayerTreeFlags);

PLATFORM_EXPORT String GraphicsLayerTreeAsTextForTesting(const GraphicsLayer*,
                                                         LayerTreeFlags);

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_GRAPHICS_LAYER_TREE_AS_TEXT_H_
