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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

START_LIBEBML_NAMESPACE

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

MemReadIOCallback::~MemReadIOCallback() {
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

END_LIBEBML_NAMESPACE
