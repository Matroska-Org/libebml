/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2014 Moritz Bunkus.  All rights reserved.
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
  \author Moritz Bunkus <moritz@bunkus.org>
*/
#ifndef LIBEBML_MEMREADIOCALLBACK_H
#define LIBEBML_MEMREADIOCALLBACK_H

#include "IOCallback.h"

START_LIBEBML_NAMESPACE

class EBML_DLL_API EbmlBinary;

class EBML_DLL_API MemReadIOCallback : public IOCallback {
protected:
  uint8 const *mStart, *mEnd, *mPtr;

public:
  MemReadIOCallback(void const *Ptr, size_t Size);
  MemReadIOCallback(EbmlBinary const &Binary);
  MemReadIOCallback(MemReadIOCallback const &Mem);
  virtual ~MemReadIOCallback();

  uint32 read(void *Buffer, size_t Size);
  void setFilePointer(int64 Offset, seek_mode Mode = seek_beginning);
  size_t write(void const *, size_t) { return 0; }
  virtual uint64 getFilePointer() { return mPtr - mStart; }
  void close() {}
  binary const *GetDataBuffer() const { return mPtr; }
  uint64 GetDataBufferSize() const { return mEnd - mStart; }

protected:
  void Init(void const *Ptr, size_t Size);
};

END_LIBEBML_NAMESPACE

#endif // LIBEBML_MEMREADIOCALLBACK_H
