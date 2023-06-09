// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_TESTING_FETCH_TESTING_PLATFORM_SUPPORT_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_TESTING_FETCH_TESTING_PLATFORM_SUPPORT_H_

#include <memory>

#include "third_party/blink/renderer/platform/heap/persistent.h"
#include "third_party/blink/renderer/platform/testing/testing_platform_support_with_mock_scheduler.h"

namespace blink {

class WebURLLoaderMockFactory;

class FetchTestingPlatformSupport
    : public TestingPlatformSupportWithMockScheduler {
 public:
  FetchTestingPlatformSupport();
  ~FetchTestingPlatformSupport() override;

  WebURLLoaderMockFactory* GetURLLoaderMockFactory();
  std::unique_ptr<WebURLLoaderFactory> CreateDefaultURLLoaderFactory();

 private:
  class FetchTestingWebURLLoaderMockFactory;

  std::unique_ptr<WebURLLoaderMockFactory> url_loader_mock_factory_;

  DISALLOW_COPY_AND_ASSIGN(FetchTestingPlatformSupport);
};

}  // namespace blink

#endif
