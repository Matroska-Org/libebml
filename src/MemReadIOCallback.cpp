// Copyright © 2002-2014 Moritz Bunkus.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Moritz Bunkus <moritz@bunkus.org>
*/

#include <cstring>
#include <algorithm>

#include "ebml/EbmlBinary.h"
#include "ebml/MemReadIOCallback.h"

namespace libebml {

MemReadIOCallback::MemReadIOCallback(void const *Ptr,
                                     std::size_t Size) {
  Init(Ptr, Size);
}

MemReadIOCallback::MemReadIOCallback(EbmlBinary const &Binary) {
  Init(Binary.GetBuffer(), Binary.GetSize());
}

MemReadIOCallback::MemReadIOCallback(MemReadIOCallback const &Mem) {
  Init(Mem.mPtr, Mem.mEnd - Mem.mPtr);
}

void
MemReadIOCallback::Init(void const *Ptr,
                        std::size_t Size) {
  mStart = reinterpret_cast<std::uint8_t const *>(Ptr);
  mEnd   = mStart + Size;
  mPtr   = mStart;
}

std::size_t
MemReadIOCallback::read(void *Buffer,
                        std::size_t Size) {
  const std::size_t RemainingBytes = mEnd - mPtr;
  if (RemainingBytes < Size)
    Size = RemainingBytes;

  std::memcpy(Buffer, mPtr, Size);
  mPtr += Size;

  return Size;
}

void
MemReadIOCallback::setFilePointer(std::int64_t Offset,
                                  seek_mode Mode) {
  std::int64_t NewPosition = Mode == seek_beginning ? Offset
                    : Mode == seek_end       ? static_cast<std::int64_t>(mEnd - mStart) + Offset
                    :                          static_cast<std::int64_t>(mPtr - mStart) + Offset;

  NewPosition = std::min<std::int64_t>(std::max<std::int64_t>(NewPosition, 0), mEnd - mStart);
  mPtr = mStart + NewPosition;
}

} // namespace libebml
