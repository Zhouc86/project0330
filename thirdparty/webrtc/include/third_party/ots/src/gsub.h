// Copyright (c) 2011-2017 The OTS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OTS_GSUB_H_
#define OTS_GSUB_H_

#include "ots.h"

namespace ots {

class OpenTypeGSUB : public Table {
 public:
  explicit OpenTypeGSUB(Font *font, uint32_t tag)
      : Table(font, tag, tag),
        num_lookups(0),
        m_data(NULL),
        m_length(0) {
  }

  bool Parse(const uint8_t *data, size_t length);
  bool Serialize(OTSStream *out);

  // Number of lookups in GPSUB table
  uint16_t num_lookups;

 //private:
  const uint8_t *m_data;
  size_t m_length;
};

}  // namespace ots

#endif  // OTS_GSUB_H_

