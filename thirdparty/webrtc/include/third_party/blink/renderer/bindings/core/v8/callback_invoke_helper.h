// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_CALLBACK_INVOKE_HELPER_H_
#define THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_CALLBACK_INVOKE_HELPER_H_

#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
#include "third_party/blink/renderer/platform/bindings/v8_value_or_script_wrappable_adapter.h"
#include "v8/include/v8.h"

namespace blink {

class CallbackFunctionBase;
class CallbackInterfaceBase;

namespace bindings {

enum class CallbackInvokeHelperMode {
  kDefault,
  kConstructorCall,
  kLegacyTreatNonObjectAsNull,
};

// This class helps implement the generated Blink-V8 bindings of IDL callback
// functions and IDL callback interfaces.  This class implements the following
// algorithms of Web IDL.
//
// https://heycam.github.io/webidl/#call-a-user-objects-operation
// 3.11. Callback interfaces
// To call a user object's operation
//
// https://heycam.github.io/webidl/#invoke-a-callback-function
// 3.12. Invoking callback functions
// To invoke a callback function type value
//
// https://heycam.github.io/webidl/#construct-a-callback-function
// 3.12. Invoking callback functions
// To construct a callback functions type value
template <class CallbackBase,
          CallbackInvokeHelperMode mode = CallbackInvokeHelperMode::kDefault>
class CallbackInvokeHelper final {
  STACK_ALLOCATED();

 public:
  CallbackInvokeHelper(CallbackBase* callback,
                       const char* class_like_name,
                       const char* property_name)
      : callback_(callback),
        class_like_name_(class_like_name),
        property_name_(property_name),
        // step: Prepare to run script with relevant settings.
        callback_relevant_context_scope_(
            callback->CallbackRelevantScriptState()),
        // step: Prepare to run a callback with stored settings.
        backup_incumbent_scope_(
            callback->IncumbentScriptState()->GetContext()) {}

  bool PrepareForCall(V8ValueOrScriptWrappableAdapter callback_this);

  bool Call(int argc, v8::Local<v8::Value>* argv);

  v8::Local<v8::Value> V8Result() { return result_; }

  template <typename IDLReturnType, typename ReturnType>
  v8::Maybe<ReturnType> Result() {
    DCHECK(!aborted_);
    ExceptionState exception_state(callback_->GetIsolate(),
                                   ExceptionState::kExecutionContext,
                                   class_like_name_, property_name_);
    auto&& result = NativeValueTraits<IDLReturnType>::NativeValue(
        callback_->GetIsolate(), result_, exception_state);
    if (exception_state.HadException())
      return v8::Nothing<ReturnType>();
    return v8::Just<ReturnType>(result);
  }

 private:
  bool Abort() {
    aborted_ = true;
    return false;
  }

  CallbackBase* callback_;
  const char* class_like_name_;
  const char* property_name_;
  v8::Local<v8::Function> function_;
  v8::Local<v8::Value> callback_this_;
  v8::Local<v8::Value> result_;
  bool aborted_ = false;

  ScriptState::Scope callback_relevant_context_scope_;
  v8::Context::BackupIncumbentScope backup_incumbent_scope_;
};

extern template class CORE_EXTERN_TEMPLATE_EXPORT
    CallbackInvokeHelper<CallbackFunctionBase>;
extern template class CORE_EXTERN_TEMPLATE_EXPORT
    CallbackInvokeHelper<CallbackFunctionBase,
                         CallbackInvokeHelperMode::kConstructorCall>;
extern template class CORE_EXTERN_TEMPLATE_EXPORT
    CallbackInvokeHelper<CallbackFunctionBase,
                         CallbackInvokeHelperMode::kLegacyTreatNonObjectAsNull>;
extern template class CORE_EXTERN_TEMPLATE_EXPORT
    CallbackInvokeHelper<CallbackInterfaceBase>;

}  // namespace bindings

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_CALLBACK_INVOKE_HELPER_H_
