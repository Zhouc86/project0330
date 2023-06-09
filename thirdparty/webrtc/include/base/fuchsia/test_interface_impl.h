// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FUCHSIA_FAKE_INTERFACE_IMPL_H_
#define BASE_FUCHSIA_FAKE_INTERFACE_IMPL_H_

#include <lib/fidl/cpp/binding_set.h>
#include <lib/zx/channel.h>

#include "base/testfidl/cpp/fidl.h"

namespace base {

class TestInterfaceImpl : public testfidl::TestInterface {
 public:
  TestInterfaceImpl();
  ~TestInterfaceImpl() override;

  // TestInterface implementation:
  void Add(int32_t a, int32_t b, AddCallback callback) override;

  fidl::BindingSet<testfidl::TestInterface>* bindings() { return &bindings_; }

 private:
  fidl::BindingSet<testfidl::TestInterface> bindings_;
};

}  // namespace base

#endif  // BASE_FUCHSIA_FAKE_INTERFACE_IMPL_H_
