// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_PERMISSIONS_WORKER_NAVIGATOR_PERMISSIONS_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_PERMISSIONS_WORKER_NAVIGATOR_PERMISSIONS_H_

#include "third_party/blink/renderer/core/workers/worker_navigator.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/supplementable.h"

namespace blink {

class WorkerNavigator;
class Permissions;

class WorkerNavigatorPermissions final
    : public GarbageCollected<WorkerNavigatorPermissions>,
      public Supplement<WorkerNavigator> {
 public:
  static const char kSupplementName[];

  static WorkerNavigatorPermissions& From(WorkerNavigator&);
  static Permissions* permissions(WorkerNavigator&);

  WorkerNavigatorPermissions();

  void Trace(Visitor*) const override;

 private:
  Member<Permissions> permissions_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_PERMISSIONS_WORKER_NAVIGATOR_PERMISSIONS_H_
