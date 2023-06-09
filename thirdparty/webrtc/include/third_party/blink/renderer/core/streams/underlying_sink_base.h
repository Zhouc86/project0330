// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_STREAMS_UNDERLYING_SINK_BASE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_STREAMS_UNDERLYING_SINK_BASE_H_

#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/streams/writable_stream_default_controller.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/visitor.h"

namespace blink {

class ExceptionState;
class ScriptValue;
class ScriptState;

class CORE_EXPORT UnderlyingSinkBase : public ScriptWrappable {
  DEFINE_WRAPPERTYPEINFO();

 public:
  ~UnderlyingSinkBase() override = default;

  // We define non-virtual |start| and |write| which take ScriptValue for
  // |controller| and are called from IDL. Also we define virtual |start| and
  // |write| which take WritableStreamDefaultController.
  virtual ScriptPromise start(ScriptState*,
                              WritableStreamDefaultController*,
                              ExceptionState&) = 0;
  virtual ScriptPromise write(ScriptState*,
                              ScriptValue chunk,
                              WritableStreamDefaultController*,
                              ExceptionState&) = 0;
  virtual ScriptPromise close(ScriptState*, ExceptionState&) = 0;
  virtual ScriptPromise abort(ScriptState*,
                              ScriptValue reason,
                              ExceptionState&) = 0;

  ScriptPromise start(ScriptState* script_state,
                      ScriptValue controller,
                      ExceptionState& exception_state) {
    controller_ = WritableStreamDefaultController::From(controller);
    return start(script_state, controller_, exception_state);
  }
  ScriptPromise write(ScriptState* script_state,
                      ScriptValue chunk,
                      ScriptValue controller,
                      ExceptionState& exception_state) {
    DCHECK(controller_);
    return write(script_state, chunk, controller_, exception_state);
  }

  void Trace(Visitor* visitor) const override {
    visitor->Trace(controller_);
    ScriptWrappable::Trace(visitor);
  }

 protected:
  WritableStreamDefaultController* Controller() const { return controller_; }

 private:
  Member<WritableStreamDefaultController> controller_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_STREAMS_UNDERLYING_SINK_BASE_H_
