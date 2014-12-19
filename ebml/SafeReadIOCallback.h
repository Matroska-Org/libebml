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
#ifndef LIBEBML_SAFEREADIOCALLBACK_H
#define LIBEBML_SAFEREADIOCALLBACK_H

#include "EbmlTypes.h"

START_LIBEBML_NAMESPACE

class EBML_DLL_API IOCallback;
class EBML_DLL_API EbmlBinary;

class EBML_DLL_API SafeReadIOCallback {
public:
  class EndOfStreamX {
  public:
    size_t mMissingBytes;
    EndOfStreamX(std::size_t MissingBytes);
  };

private:
  IOCallback *mIO;
  bool mDeleteIO;
  size_t mSize;

public:
  SafeReadIOCallback(IOCallback *IO, bool DeleteIO);
  SafeReadIOCallback(void const *Mem, size_t Size);
  SafeReadIOCallback(EbmlBinary const &Binary);
  ~SafeReadIOCallback();

  size_t GetPosition() const;
  size_t GetSize() const;
  size_t GetRemainingBytes() const;
  bool IsEmpty() const;

  uint8  GetUInt8();
  uint64 GetUIntBE(size_t NumBytes);
  uint16 GetUInt16BE();
  uint32 GetUInt24BE();
  uint32 GetUInt32BE();
  uint64 GetUInt64BE();

  void Read(void *Dst, size_t Count);

  void Skip(size_t Count);
  void Seek(size_t Position);

private:
  SafeReadIOCallback(SafeReadIOCallback const &) { }

protected:
  void Init(IOCallback *IO, bool DeleteIO);
};

END_LIBEBML_NAMESPACE

#endif  // LIBEBML_SAFEREADIOCALLBACK_H
