// Copyright © 2002-2014 Moritz Bunkus.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
  \author Moritz Bunkus <moritz@bunkus.org>
*/

#include <cstring>

#include "ebml/EbmlBinary.h"
#include "ebml/MemReadIOCallback.h"

namespace libebml {

MemReadIOCallback::MemReadIOCallback(void const *Ptr,
                                     size_t Size) {
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
                        size_t Size) {
  mStart = reinterpret_cast<uint8 const *>(Ptr);
  mEnd   = mStart + Size;
  mPtr   = mStart;
}

uint32
MemReadIOCallback::read(void *Buffer,
                        size_t Size) {
  size_t RemainingBytes = mEnd - mPtr;
  if (RemainingBytes < Size)
    Size = RemainingBytes;

  std::memcpy(Buffer, mPtr, Size);
  mPtr += Size;

  return Size;
}

void
MemReadIOCallback::setFilePointer(int64 Offset,
                                  seek_mode Mode) {
  int64 NewPosition = Mode == seek_beginning ? Offset
                    : Mode == seek_end       ? static_cast<int64>(mEnd - mStart) + Offset
                    :                          static_cast<int64>(mPtr - mStart) + Offset;

  NewPosition = std::min<int64>(std::max<int64>(NewPosition, 0), mEnd - mStart);
  mPtr = mStart + NewPosition;
}

} // namespace libebml
