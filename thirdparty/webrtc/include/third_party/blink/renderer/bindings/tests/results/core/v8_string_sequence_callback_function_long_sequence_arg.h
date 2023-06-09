// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/callback_function.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_BINDINGS_TESTS_RESULTS_CORE_V8_STRING_SEQUENCE_CALLBACK_FUNCTION_LONG_SEQUENCE_ARG_H_
#define THIRD_PARTY_BLINK_RENDERER_BINDINGS_TESTS_RESULTS_CORE_V8_STRING_SEQUENCE_CALLBACK_FUNCTION_LONG_SEQUENCE_ARG_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/bindings/callback_function_base.h"
#include "third_party/blink/renderer/platform/bindings/v8_value_or_script_wrappable_adapter.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"

namespace blink {

class CORE_EXPORT V8StringSequenceCallbackFunctionLongSequenceArg final : public CallbackFunctionBase {
 public:
  static V8StringSequenceCallbackFunctionLongSequenceArg* Create(v8::Local<v8::Function> callback_function) {
    return MakeGarbageCollected<V8StringSequenceCallbackFunctionLongSequenceArg>(callback_function);
  }

  explicit V8StringSequenceCallbackFunctionLongSequenceArg(v8::Local<v8::Function> callback_function)
      : CallbackFunctionBase(callback_function) {}
  ~V8StringSequenceCallbackFunctionLongSequenceArg() override = default;

  // NameClient overrides:
  const char* NameInHeapSnapshot() const override;

  // Performs "invoke".
  // https://heycam.github.io/webidl/#es-invoking-callback-functions
  v8::Maybe<Vector<String>> Invoke(bindings::V8ValueOrScriptWrappableAdapter callback_this_value, const Vector<int32_t>& arg) WARN_UNUSED_RESULT;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_BINDINGS_TESTS_RESULTS_CORE_V8_STRING_SEQUENCE_CALLBACK_FUNCTION_LONG_SEQUENCE_ARG_H_
