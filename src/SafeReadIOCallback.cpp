/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2014 Moritz Bunkus.  All rights reserved.
**
** This file is part of libebml.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** See http://www.matroska.org/license/lgpl/ for LGPL licensing information.
**
** Contact license@matroska.org if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

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
  const std::int64_t PrevPosition = IO->getFilePointer();
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

std::uint64_t
SafeReadIOCallback::GetUIntBE(size_t NumBytes) {
  std::uint8_t Buffer[8];

  NumBytes     = std::min<size_t>(std::max<size_t>(1, NumBytes), 8);
  std::uint64_t Value = 0;
  std::uint8_t* Ptr   = &Buffer[0];

  Read(Buffer, NumBytes);

  for (size_t i = 0; NumBytes > i; ++i, ++Ptr)
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
SafeReadIOCallback::Skip(size_t Count) {
  const std::int64_t PrevPosition     = mIO->getFilePointer();
  const std::int64_t ExpectedPosition = PrevPosition + Count;
  mIO->setFilePointer(Count, seek_current);
  const std::int64_t ActualPosition   = mIO->getFilePointer();

  if (ActualPosition != ExpectedPosition)
    throw SafeReadIOCallback::EndOfStreamX(ExpectedPosition - ActualPosition);
}

void
SafeReadIOCallback::Seek(size_t Position) {
  mIO->setFilePointer(Position);
  const std::uint64_t ActualPosition = mIO->getFilePointer();
  if (ActualPosition != Position)
    throw EndOfStreamX(ActualPosition - Position);
}

void
SafeReadIOCallback::Read(void *Dst,
                       size_t Count) {
 const std::uint64_t NumRead = mIO->read(Dst, Count);
  if (NumRead != Count)
    throw SafeReadIOCallback::EndOfStreamX(Count - NumRead);
}

} // namespace libebml
