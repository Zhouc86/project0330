// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_SENSOR_GYROSCOPE_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_SENSOR_GYROSCOPE_H_

#include "third_party/blink/renderer/bindings/modules/v8/v8_spatial_sensor_options.h"
#include "third_party/blink/renderer/modules/sensor/sensor.h"

namespace blink {

class Gyroscope final : public Sensor {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static Gyroscope* Create(ExecutionContext*,
                           const SpatialSensorOptions*,
                           ExceptionState&);
  static Gyroscope* Create(ExecutionContext*, ExceptionState&);

  Gyroscope(ExecutionContext*, const SpatialSensorOptions*, ExceptionState&);

  base::Optional<double> x() const;
  base::Optional<double> y() const;
  base::Optional<double> z() const;

  void Trace(Visitor*) const override;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_SENSOR_GYROSCOPE_H_
