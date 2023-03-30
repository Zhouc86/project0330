/*
 * Copyright (C) 2004, 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_WTF_TEXT_TEXT_CODEC_LATIN1_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_WTF_TEXT_TEXT_CODEC_LATIN1_H_

#include "third_party/blink/renderer/platform/wtf/text/text_codec.h"

namespace WTF {

class TextCodecLatin1 final : public TextCodec {
 public:
  static void RegisterEncodingNames(EncodingNameRegistrar);
  static void RegisterCodecs(TextCodecRegistrar);

 private:
  String Decode(const char*,
                wtf_size_t length,
                FlushBehavior,
                bool stop_on_error,
                bool& saw_error) override;
  std::string Encode(const UChar*,
                     wtf_size_t length,
                     UnencodableHandling) override;
  std::string Encode(const LChar*,
                     wtf_size_t length,
                     UnencodableHandling) override;

  template <typename CharType>
  std::string EncodeCommon(const CharType*,
                           wtf_size_t length,
                           UnencodableHandling);
};

}  // namespace WTF

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_WTF_TEXT_TEXT_CODEC_LATIN1_H_
