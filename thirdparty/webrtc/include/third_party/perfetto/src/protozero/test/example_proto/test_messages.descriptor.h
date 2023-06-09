/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_PROTOZERO_TEST_EXAMPLE_PROTO_TEST_MESSAGES_DESCRIPTOR_H_
#define SRC_PROTOZERO_TEST_EXAMPLE_PROTO_TEST_MESSAGES_DESCRIPTOR_H_

#include <stddef.h>
#include <stdint.h>

#include <array>

// This file was autogenerated by tools/gen_binary_descriptors. Do not edit.

// SHA1(tools/gen_binary_descriptors)
// e5c244903aa00cad06faf3d126918306a7fe811e
// SHA1(src/protozero/test/example_proto/test_messages.proto)
// 6db2d291c87441f363d941410a1c326078566aa0

// This is the proto TestMessages encoded as a ProtoFileDescriptor to allow
// for reflection without libprotobuf full/non-lite protos.

namespace perfetto {

constexpr std::array<uint8_t, 4235> kTestMessagesDescriptor{
    {0x0a, 0x62, 0x0a, 0x33, 0x73, 0x72, 0x63, 0x2f, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x65,
     0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x5f, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x2f, 0x75, 0x70, 0x70, 0x65, 0x72, 0x5f, 0x69, 0x6d, 0x70, 0x6f, 0x72,
     0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x15, 0x70, 0x72, 0x6f,
     0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e,
     0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x22, 0x14, 0x0a, 0x12, 0x54, 0x72,
     0x69, 0x63, 0x6b, 0x79, 0x50, 0x75, 0x62, 0x6c, 0x69, 0x63, 0x49, 0x6d,
     0x70, 0x6f, 0x72, 0x74, 0x0a, 0xc8, 0x01, 0x0a, 0x41, 0x73, 0x72, 0x63,
     0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2f, 0x74,
     0x65, 0x73, 0x74, 0x2f, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x5f,
     0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2f, 0x6c, 0x69, 0x62, 0x72, 0x61, 0x72,
     0x79, 0x5f, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x61, 0x6c, 0x73, 0x2f,
     0x67, 0x61, 0x6c, 0x61, 0x78, 0x69, 0x65, 0x73, 0x2e, 0x70, 0x72, 0x6f,
     0x74, 0x6f, 0x12, 0x15, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72,
     0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x73, 0x1a, 0x33, 0x73, 0x72, 0x63, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x7a, 0x65, 0x72, 0x6f, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x65, 0x78,
     0x61, 0x6d, 0x70, 0x6c, 0x65, 0x5f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2f,
     0x75, 0x70, 0x70, 0x65, 0x72, 0x5f, 0x69, 0x6d, 0x70, 0x6f, 0x72, 0x74,
     0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2a, 0x35, 0x0a, 0x06, 0x47, 0x61,
     0x6c, 0x61, 0x78, 0x79, 0x12, 0x0d, 0x0a, 0x09, 0x4d, 0x49, 0x4c, 0x4b,
     0x59, 0x5f, 0x57, 0x41, 0x59, 0x10, 0x01, 0x12, 0x0d, 0x0a, 0x09, 0x41,
     0x4e, 0x44, 0x52, 0x4f, 0x4d, 0x45, 0x44, 0x41, 0x10, 0x02, 0x12, 0x0d,
     0x0a, 0x09, 0x53, 0x55, 0x4e, 0x46, 0x4c, 0x4f, 0x57, 0x45, 0x52, 0x10,
     0x03, 0x50, 0x00, 0x0a, 0xb0, 0x03, 0x0a, 0x2e, 0x73, 0x72, 0x63, 0x2f,
     0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2f, 0x74, 0x65,
     0x73, 0x74, 0x2f, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x5f, 0x70,
     0x72, 0x6f, 0x74, 0x6f, 0x2f, 0x6c, 0x69, 0x62, 0x72, 0x61, 0x72, 0x79,
     0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x15, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70,
     0x72, 0x6f, 0x74, 0x6f, 0x73, 0x1a, 0x41, 0x73, 0x72, 0x63, 0x2f, 0x70,
     0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2f, 0x74, 0x65, 0x73,
     0x74, 0x2f, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x5f, 0x70, 0x72,
     0x6f, 0x74, 0x6f, 0x2f, 0x6c, 0x69, 0x62, 0x72, 0x61, 0x72, 0x79, 0x5f,
     0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x61, 0x6c, 0x73, 0x2f, 0x67, 0x61,
     0x6c, 0x61, 0x78, 0x69, 0x65, 0x73, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x22, 0xa1, 0x02, 0x0a, 0x14, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x67, 0x61,
     0x6c, 0x61, 0x63, 0x74, 0x69, 0x63, 0x4d, 0x65, 0x73, 0x73, 0x61, 0x67,
     0x65, 0x12, 0x42, 0x0a, 0x0d, 0x6f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x5f,
     0x67, 0x61, 0x6c, 0x61, 0x78, 0x79, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0e,
     0x32, 0x1d, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f,
     0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73,
     0x2e, 0x47, 0x61, 0x6c, 0x61, 0x78, 0x79, 0x52, 0x0c, 0x6f, 0x72, 0x69,
     0x67, 0x69, 0x6e, 0x47, 0x61, 0x6c, 0x61, 0x78, 0x79, 0x12, 0x23, 0x0a,
     0x0d, 0x6f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x5f, 0x70, 0x6c, 0x61, 0x6e,
     0x65, 0x74, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0c, 0x6f, 0x72,
     0x69, 0x67, 0x69, 0x6e, 0x50, 0x6c, 0x61, 0x6e, 0x65, 0x74, 0x12, 0x4c,
     0x0a, 0x12, 0x64, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x61, 0x74, 0x69, 0x6f,
     0x6e, 0x5f, 0x67, 0x61, 0x6c, 0x61, 0x78, 0x79, 0x18, 0x03, 0x20, 0x01,
     0x28, 0x0e, 0x32, 0x1d, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65,
     0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x73, 0x2e, 0x47, 0x61, 0x6c, 0x61, 0x78, 0x79, 0x52, 0x11, 0x64,
     0x65, 0x73, 0x74, 0x69, 0x6e, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x47, 0x61,
     0x6c, 0x61, 0x78, 0x79, 0x12, 0x2d, 0x0a, 0x12, 0x64, 0x65, 0x73, 0x74,
     0x69, 0x6e, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x5f, 0x70, 0x6c, 0x61, 0x6e,
     0x65, 0x74, 0x18, 0x04, 0x20, 0x01, 0x28, 0x09, 0x52, 0x11, 0x64, 0x65,
     0x73, 0x74, 0x69, 0x6e, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x50, 0x6c, 0x61,
     0x6e, 0x65, 0x74, 0x12, 0x23, 0x0a, 0x0d, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x18, 0x05, 0x20, 0x01,
     0x28, 0x0c, 0x52, 0x0c, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x4d, 0x65, 0x73,
     0x73, 0x61, 0x67, 0x65, 0x50, 0x00, 0x0a, 0xa6, 0x1b, 0x0a, 0x34, 0x73,
     0x72, 0x63, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f,
     0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c,
     0x65, 0x5f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x2f, 0x74, 0x65, 0x73, 0x74,
     0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x73, 0x2e, 0x70, 0x72,
     0x6f, 0x74, 0x6f, 0x12, 0x15, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65,
     0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x73, 0x1a, 0x2e, 0x73, 0x72, 0x63, 0x2f, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x65,
     0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x5f, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x2f, 0x6c, 0x69, 0x62, 0x72, 0x61, 0x72, 0x79, 0x2e, 0x70, 0x72, 0x6f,
     0x74, 0x6f, 0x22, 0x81, 0x01, 0x0a, 0x13, 0x54, 0x72, 0x61, 0x6e, 0x73,
     0x67, 0x61, 0x6c, 0x61, 0x63, 0x74, 0x69, 0x63, 0x50, 0x61, 0x72, 0x63,
     0x65, 0x6c, 0x12, 0x45, 0x0a, 0x07, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67,
     0x65, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x2b, 0x2e, 0x70, 0x72,
     0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74,
     0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x54, 0x72, 0x61, 0x6e,
     0x73, 0x67, 0x61, 0x6c, 0x61, 0x63, 0x74, 0x69, 0x63, 0x4d, 0x65, 0x73,
     0x73, 0x61, 0x67, 0x65, 0x52, 0x07, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67,
     0x65, 0x12, 0x23, 0x0a, 0x0d, 0x74, 0x72, 0x61, 0x63, 0x6b, 0x69, 0x6e,
     0x67, 0x5f, 0x63, 0x6f, 0x64, 0x65, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09,
     0x52, 0x0c, 0x74, 0x72, 0x61, 0x63, 0x6b, 0x69, 0x6e, 0x67, 0x43, 0x6f,
     0x64, 0x65, 0x22, 0xbb, 0x07, 0x0a, 0x0a, 0x45, 0x76, 0x65, 0x72, 0x79,
     0x46, 0x69, 0x65, 0x6c, 0x64, 0x12, 0x1f, 0x0a, 0x0b, 0x66, 0x69, 0x65,
     0x6c, 0x64, 0x5f, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x18, 0x01, 0x20, 0x01,
     0x28, 0x05, 0x52, 0x0a, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x49, 0x6e, 0x74,
     0x33, 0x32, 0x12, 0x1f, 0x0a, 0x0b, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f,
     0x69, 0x6e, 0x74, 0x36, 0x34, 0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52,
     0x0a, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x49, 0x6e, 0x74, 0x36, 0x34, 0x12,
     0x21, 0x0a, 0x0c, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x75, 0x69, 0x6e,
     0x74, 0x33, 0x32, 0x18, 0x03, 0x20, 0x01, 0x28, 0x0d, 0x52, 0x0b, 0x66,
     0x69, 0x65, 0x6c, 0x64, 0x55, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x12, 0x21,
     0x0a, 0x0c, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x75, 0x69, 0x6e, 0x74,
     0x36, 0x34, 0x18, 0x04, 0x20, 0x01, 0x28, 0x04, 0x52, 0x0b, 0x66, 0x69,
     0x65, 0x6c, 0x64, 0x55, 0x69, 0x6e, 0x74, 0x36, 0x34, 0x12, 0x21, 0x0a,
     0x0c, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x73, 0x69, 0x6e, 0x74, 0x33,
     0x32, 0x18, 0x05, 0x20, 0x01, 0x28, 0x11, 0x52, 0x0b, 0x66, 0x69, 0x65,
     0x6c, 0x64, 0x53, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x12, 0x21, 0x0a, 0x0c,
     0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x73, 0x69, 0x6e, 0x74, 0x36, 0x34,
     0x18, 0x06, 0x20, 0x01, 0x28, 0x12, 0x52, 0x0b, 0x66, 0x69, 0x65, 0x6c,
     0x64, 0x53, 0x69, 0x6e, 0x74, 0x36, 0x34, 0x12, 0x23, 0x0a, 0x0d, 0x66,
     0x69, 0x65, 0x6c, 0x64, 0x5f, 0x66, 0x69, 0x78, 0x65, 0x64, 0x33, 0x32,
     0x18, 0x07, 0x20, 0x01, 0x28, 0x07, 0x52, 0x0c, 0x66, 0x69, 0x65, 0x6c,
     0x64, 0x46, 0x69, 0x78, 0x65, 0x64, 0x33, 0x32, 0x12, 0x23, 0x0a, 0x0d,
     0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x66, 0x69, 0x78, 0x65, 0x64, 0x36,
     0x34, 0x18, 0x08, 0x20, 0x01, 0x28, 0x06, 0x52, 0x0c, 0x66, 0x69, 0x65,
     0x6c, 0x64, 0x46, 0x69, 0x78, 0x65, 0x64, 0x36, 0x34, 0x12, 0x25, 0x0a,
     0x0e, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x73, 0x66, 0x69, 0x78, 0x65,
     0x64, 0x33, 0x32, 0x18, 0x09, 0x20, 0x01, 0x28, 0x0f, 0x52, 0x0d, 0x66,
     0x69, 0x65, 0x6c, 0x64, 0x53, 0x66, 0x69, 0x78, 0x65, 0x64, 0x33, 0x32,
     0x12, 0x25, 0x0a, 0x0e, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x73, 0x66,
     0x69, 0x78, 0x65, 0x64, 0x36, 0x34, 0x18, 0x0a, 0x20, 0x01, 0x28, 0x10,
     0x52, 0x0d, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x53, 0x66, 0x69, 0x78, 0x65,
     0x64, 0x36, 0x34, 0x12, 0x1f, 0x0a, 0x0b, 0x66, 0x69, 0x65, 0x6c, 0x64,
     0x5f, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x18, 0x0b, 0x20, 0x01, 0x28, 0x02,
     0x52, 0x0a, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x46, 0x6c, 0x6f, 0x61, 0x74,
     0x12, 0x21, 0x0a, 0x0c, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x64, 0x6f,
     0x75, 0x62, 0x6c, 0x65, 0x18, 0x0c, 0x20, 0x01, 0x28, 0x01, 0x52, 0x0b,
     0x66, 0x69, 0x65, 0x6c, 0x64, 0x44, 0x6f, 0x75, 0x62, 0x6c, 0x65, 0x12,
     0x1d, 0x0a, 0x0a, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x62, 0x6f, 0x6f,
     0x6c, 0x18, 0x0d, 0x20, 0x01, 0x28, 0x08, 0x52, 0x09, 0x66, 0x69, 0x65,
     0x6c, 0x64, 0x42, 0x6f, 0x6f, 0x6c, 0x12, 0x44, 0x0a, 0x0c, 0x66, 0x69,
     0x65, 0x6c, 0x64, 0x5f, 0x6e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x18, 0x0e,
     0x20, 0x03, 0x28, 0x0b, 0x32, 0x21, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72,
     0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x45, 0x76, 0x65, 0x72, 0x79, 0x46, 0x69,
     0x65, 0x6c, 0x64, 0x52, 0x0b, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x4e, 0x65,
     0x73, 0x74, 0x65, 0x64, 0x12, 0x3f, 0x0a, 0x0a, 0x73, 0x6d, 0x61, 0x6c,
     0x6c, 0x5f, 0x65, 0x6e, 0x75, 0x6d, 0x18, 0x33, 0x20, 0x01, 0x28, 0x0e,
     0x32, 0x20, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f,
     0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73,
     0x2e, 0x53, 0x6d, 0x61, 0x6c, 0x6c, 0x45, 0x6e, 0x75, 0x6d, 0x52, 0x09,
     0x73, 0x6d, 0x61, 0x6c, 0x6c, 0x45, 0x6e, 0x75, 0x6d, 0x12, 0x42, 0x0a,
     0x0b, 0x73, 0x69, 0x67, 0x6e, 0x65, 0x64, 0x5f, 0x65, 0x6e, 0x75, 0x6d,
     0x18, 0x34, 0x20, 0x01, 0x28, 0x0e, 0x32, 0x21, 0x2e, 0x70, 0x72, 0x6f,
     0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e,
     0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x53, 0x69, 0x67, 0x6e, 0x65,
     0x64, 0x45, 0x6e, 0x75, 0x6d, 0x52, 0x0a, 0x73, 0x69, 0x67, 0x6e, 0x65,
     0x64, 0x45, 0x6e, 0x75, 0x6d, 0x12, 0x39, 0x0a, 0x08, 0x62, 0x69, 0x67,
     0x5f, 0x65, 0x6e, 0x75, 0x6d, 0x18, 0x35, 0x20, 0x01, 0x28, 0x0e, 0x32,
     0x1e, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e,
     0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e,
     0x42, 0x69, 0x67, 0x45, 0x6e, 0x75, 0x6d, 0x52, 0x07, 0x62, 0x69, 0x67,
     0x45, 0x6e, 0x75, 0x6d, 0x12, 0x22, 0x0a, 0x0c, 0x66, 0x69, 0x65, 0x6c,
     0x64, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x18, 0xf4, 0x03, 0x20,
     0x01, 0x28, 0x09, 0x52, 0x0b, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x53, 0x74,
     0x72, 0x69, 0x6e, 0x67, 0x12, 0x20, 0x0a, 0x0b, 0x66, 0x69, 0x65, 0x6c,
     0x64, 0x5f, 0x62, 0x79, 0x74, 0x65, 0x73, 0x18, 0xf9, 0x03, 0x20, 0x01,
     0x28, 0x0c, 0x52, 0x0a, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x42, 0x79, 0x74,
     0x65, 0x73, 0x12, 0x4e, 0x0a, 0x0b, 0x6e, 0x65, 0x73, 0x74, 0x65, 0x64,
     0x5f, 0x65, 0x6e, 0x75, 0x6d, 0x18, 0xd8, 0x04, 0x20, 0x01, 0x28, 0x0e,
     0x32, 0x2c, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f,
     0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73,
     0x2e, 0x45, 0x76, 0x65, 0x72, 0x79, 0x46, 0x69, 0x65, 0x6c, 0x64, 0x2e,
     0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x45, 0x6e, 0x75, 0x6d, 0x52, 0x0a,
     0x6e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x45, 0x6e, 0x75, 0x6d, 0x12, 0x26,
     0x0a, 0x0e, 0x72, 0x65, 0x70, 0x65, 0x61, 0x74, 0x65, 0x64, 0x5f, 0x69,
     0x6e, 0x74, 0x33, 0x32, 0x18, 0xe7, 0x07, 0x20, 0x03, 0x28, 0x05, 0x52,
     0x0d, 0x72, 0x65, 0x70, 0x65, 0x61, 0x74, 0x65, 0x64, 0x49, 0x6e, 0x74,
     0x33, 0x32, 0x22, 0x20, 0x0a, 0x0a, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64,
     0x45, 0x6e, 0x75, 0x6d, 0x12, 0x08, 0x0a, 0x04, 0x50, 0x49, 0x4e, 0x47,
     0x10, 0x01, 0x12, 0x08, 0x0a, 0x04, 0x50, 0x4f, 0x4e, 0x47, 0x10, 0x02,
     0x22, 0x9b, 0x02, 0x0a, 0x07, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x41,
     0x12, 0x45, 0x0a, 0x0a, 0x72, 0x65, 0x70, 0x65, 0x61, 0x74, 0x65, 0x64,
     0x5f, 0x61, 0x18, 0x02, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x26, 0x2e, 0x70,
     0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73,
     0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x4e, 0x65, 0x73,
     0x74, 0x65, 0x64, 0x41, 0x2e, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x42,
     0x52, 0x09, 0x72, 0x65, 0x70, 0x65, 0x61, 0x74, 0x65, 0x64, 0x41, 0x12,
     0x51, 0x0a, 0x0c, 0x73, 0x75, 0x70, 0x65, 0x72, 0x5f, 0x6e, 0x65, 0x73,
     0x74, 0x65, 0x64, 0x18, 0x03, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x2e, 0x2e,
     0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65,
     0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x4e, 0x65,
     0x73, 0x74, 0x65, 0x64, 0x41, 0x2e, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64,
     0x42, 0x2e, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x43, 0x52, 0x0b, 0x73,
     0x75, 0x70, 0x65, 0x72, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x1a, 0x76,
     0x0a, 0x07, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x42, 0x12, 0x47, 0x0a,
     0x07, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x5f, 0x62, 0x18, 0x01, 0x20, 0x01,
     0x28, 0x0b, 0x32, 0x2e, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65,
     0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x73, 0x2e, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x41, 0x2e, 0x4e,
     0x65, 0x73, 0x74, 0x65, 0x64, 0x42, 0x2e, 0x4e, 0x65, 0x73, 0x74, 0x65,
     0x64, 0x43, 0x52, 0x06, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x42, 0x1a, 0x22,
     0x0a, 0x07, 0x4e, 0x65, 0x73, 0x74, 0x65, 0x64, 0x43, 0x12, 0x17, 0x0a,
     0x07, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x5f, 0x63, 0x18, 0x01, 0x20, 0x01,
     0x28, 0x05, 0x52, 0x06, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x43, 0x22, 0x80,
     0x02, 0x0a, 0x0f, 0x43, 0x61, 0x6d, 0x65, 0x6c, 0x43, 0x61, 0x73, 0x65,
     0x46, 0x69, 0x65, 0x6c, 0x64, 0x73, 0x12, 0x1e, 0x0a, 0x0b, 0x66, 0x6f,
     0x6f, 0x5f, 0x62, 0x61, 0x72, 0x5f, 0x62, 0x61, 0x7a, 0x18, 0x01, 0x20,
     0x01, 0x28, 0x08, 0x52, 0x09, 0x66, 0x6f, 0x6f, 0x42, 0x61, 0x72, 0x42,
     0x61, 0x7a, 0x12, 0x16, 0x0a, 0x06, 0x62, 0x61, 0x72, 0x42, 0x61, 0x7a,
     0x18, 0x02, 0x20, 0x01, 0x28, 0x08, 0x52, 0x06, 0x62, 0x61, 0x72, 0x42,
     0x61, 0x7a, 0x12, 0x16, 0x0a, 0x06, 0x4d, 0x6f, 0x6f, 0x4d, 0x6f, 0x6f,
     0x18, 0x03, 0x20, 0x01, 0x28, 0x08, 0x52, 0x06, 0x4d, 0x6f, 0x6f, 0x4d,
     0x6f, 0x6f, 0x12, 0x1e, 0x0a, 0x0a, 0x55, 0x52, 0x4c, 0x45, 0x6e, 0x63,
     0x6f, 0x64, 0x65, 0x72, 0x18, 0x04, 0x20, 0x01, 0x28, 0x08, 0x52, 0x0a,
     0x55, 0x52, 0x4c, 0x45, 0x6e, 0x63, 0x6f, 0x64, 0x65, 0x72, 0x12, 0x12,
     0x0a, 0x04, 0x58, 0x4d, 0x61, 0x70, 0x18, 0x05, 0x20, 0x01, 0x28, 0x08,
     0x52, 0x04, 0x58, 0x4d, 0x61, 0x70, 0x12, 0x21, 0x0a, 0x0d, 0x55, 0x72,
     0x4c, 0x45, 0x5f, 0x6e, 0x63, 0x6f, 0x5f, 0x5f, 0x64, 0x65, 0x72, 0x18,
     0x06, 0x20, 0x01, 0x28, 0x08, 0x52, 0x0a, 0x55, 0x72, 0x4c, 0x45, 0x4e,
     0x63, 0x6f, 0x44, 0x65, 0x72, 0x12, 0x1a, 0x0a, 0x09, 0x5f, 0x5f, 0x62,
     0x69, 0x67, 0x42, 0x61, 0x6e, 0x67, 0x18, 0x07, 0x20, 0x01, 0x28, 0x08,
     0x52, 0x07, 0x42, 0x69, 0x67, 0x42, 0x61, 0x6e, 0x67, 0x12, 0x0e, 0x0a,
     0x02, 0x55, 0x32, 0x18, 0x08, 0x20, 0x01, 0x28, 0x08, 0x52, 0x02, 0x55,
     0x32, 0x12, 0x1a, 0x0a, 0x09, 0x62, 0x61, 0x6e, 0x67, 0x42, 0x69, 0x67,
     0x5f, 0x5f, 0x18, 0x09, 0x20, 0x01, 0x28, 0x08, 0x52, 0x07, 0x62, 0x61,
     0x6e, 0x67, 0x42, 0x69, 0x67, 0x22, 0x8f, 0x01, 0x0a, 0x14, 0x50, 0x61,
     0x63, 0x6b, 0x65, 0x64, 0x52, 0x65, 0x70, 0x65, 0x61, 0x74, 0x65, 0x64,
     0x46, 0x69, 0x65, 0x6c, 0x64, 0x73, 0x12, 0x23, 0x0a, 0x0b, 0x66, 0x69,
     0x65, 0x6c, 0x64, 0x5f, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x18, 0x01, 0x20,
     0x03, 0x28, 0x05, 0x42, 0x02, 0x10, 0x01, 0x52, 0x0a, 0x66, 0x69, 0x65,
     0x6c, 0x64, 0x49, 0x6e, 0x74, 0x33, 0x32, 0x12, 0x27, 0x0a, 0x0d, 0x66,
     0x69, 0x65, 0x6c, 0x64, 0x5f, 0x66, 0x69, 0x78, 0x65, 0x64, 0x33, 0x32,
     0x18, 0x02, 0x20, 0x03, 0x28, 0x07, 0x42, 0x02, 0x10, 0x01, 0x52, 0x0c,
     0x66, 0x69, 0x65, 0x6c, 0x64, 0x46, 0x69, 0x78, 0x65, 0x64, 0x33, 0x32,
     0x12, 0x29, 0x0a, 0x0e, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x5f, 0x73, 0x66,
     0x69, 0x78, 0x65, 0x64, 0x36, 0x34, 0x18, 0x03, 0x20, 0x03, 0x28, 0x10,
     0x42, 0x02, 0x10, 0x01, 0x52, 0x0d, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x53,
     0x66, 0x69, 0x78, 0x65, 0x64, 0x36, 0x34, 0x22, 0x82, 0x04, 0x0a, 0x11,
     0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x69,
     0x6e, 0x67, 0x5f, 0x56, 0x31, 0x12, 0x19, 0x0a, 0x08, 0x72, 0x6f, 0x6f,
     0x74, 0x5f, 0x69, 0x6e, 0x74, 0x18, 0x01, 0x20, 0x01, 0x28, 0x05, 0x52,
     0x07, 0x72, 0x6f, 0x6f, 0x74, 0x49, 0x6e, 0x74, 0x12, 0x47, 0x0a, 0x05,
     0x65, 0x6e, 0x75, 0x6d, 0x7a, 0x18, 0x02, 0x20, 0x03, 0x28, 0x0e, 0x32,
     0x31, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e,
     0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e,
     0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x69,
     0x6e, 0x67, 0x5f, 0x56, 0x31, 0x2e, 0x45, 0x6e, 0x75, 0x6d, 0x7a, 0x5f,
     0x56, 0x31, 0x52, 0x05, 0x65, 0x6e, 0x75, 0x6d, 0x7a, 0x12, 0x1f, 0x0a,
     0x0b, 0x72, 0x6f, 0x6f, 0x74, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67,
     0x18, 0x03, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0a, 0x72, 0x6f, 0x6f, 0x74,
     0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x12, 0x1d, 0x0a, 0x0a, 0x72, 0x65,
     0x70, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x18, 0x04, 0x20, 0x03,
     0x28, 0x09, 0x52, 0x09, 0x72, 0x65, 0x70, 0x53, 0x74, 0x72, 0x69, 0x6e,
     0x67, 0x12, 0x44, 0x0a, 0x04, 0x73, 0x75, 0x62, 0x31, 0x18, 0x05, 0x20,
     0x01, 0x28, 0x0b, 0x32, 0x30, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a,
     0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f,
     0x74, 0x6f, 0x73, 0x2e, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73,
     0x69, 0x6f, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x56, 0x31, 0x2e, 0x53, 0x75,
     0x62, 0x31, 0x5f, 0x56, 0x31, 0x52, 0x04, 0x73, 0x75, 0x62, 0x31, 0x12,
     0x4b, 0x0a, 0x08, 0x73, 0x75, 0x62, 0x31, 0x5f, 0x72, 0x65, 0x70, 0x18,
     0x06, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x30, 0x2e, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70,
     0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65,
     0x72, 0x73, 0x69, 0x6f, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x56, 0x31, 0x2e,
     0x53, 0x75, 0x62, 0x31, 0x5f, 0x56, 0x31, 0x52, 0x07, 0x73, 0x75, 0x62,
     0x31, 0x52, 0x65, 0x70, 0x12, 0x51, 0x0a, 0x09, 0x73, 0x75, 0x62, 0x31,
     0x5f, 0x6c, 0x61, 0x7a, 0x79, 0x18, 0x07, 0x20, 0x01, 0x28, 0x0b, 0x32,
     0x30, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e,
     0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e,
     0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x69,
     0x6e, 0x67, 0x5f, 0x56, 0x31, 0x2e, 0x53, 0x75, 0x62, 0x31, 0x5f, 0x56,
     0x31, 0x42, 0x02, 0x28, 0x01, 0x52, 0x08, 0x73, 0x75, 0x62, 0x31, 0x4c,
     0x61, 0x7a, 0x79, 0x1a, 0x45, 0x0a, 0x07, 0x53, 0x75, 0x62, 0x31, 0x5f,
     0x56, 0x31, 0x12, 0x19, 0x0a, 0x08, 0x73, 0x75, 0x62, 0x31, 0x5f, 0x69,
     0x6e, 0x74, 0x18, 0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x07, 0x73, 0x75,
     0x62, 0x31, 0x49, 0x6e, 0x74, 0x12, 0x1f, 0x0a, 0x0b, 0x73, 0x75, 0x62,
     0x31, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x18, 0x02, 0x20, 0x01,
     0x28, 0x09, 0x52, 0x0a, 0x73, 0x75, 0x62, 0x31, 0x53, 0x74, 0x72, 0x69,
     0x6e, 0x67, 0x22, 0x1c, 0x0a, 0x08, 0x45, 0x6e, 0x75, 0x6d, 0x7a, 0x5f,
     0x56, 0x31, 0x12, 0x07, 0x0a, 0x03, 0x4f, 0x4e, 0x45, 0x10, 0x01, 0x12,
     0x07, 0x0a, 0x03, 0x54, 0x57, 0x4f, 0x10, 0x02, 0x22, 0xa4, 0x07, 0x0a,
     0x11, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e,
     0x69, 0x6e, 0x67, 0x5f, 0x56, 0x32, 0x12, 0x19, 0x0a, 0x08, 0x72, 0x6f,
     0x6f, 0x74, 0x5f, 0x69, 0x6e, 0x74, 0x18, 0x01, 0x20, 0x01, 0x28, 0x05,
     0x52, 0x07, 0x72, 0x6f, 0x6f, 0x74, 0x49, 0x6e, 0x74, 0x12, 0x47, 0x0a,
     0x05, 0x65, 0x6e, 0x75, 0x6d, 0x7a, 0x18, 0x02, 0x20, 0x03, 0x28, 0x0e,
     0x32, 0x31, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f,
     0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73,
     0x2e, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e,
     0x69, 0x6e, 0x67, 0x5f, 0x56, 0x32, 0x2e, 0x45, 0x6e, 0x75, 0x6d, 0x7a,
     0x5f, 0x56, 0x32, 0x52, 0x05, 0x65, 0x6e, 0x75, 0x6d, 0x7a, 0x12, 0x1f,
     0x0a, 0x0b, 0x72, 0x6f, 0x6f, 0x74, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e,
     0x67, 0x18, 0x03, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0a, 0x72, 0x6f, 0x6f,
     0x74, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x12, 0x1d, 0x0a, 0x0a, 0x72,
     0x65, 0x70, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x18, 0x04, 0x20,
     0x03, 0x28, 0x09, 0x52, 0x09, 0x72, 0x65, 0x70, 0x53, 0x74, 0x72, 0x69,
     0x6e, 0x67, 0x12, 0x44, 0x0a, 0x04, 0x73, 0x75, 0x62, 0x31, 0x18, 0x05,
     0x20, 0x01, 0x28, 0x0b, 0x32, 0x30, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72,
     0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72,
     0x73, 0x69, 0x6f, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x56, 0x32, 0x2e, 0x53,
     0x75, 0x62, 0x31, 0x5f, 0x56, 0x32, 0x52, 0x04, 0x73, 0x75, 0x62, 0x31,
     0x12, 0x4b, 0x0a, 0x08, 0x73, 0x75, 0x62, 0x31, 0x5f, 0x72, 0x65, 0x70,
     0x18, 0x06, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x30, 0x2e, 0x70, 0x72, 0x6f,
     0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e,
     0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x54, 0x65, 0x73, 0x74, 0x56,
     0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x56, 0x32,
     0x2e, 0x53, 0x75, 0x62, 0x31, 0x5f, 0x56, 0x32, 0x52, 0x07, 0x73, 0x75,
     0x62, 0x31, 0x52, 0x65, 0x70, 0x12, 0x51, 0x0a, 0x09, 0x73, 0x75, 0x62,
     0x31, 0x5f, 0x6c, 0x61, 0x7a, 0x79, 0x18, 0x07, 0x20, 0x01, 0x28, 0x0b,
     0x32, 0x30, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f,
     0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73,
     0x2e, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e,
     0x69, 0x6e, 0x67, 0x5f, 0x56, 0x32, 0x2e, 0x53, 0x75, 0x62, 0x31, 0x5f,
     0x56, 0x32, 0x42, 0x02, 0x28, 0x01, 0x52, 0x08, 0x73, 0x75, 0x62, 0x31,
     0x4c, 0x61, 0x7a, 0x79, 0x12, 0x1e, 0x0a, 0x0b, 0x72, 0x6f, 0x6f, 0x74,
     0x5f, 0x69, 0x6e, 0x74, 0x5f, 0x76, 0x32, 0x18, 0x0a, 0x20, 0x01, 0x28,
     0x05, 0x52, 0x09, 0x72, 0x6f, 0x6f, 0x74, 0x49, 0x6e, 0x74, 0x56, 0x32,
     0x12, 0x44, 0x0a, 0x04, 0x73, 0x75, 0x62, 0x32, 0x18, 0x0b, 0x20, 0x01,
     0x28, 0x0b, 0x32, 0x30, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65,
     0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74,
     0x6f, 0x73, 0x2e, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69,
     0x6f, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x56, 0x32, 0x2e, 0x53, 0x75, 0x62,
     0x32, 0x5f, 0x56, 0x32, 0x52, 0x04, 0x73, 0x75, 0x62, 0x32, 0x12, 0x4b,
     0x0a, 0x08, 0x73, 0x75, 0x62, 0x32, 0x5f, 0x72, 0x65, 0x70, 0x18, 0x0c,
     0x20, 0x03, 0x28, 0x0b, 0x32, 0x30, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
     0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x70, 0x72,
     0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x72,
     0x73, 0x69, 0x6f, 0x6e, 0x69, 0x6e, 0x67, 0x5f, 0x56, 0x32, 0x2e, 0x53,
     0x75, 0x62, 0x32, 0x5f, 0x56, 0x32, 0x52, 0x07, 0x73, 0x75, 0x62, 0x32,
     0x52, 0x65, 0x70, 0x12, 0x51, 0x0a, 0x09, 0x73, 0x75, 0x62, 0x32, 0x5f,
     0x6c, 0x61, 0x7a, 0x79, 0x18, 0x0d, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x30,
     0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x7a, 0x65, 0x72, 0x6f, 0x2e, 0x74,
     0x65, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2e, 0x54,
     0x65, 0x73, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x69, 0x6e,
     0x67, 0x5f, 0x56, 0x32, 0x2e, 0x53, 0x75, 0x62, 0x32, 0x5f, 0x56, 0x32,
     0x42, 0x02, 0x28, 0x01, 0x52, 0x08, 0x73, 0x75, 0x62, 0x32, 0x4c, 0x61,
     0x7a, 0x79, 0x1a, 0x8b, 0x01, 0x0a, 0x07, 0x53, 0x75, 0x62, 0x31, 0x5f,
     0x56, 0x32, 0x12, 0x19, 0x0a, 0x08, 0x73, 0x75, 0x62, 0x31, 0x5f, 0x69,
     0x6e, 0x74, 0x18, 0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x07, 0x73, 0x75,
     0x62, 0x31, 0x49, 0x6e, 0x74, 0x12, 0x1f, 0x0a, 0x0b, 0x73, 0x75, 0x62,
     0x31, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x18, 0x02, 0x20, 0x01,
     0x28, 0x09, 0x52, 0x0a, 0x73, 0x75, 0x62, 0x31, 0x53, 0x74, 0x72, 0x69,
     0x6e, 0x67, 0x12, 0x1e, 0x0a, 0x0b, 0x73, 0x75, 0x62, 0x31, 0x5f, 0x69,
     0x6e, 0x74, 0x5f, 0x76, 0x32, 0x18, 0x03, 0x20, 0x01, 0x28, 0x05, 0x52,
     0x09, 0x73, 0x75, 0x62, 0x31, 0x49, 0x6e, 0x74, 0x56, 0x32, 0x12, 0x24,
     0x0a, 0x0e, 0x73, 0x75, 0x62, 0x31, 0x5f, 0x73, 0x74, 0x72, 0x69, 0x6e,
     0x67, 0x5f, 0x76, 0x32, 0x18, 0x04, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0c,
     0x73, 0x75, 0x62, 0x31, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x56, 0x32,
     0x1a, 0x45, 0x0a, 0x07, 0x53, 0x75, 0x62, 0x32, 0x5f, 0x56, 0x32, 0x12,
     0x19, 0x0a, 0x08, 0x73, 0x75, 0x62, 0x32, 0x5f, 0x69, 0x6e, 0x74, 0x18,
     0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x07, 0x73, 0x75, 0x62, 0x32, 0x49,
     0x6e, 0x74, 0x12, 0x1f, 0x0a, 0x0b, 0x73, 0x75, 0x62, 0x32, 0x5f, 0x73,
     0x74, 0x72, 0x69, 0x6e, 0x67, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09, 0x52,
     0x0a, 0x73, 0x75, 0x62, 0x32, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x22,
     0x2a, 0x0a, 0x08, 0x45, 0x6e, 0x75, 0x6d, 0x7a, 0x5f, 0x56, 0x32, 0x12,
     0x07, 0x0a, 0x03, 0x4f, 0x4e, 0x45, 0x10, 0x01, 0x12, 0x07, 0x0a, 0x03,
     0x54, 0x57, 0x4f, 0x10, 0x02, 0x12, 0x0c, 0x0a, 0x08, 0x54, 0x48, 0x52,
     0x45, 0x45, 0x5f, 0x56, 0x32, 0x10, 0x03, 0x2a, 0x25, 0x0a, 0x09, 0x53,
     0x6d, 0x61, 0x6c, 0x6c, 0x45, 0x6e, 0x75, 0x6d, 0x12, 0x09, 0x0a, 0x05,
     0x54, 0x4f, 0x5f, 0x42, 0x45, 0x10, 0x01, 0x12, 0x0d, 0x0a, 0x09, 0x4e,
     0x4f, 0x54, 0x5f, 0x54, 0x4f, 0x5f, 0x42, 0x45, 0x10, 0x00, 0x2a, 0x3e,
     0x0a, 0x0a, 0x53, 0x69, 0x67, 0x6e, 0x65, 0x64, 0x45, 0x6e, 0x75, 0x6d,
     0x12, 0x0c, 0x0a, 0x08, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x56, 0x45,
     0x10, 0x01, 0x12, 0x0b, 0x0a, 0x07, 0x4e, 0x45, 0x55, 0x54, 0x52, 0x41,
     0x4c, 0x10, 0x00, 0x12, 0x15, 0x0a, 0x08, 0x4e, 0x45, 0x47, 0x41, 0x54,
     0x49, 0x56, 0x45, 0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
     0xff, 0x01, 0x2a, 0x1f, 0x0a, 0x07, 0x42, 0x69, 0x67, 0x45, 0x6e, 0x75,
     0x6d, 0x12, 0x09, 0x0a, 0x05, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x10, 0x0a,
     0x12, 0x09, 0x0a, 0x03, 0x45, 0x4e, 0x44, 0x10, 0x94, 0x91, 0x06}};

}  // namespace perfetto

#endif  // SRC_PROTOZERO_TEST_EXAMPLE_PROTO_TEST_MESSAGES_DESCRIPTOR_H_
