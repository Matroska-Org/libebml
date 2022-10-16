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
