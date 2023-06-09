// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_PAYMENTS_CAN_MAKE_PAYMENT_EVENT_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_PAYMENTS_CAN_MAKE_PAYMENT_EVENT_H_

#include "base/macros.h"
#include "third_party/blink/renderer/bindings/core/v8/script_value.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_can_make_payment_event_init.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_payment_details_modifier.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_payment_method_data.h"
#include "third_party/blink/renderer/modules/event_modules.h"
#include "third_party/blink/renderer/modules/service_worker/extendable_event.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace WTF {
class AtomicString;
}

namespace blink {

class CanMakePaymentRespondWithObserver;
class ScriptState;

class MODULES_EXPORT CanMakePaymentEvent final : public ExtendableEvent {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static CanMakePaymentEvent* Create(const AtomicString& type,
                                     const CanMakePaymentEventInit*);
  static CanMakePaymentEvent* Create(const AtomicString& type,
                                     const CanMakePaymentEventInit*,
                                     CanMakePaymentRespondWithObserver*,
                                     WaitUntilObserver*);

  CanMakePaymentEvent(const AtomicString& type,
                      const CanMakePaymentEventInit*,
                      CanMakePaymentRespondWithObserver*,
                      WaitUntilObserver*);
  ~CanMakePaymentEvent() override;

  const AtomicString& InterfaceName() const override;

  const String& topOrigin() const;
  const String& paymentRequestOrigin() const;
  const HeapVector<Member<PaymentMethodData>>& methodData() const;
  const HeapVector<Member<PaymentDetailsModifier>>& modifiers() const;

  void respondWith(ScriptState*, ScriptPromise, ExceptionState&);

  const String& currency() const;
  void respondWithMinimalUI(ScriptState*, ScriptPromise, ExceptionState&);

  void Trace(Visitor*) const override;

 private:
  void RespondToCanMakePaymentEvent(ScriptState*,
                                    ScriptPromise,
                                    ExceptionState&,
                                    bool is_minimal_ui);

  String top_origin_;
  String payment_request_origin_;
  HeapVector<Member<PaymentMethodData>> method_data_;
  HeapVector<Member<PaymentDetailsModifier>> modifiers_;
  String currency_;

  Member<CanMakePaymentRespondWithObserver> observer_;

  DISALLOW_COPY_AND_ASSIGN(CanMakePaymentEvent);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_PAYMENTS_CAN_MAKE_PAYMENT_EVENT_H_
