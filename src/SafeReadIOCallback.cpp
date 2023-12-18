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
#include "ebml/SafeReadIOCallback.h"

namespace libebml {

SafeReadIOCallback::EndOfStreamX::EndOfStreamX(std::size_t MissingBytes)
  : mMissingBytes(MissingBytes)
{
}

// ----------------------------------------------------------------------

SafeReadIOCallback::SafeReadIOCallback(IOCallback *IO,
                                       bool DeleteIO) {
  Init(IO, DeleteIO);
}

SafeReadIOCallback::SafeReadIOCallback(void const *Mem,
                                       std::size_t Size) {
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
  const std::int64_t PrevPosition = IO->getFilePointer();
  IO->setFilePointer(0, seek_end);
  mSize              = IO->getFilePointer();
  IO->setFilePointer(PrevPosition);
}

std::size_t
SafeReadIOCallback::GetPosition()
  const {
  return mIO->getFilePointer();
}

std::size_t
SafeReadIOCallback::GetSize()
  const {
  return mSize;
}

std::size_t
SafeReadIOCallback::GetRemainingBytes()
  const {
  return GetSize() - GetPosition();
}

bool
SafeReadIOCallback::IsEmpty()
  const {
  return !GetRemainingBytes();
}

std::uint64_t
SafeReadIOCallback::GetUIntBE(std::size_t NumBytes) {
  std::uint8_t Buffer[8];

  NumBytes     = std::min<std::size_t>(std::max<std::size_t>(1, NumBytes), 8);
  std::uint64_t Value = 0;
  std::uint8_t* Ptr   = &Buffer[0];

  Read(Buffer, NumBytes);

  for (std::size_t i = 0; NumBytes > i; ++i, ++Ptr)
    Value = (Value << 8) + *Ptr;

  return Value;
}

std::uint8_t
SafeReadIOCallback::GetUInt8() {
  return static_cast<std::uint8_t>(GetUIntBE(1));
}

std::uint16_t
SafeReadIOCallback::GetUInt16BE() {
  return static_cast<std::uint16_t>(GetUIntBE(2));
}

std::uint32_t
SafeReadIOCallback::GetUInt24BE() {
  return static_cast<std::uint32_t>(GetUIntBE(3));
}

std::uint32_t
SafeReadIOCallback::GetUInt32BE() {
  return static_cast<std::uint32_t>(GetUIntBE(4));
}

std::uint64_t
SafeReadIOCallback::GetUInt64BE() {
  return GetUIntBE(8);
}

void
SafeReadIOCallback::Skip(std::size_t Count) {
  const std::int64_t PrevPosition     = mIO->getFilePointer();
  const std::int64_t ExpectedPosition = PrevPosition + Count;
  mIO->setFilePointer(Count, seek_current);
  const std::int64_t ActualPosition   = mIO->getFilePointer();

  if (ActualPosition != ExpectedPosition)
    throw SafeReadIOCallback::EndOfStreamX(ExpectedPosition - ActualPosition);
}

void
SafeReadIOCallback::Seek(std::size_t Position) {
  mIO->setFilePointer(Position);
  const std::uint64_t ActualPosition = mIO->getFilePointer();
  if (ActualPosition != Position)
    throw EndOfStreamX(ActualPosition - Position);
}

void
SafeReadIOCallback::Read(void *Dst,
                       std::size_t Count) {
 const std::uint64_t NumRead = mIO->read(Dst, Count);
  if (NumRead != Count)
    throw SafeReadIOCallback::EndOfStreamX(Count - NumRead);
}

} // namespace libebml
