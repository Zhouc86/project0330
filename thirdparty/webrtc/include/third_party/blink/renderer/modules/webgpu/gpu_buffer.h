// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_WEBGPU_GPU_BUFFER_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_WEBGPU_GPU_BUFFER_H_

#include "base/optional.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/core/typed_arrays/array_buffer_view_helpers.h"
#include "third_party/blink/renderer/core/typed_arrays/flexible_array_buffer_view.h"
#include "third_party/blink/renderer/modules/webgpu/dawn_object.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

namespace blink {

class DOMArrayBuffer;
class GPUBufferDescriptor;
class ScriptPromiseResolver;

class GPUBuffer : public DawnObject<WGPUBuffer> {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static GPUBuffer* Create(GPUDevice* device,
                           const GPUBufferDescriptor* webgpu_desc);
  explicit GPUBuffer(GPUDevice* device,
                     uint64_t size,
                     bool mapped_at_creation,
                     WGPUBuffer buffer);
  ~GPUBuffer() override;

  void Trace(Visitor* visitor) const override;

  // gpu_buffer.idl
  ScriptPromise mapAsync(ScriptState* script_state,
                         uint32_t mode,
                         uint64_t offset,
                         ExceptionState& exception_state);
  ScriptPromise mapAsync(ScriptState* script_state,
                         uint32_t mode,
                         uint64_t offset,
                         uint64_t size,
                         ExceptionState& exception_state);
  DOMArrayBuffer* getMappedRange(uint64_t offset,
                                 ExceptionState& exception_state);
  DOMArrayBuffer* getMappedRange(uint64_t offset,
                                 uint64_t size,
                                 ExceptionState& exception_state);
  void unmap(ScriptState* script_state);
  void destroy(ScriptState* script_state);
  // TODO(crbug.com/877147): implement GPUBuffer.

 private:
  ScriptPromise MapAsyncImpl(ScriptState* script_state,
                             uint32_t mode,
                             uint64_t offset,
                             base::Optional<uint64_t> size,
                             ExceptionState& exception_state);
  DOMArrayBuffer* GetMappedRangeImpl(uint64_t offset,
                                     base::Optional<uint64_t> size,
                                     ExceptionState& exception_state);

  void OnMapAsyncCallback(ScriptPromiseResolver* resolver,
                          uint64_t map_start,
                          uint64_t map_end,
                          WGPUBufferMapAsyncStatus status);

  DOMArrayBuffer* CreateArrayBufferForMappedData(void* data,
                                                 size_t data_length);
  void ResetMappingState(ScriptState* script_state);

  uint64_t size_;

  // Holds onto any ArrayBuffers returned by getMappedRange, mapReadAsync, or
  // mapWriteAsync.
  HeapVector<Member<DOMArrayBuffer>> mapped_array_buffers_;

  uint64_t map_start_ = 0;
  uint64_t map_end_ = 0;
  // List of ranges currently returned by getMappedRange, to avoid overlaps.
  Vector<std::pair<size_t, size_t>> mapped_ranges_;

  DISALLOW_COPY_AND_ASSIGN(GPUBuffer);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_WEBGPU_GPU_BUFFER_H_
