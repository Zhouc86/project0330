// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CONTROLLER_PERFORMANCE_MANAGER_RENDERER_RESOURCE_COORDINATOR_IMPL_H_
#define THIRD_PARTY_BLINK_RENDERER_CONTROLLER_PERFORMANCE_MANAGER_RENDERER_RESOURCE_COORDINATOR_IMPL_H_

#include "components/performance_manager/public/mojom/coordination_unit.mojom-blink.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "third_party/blink/renderer/controller/controller_export.h"
#include "third_party/blink/renderer/platform/instrumentation/resource_coordinator/renderer_resource_coordinator.h"
#include "third_party/blink/renderer/platform/wtf/allocator/allocator.h"

namespace blink {

class CONTROLLER_EXPORT RendererResourceCoordinatorImpl final
    : public RendererResourceCoordinator {
  USING_FAST_MALLOC(RendererResourceCoordinatorImpl);

 public:
  RendererResourceCoordinatorImpl(const RendererResourceCoordinatorImpl&) =
      delete;
  RendererResourceCoordinatorImpl& operator=(
      const RendererResourceCoordinatorImpl&) = delete;
  ~RendererResourceCoordinatorImpl() final;

  // Only initializes if the instrumentation runtime feature is enabled.
  static void MaybeInitialize();

  // RendererResourceCoordinator:
  void SetMainThreadTaskLoadIsLow(bool) final;
  void OnScriptStateCreated(ScriptState* script_state,
                            ExecutionContext* execution_context) final;
  void OnScriptStateDetached(ScriptState* script_state) final;
  void OnScriptStateDestroyed(ScriptState* script_state) final;

 private:
  explicit RendererResourceCoordinatorImpl(
      mojo::PendingRemote<
          performance_manager::mojom::blink::ProcessCoordinationUnit> remote);

  mojo::Remote<performance_manager::mojom::blink::ProcessCoordinationUnit>
      service_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_INSTRUMENTATION_RESOURCE_COORDINATOR_RENDERER_RESOURCE_COORDINATOR_H_
