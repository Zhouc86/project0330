/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_TIME_RANGE_H_
#define THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_TIME_RANGE_H_

#include <algorithm>

#include "third_party/blink/public/platform/web_vector.h"

namespace blink {

class WebTimeRange {
 public:
  WebTimeRange() : start(0), end(0) {}
  WebTimeRange(double s, double e) : start(s), end(e) {}

  double start;
  double end;

  inline bool IsPointInRange(double point) const {
    return start <= point && point < end;
  }

  inline bool IsOverlappingRange(const WebTimeRange& range) const {
    return IsPointInRange(range.start) || IsPointInRange(range.end) ||
           range.IsPointInRange(start);
  }

  inline bool IsContiguousWithRange(const WebTimeRange& range) const {
    return range.start == end || range.end == start;
  }

  inline WebTimeRange UnionWithOverlappingOrContiguousRange(
      const WebTimeRange& range) const {
    WebTimeRange ret;

    ret.start = std::min(start, range.start);
    ret.end = std::max(end, range.end);

    return ret;
  }

  inline bool IsBeforeRange(const WebTimeRange& range) const {
    return range.start >= end;
  }
};

class BLINK_PLATFORM_EXPORT WebTimeRanges : public WebVector<WebTimeRange> {
 public:
  // Expose base constructors.
  using WebVector<WebTimeRange>::WebVector;

  void IntersectWith(const WebTimeRanges& other);
  void UnionWith(const WebTimeRanges& other);
  void Add(double start, double end);
  bool Contain(double time) const;
  double Nearest(double new_playback_position,
                 double current_playback_position) const;
  void Invert();
};

}  // namespace blink

#endif
