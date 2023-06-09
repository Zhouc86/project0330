// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_COMMON_BLUETOOTH_WEB_BLUETOOTH_DEVICE_ID_MOJOM_TRAITS_H_
#define THIRD_PARTY_BLINK_PUBLIC_COMMON_BLUETOOTH_WEB_BLUETOOTH_DEVICE_ID_MOJOM_TRAITS_H_

#include <string>

#include "third_party/blink/public/common/bluetooth/web_bluetooth_device_id.h"
#include "third_party/blink/public/mojom/bluetooth/web_bluetooth.mojom-shared.h"

namespace mojo {

template <>
struct StructTraits<blink::mojom::WebBluetoothDeviceIdDataView,
                    blink::WebBluetoothDeviceId> {
  static const std::string& device_id(
      const blink::WebBluetoothDeviceId& device_id) {
    return device_id.str();
  }

  static bool Read(blink::mojom::WebBluetoothDeviceIdDataView input,
                   blink::WebBluetoothDeviceId* output) {
    std::string result;

    if (!input.ReadDeviceId(&result))
      return false;
    if (!blink::WebBluetoothDeviceId::IsValid(result))
      return false;

    *output = blink::WebBluetoothDeviceId(std::move(result));
    return true;
  }
};

}  // namespace mojo

#endif  // THIRD_PARTY_BLINK_PUBLIC_COMMON_BLUETOOTH_WEB_BLUETOOTH_DEVICE_ID_MOJOM_TRAITS_H_
