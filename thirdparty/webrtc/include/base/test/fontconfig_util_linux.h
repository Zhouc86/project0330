// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_FONTCONFIG_UTIL_LINUX_H_
#define BASE_TEST_FONTCONFIG_UTIL_LINUX_H_

namespace base {

// Prepares Fontconfig with a custom configuration suitable for tests.  FcInit()
// must still be called.
void SetUpFontconfig();

}  // namespace base

#endif  // BASE_TEST_FONTCONFIG_UTIL_LINUX_H_
