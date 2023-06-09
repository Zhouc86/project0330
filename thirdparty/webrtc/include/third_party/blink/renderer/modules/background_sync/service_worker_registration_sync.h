// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_BACKGROUND_SYNC_SERVICE_WORKER_REGISTRATION_SYNC_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_BACKGROUND_SYNC_SERVICE_WORKER_REGISTRATION_SYNC_H_

#include "base/macros.h"
#include "third_party/blink/renderer/modules/service_worker/service_worker_registration.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/supplementable.h"

namespace blink {

class PeriodicSyncManager;
class SyncManager;
class ServiceWorkerRegistration;

class ServiceWorkerRegistrationSync final
    : public GarbageCollected<ServiceWorkerRegistrationSync>,
      public Supplement<ServiceWorkerRegistration> {
 public:
  static const char kSupplementName[];

  static ServiceWorkerRegistrationSync& From(
      ServiceWorkerRegistration& registration);

  static PeriodicSyncManager* periodicSync(
      ServiceWorkerRegistration& registration);
  static SyncManager* sync(ServiceWorkerRegistration& registration);

  explicit ServiceWorkerRegistrationSync(
      ServiceWorkerRegistration* registration);
  virtual ~ServiceWorkerRegistrationSync();

  PeriodicSyncManager* periodicSync();
  SyncManager* sync();

  void Trace(Visitor*) const override;

 private:
  Member<ServiceWorkerRegistration> registration_;
  Member<SyncManager> sync_manager_;
  Member<PeriodicSyncManager> periodic_sync_manager_;

  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerRegistrationSync);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_BACKGROUND_SYNC_SERVICE_WORKER_REGISTRATION_SYNC_H_
