// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_LOADER_MODULESCRIPT_WORKER_MODULE_SCRIPT_FETCHER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_LOADER_MODULESCRIPT_WORKER_MODULE_SCRIPT_FETCHER_H_

#include "third_party/blink/renderer/core/loader/modulescript/module_script_fetcher.h"
#include "third_party/blink/renderer/platform/loader/fetch/url_loader/worker_main_script_loader.h"
#include "third_party/blink/renderer/platform/loader/fetch/url_loader/worker_main_script_loader_client.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"

namespace blink {

class WorkerGlobalScope;

// WorkerModuleScriptFetcher is an implementation of ModuleScriptFetcher
// interface for WebWorkers. This implements the custom "perform the fetch" hook
// defined in the HTML spec:
// https://html.spec.whatwg.org/C/#fetching-scripts-perform-fetch
// https://html.spec.whatwg.org/C/#worker-processing-model
class CORE_EXPORT WorkerModuleScriptFetcher final
    : public GarbageCollected<WorkerModuleScriptFetcher>,
      public ModuleScriptFetcher,
      public WorkerMainScriptLoaderClient {
 public:
  WorkerModuleScriptFetcher(WorkerGlobalScope*,
                            util::PassKey<ModuleScriptLoader>);

  // Implements ModuleScriptFetcher.
  void Fetch(FetchParameters&,
             ResourceFetcher*,
             ModuleGraphLevel,
             ModuleScriptFetcher::Client*) override;

  // Implements WorkerMainScriptLoaderClient, and these will be called for
  // dedicated workers (when PlzDedicatedWorker is enabled) and shared workers.
  void DidReceiveData(base::span<const char> span) override;
  void OnStartLoadingBody(const ResourceResponse& resource_response) override;
  void OnFinishedLoadingWorkerMainScript() override;
  void OnFailedLoadingWorkerMainScript() override;

  void Trace(Visitor*) const override;

 private:
  // Implements ResourceClient
  void NotifyFinished(Resource*) override;
  String DebugName() const override { return "WorkerModuleScriptFetcher"; }

  void NotifyClient(const KURL& request_url,
                    ModuleScriptCreationParams::ModuleType module_type,
                    const network::mojom::CredentialsMode credentials_mode,
                    const ParkableString& source_text,
                    const ResourceResponse& response,
                    SingleCachedMetadataHandler* cache_handler);

  const Member<WorkerGlobalScope> global_scope_;

  // These are used for dedicated workers (when PlzDedicatedWorker is enabled)
  // and shared workers.
  Member<WorkerMainScriptLoader> worker_main_script_loader_;
  std::unique_ptr<TextResourceDecoder> decoder_;
  StringBuilder source_text_;

  Member<ResourceFetcher> fetch_client_settings_object_fetcher_;
  Member<Client> client_;
  ModuleGraphLevel level_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_LOADER_MODULESCRIPT_WORKER_MODULE_SCRIPT_FETCHER_H_
