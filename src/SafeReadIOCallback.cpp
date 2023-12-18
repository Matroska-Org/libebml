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
#include "ebml/SafeReadIOCallback.h"

namespace libebml {

SafeReadIOCallback::EndOfStreamX::EndOfStreamX(size_t MissingBytes)
  : mMissingBytes(MissingBytes)
{
}

// ----------------------------------------------------------------------

SafeReadIOCallback::SafeReadIOCallback(IOCallback *IO,
                                       bool DeleteIO) {
  Init(IO, DeleteIO);
}

SafeReadIOCallback::SafeReadIOCallback(void const *Mem,
                                       size_t Size) {
  Init(new MemReadIOCallback(Mem, Size), true);
}

SafeReadIOCallback::SafeReadIOCallback(EbmlBinary const &Binary) {
  Init(new MemReadIOCallback(Binary), true);
}

SafeReadIOCallback::~SafeReadIOCallback() {
  if (mDeleteIO)
    delete mIO;
}

void
SafeReadIOCallback::Init(IOCallback *IO,
                         bool DeleteIO) {
  mIO                = IO;
  mDeleteIO          = DeleteIO;
  int64 PrevPosition = IO->getFilePointer();
  IO->setFilePointer(0, seek_end);
  mSize              = IO->getFilePointer();
  IO->setFilePointer(PrevPosition);
}

size_t
SafeReadIOCallback::GetPosition()
  const {
  return mIO->getFilePointer();
}

size_t
SafeReadIOCallback::GetSize()
  const {
  return mSize;
}

size_t
SafeReadIOCallback::GetRemainingBytes()
  const {
  return GetSize() - GetPosition();
}

bool
SafeReadIOCallback::IsEmpty()
  const {
  return !GetRemainingBytes();
}

uint64
SafeReadIOCallback::GetUIntBE(size_t NumBytes) {
  uint8 Buffer[8];

  NumBytes     = std::min<size_t>(std::max<size_t>(1, NumBytes), 8);
  uint64 Value = 0;
  uint8* Ptr   = &Buffer[0];

  Read(Buffer, NumBytes);

  for (size_t i = 0; NumBytes > i; ++i, ++Ptr)
    Value = (Value << 8) + *Ptr;

  return Value;
}

uint8
SafeReadIOCallback::GetUInt8() {
  return GetUIntBE(1);
}

uint16
SafeReadIOCallback::GetUInt16BE() {
  return GetUIntBE(2);
}

uint32
SafeReadIOCallback::GetUInt24BE() {
  return GetUIntBE(3);
}

uint32
SafeReadIOCallback::GetUInt32BE() {
  return GetUIntBE(4);
}

uint64
SafeReadIOCallback::GetUInt64BE() {
  return GetUIntBE(8);
}

void
SafeReadIOCallback::Skip(size_t Count) {
  int64 PrevPosition     = mIO->getFilePointer();
  int64 ExpectedPosition = PrevPosition + Count;
  mIO->setFilePointer(Count, seek_current);
  int64 ActualPosition   = mIO->getFilePointer();

  if (ActualPosition != ExpectedPosition)
    throw SafeReadIOCallback::EndOfStreamX(ExpectedPosition - ActualPosition);
}

void
SafeReadIOCallback::Seek(size_t Position) {
  mIO->setFilePointer(Position);
  uint64 ActualPosition = mIO->getFilePointer();
  if (ActualPosition != Position)
    throw EndOfStreamX(ActualPosition - Position);
}

void
SafeReadIOCallback::Read(void *Dst,
                       size_t Count) {
  uint64 NumRead = mIO->read(Dst, Count);
  if (NumRead != Count)
    throw SafeReadIOCallback::EndOfStreamX(Count - NumRead);
}

} // namespace libebml
