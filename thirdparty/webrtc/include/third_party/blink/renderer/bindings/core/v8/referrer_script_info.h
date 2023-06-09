// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_REFERRER_SCRIPT_INFO_H_
#define THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_REFERRER_SCRIPT_INFO_H_

#include "services/network/public/mojom/fetch_api.mojom-blink.h"
#include "services/network/public/mojom/referrer_policy.mojom-blink.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/loader/fetch/script_fetch_options.h"
#include "third_party/blink/renderer/platform/wtf/allocator/allocator.h"
#include "third_party/blink/renderer/platform/wtf/text/text_position.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "v8/include/v8.h"

namespace blink {

// ReferrerScriptInfo carries a copy of "referencing script's" info referenced
// in HTML Spec: "HostImportModuleDynamically" algorithm.
// https://html.spec.whatwg.org/C/#hostimportmoduledynamically(referencingscriptormodule,-specifier,-promisecapability)
class CORE_EXPORT ReferrerScriptInfo {
  STACK_ALLOCATED();

 public:
  enum class BaseUrlSource {
    kClassicScriptCORSSameOrigin,
    kClassicScriptCORSCrossOrigin,
    kOther
  };
  ReferrerScriptInfo() {}
  ReferrerScriptInfo(const KURL& base_url,
                     network::mojom::CredentialsMode credentials_mode,
                     const String& nonce,
                     ParserDisposition parser_state,
                     network::mojom::ReferrerPolicy referrer_policy,
                     BaseUrlSource base_url_source)
      : base_url_(base_url),
        credentials_mode_(credentials_mode),
        nonce_(nonce),
        parser_state_(parser_state),
        referrer_policy_(referrer_policy),
        base_url_source_(base_url_source) {}
  ReferrerScriptInfo(const KURL& base_url,
                     const ScriptFetchOptions& options,
                     BaseUrlSource base_url_source)
      : ReferrerScriptInfo(base_url,
                           options.CredentialsMode(),
                           options.Nonce(),
                           options.ParserState(),
                           options.GetReferrerPolicy(),
                           base_url_source) {}

  static ReferrerScriptInfo FromV8HostDefinedOptions(
      v8::Local<v8::Context>,
      v8::Local<v8::PrimitiveArray>);
  v8::Local<v8::PrimitiveArray> ToV8HostDefinedOptions(v8::Isolate*) const;

  const KURL& BaseURL() const { return base_url_; }
  network::mojom::CredentialsMode CredentialsMode() const {
    return credentials_mode_;
  }
  const String& Nonce() const { return nonce_; }
  ParserDisposition ParserState() const { return parser_state_; }
  network::mojom::ReferrerPolicy GetReferrerPolicy() const {
    return referrer_policy_;
  }
  BaseUrlSource GetBaseUrlSource() const { return base_url_source_; }

  bool IsDefaultValue() const {
    return base_url_.IsNull() &&
           credentials_mode_ == network::mojom::CredentialsMode::kSameOrigin &&
           nonce_.IsEmpty() && parser_state_ == kNotParserInserted &&
           base_url_source_ == BaseUrlSource::kOther;
  }

 private:
  // Spec: "referencing script's base URL"
  // https://html.spec.whatwg.org/C/#concept-script-base-url
  //
  // If base_url_.IsNull(), refer to ScriptOrigin::ResourceName() instead.
  // Note: This improves the chance of getting into the fast path in
  //       ToV8HostDefinedOptions().
  const KURL base_url_;

  // Spec: "referencing script's credentials mode"
  // The default value is "same-origin" per:
  // https://html.spec.whatwg.org/C/#default-classic-script-fetch-options
  const network::mojom::CredentialsMode credentials_mode_ =
      network::mojom::CredentialsMode::kSameOrigin;

  // Spec: "referencing script's cryptographic nonce"
  const String nonce_;

  // Spec: "referencing script's parser state"
  // The default value is "not-parser-inserted" per:
  // https://html.spec.whatwg.org/C/#default-classic-script-fetch-options
  const ParserDisposition parser_state_ = kNotParserInserted;

  // Spec: "referencing script's referrer policy"
  // The default value is "the empty string" per:
  // https://html.spec.whatwg.org/C/#default-classic-script-fetch-options
  const network::mojom::ReferrerPolicy referrer_policy_ =
      network::mojom::ReferrerPolicy::kDefault;

  // Temporary flag to collect UMA for crbug.com/1082086.
  const BaseUrlSource base_url_source_ = BaseUrlSource::kOther;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_REFERRER_SCRIPT_INFO_H_
