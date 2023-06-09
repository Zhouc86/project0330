// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_SHARED_MEMORY_SECURITY_POLICY_H_
#define BASE_MEMORY_SHARED_MEMORY_SECURITY_POLICY_H_

#include <stddef.h>

#include "base/compiler_specific.h"

namespace base {

namespace subtle {
class PlatformSharedMemoryRegion;
}  // namespace subtle

// Helper to enforce a limit for the total amount of shared memory that can be
// mapped. This can help prevent an attacker from spraying the address space of
// a process with shared memory mappings to bypass ASLR. For more details, see
// https://googleprojectzero.blogspot.com/2019/04/virtually-unlimited-memory-escaping.html
class SharedMemorySecurityPolicy {
 private:
  friend class subtle::PlatformSharedMemoryRegion;
  friend class SharedMemoryMapping;

  // Checks that a mapping with |size| can be created. Returns false if there is
  // an overflow in internal calculations, or the max limit has been reached.
  static bool AcquireReservationForMapping(size_t size) WARN_UNUSED_RESULT;

  // Releases a reservation that was previously acquired.
  static void ReleaseReservationForMapping(size_t size);
};

}  // namespace base

#endif  // BASE_MEMORY_SHARED_MEMORY_SECURITY_POLICY_H_
