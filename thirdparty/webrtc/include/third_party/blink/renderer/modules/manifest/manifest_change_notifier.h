// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_MANIFEST_MANIFEST_CHANGE_NOTIFIER_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_MANIFEST_MANIFEST_CHANGE_NOTIFIER_H_

#include "base/macros.h"
#include "third_party/blink/public/mojom/manifest/manifest_observer.mojom-blink.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/heap/member.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_associated_remote.h"

namespace blink {

class LocalDOMWindow;

class MODULES_EXPORT ManifestChangeNotifier
    : public GarbageCollected<ManifestChangeNotifier> {
 public:
  explicit ManifestChangeNotifier(LocalDOMWindow& window);
  virtual ~ManifestChangeNotifier();

  virtual void Trace(Visitor*) const;

  virtual void DidChangeManifest();

 private:
  void ReportManifestChange();
  void EnsureManifestChangeObserver();

  Member<LocalDOMWindow> window_;
  HeapMojoAssociatedRemote<mojom::blink::ManifestUrlChangeObserver>
      manifest_change_observer_;
  bool report_task_scheduled_ = false;

  DISALLOW_COPY_AND_ASSIGN(ManifestChangeNotifier);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_MANIFEST_MANIFEST_CHANGE_NOTIFIER_H_
