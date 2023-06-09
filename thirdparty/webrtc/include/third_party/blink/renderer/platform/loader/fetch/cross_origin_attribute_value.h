// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_CROSS_ORIGIN_ATTRIBUTE_VALUE_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_CROSS_ORIGIN_ATTRIBUTE_VALUE_H_

namespace blink {

enum CrossOriginAttributeValue {
  kCrossOriginAttributeNotSet,
  kCrossOriginAttributeAnonymous,
  kCrossOriginAttributeUseCredentials,
};

}  // namespace blink

#endif
