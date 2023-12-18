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
  \author Moritz Bunkus <moritz@bunkus.org>
*/
#ifndef LIBEBML_SAFEREADIOCALLBACK_H
#define LIBEBML_SAFEREADIOCALLBACK_H

#include "EbmlBinary.h"
#include "EbmlTypes.h"
#include "IOCallback.h"

namespace libebml {

class EBML_DLL_API SafeReadIOCallback : public std::exception {
public:
  class EBML_DLL_API EndOfStreamX : public std::exception {
  public:
    std::size_t mMissingBytes;
    explicit EndOfStreamX(std::size_t MissingBytes);
  };

private:
  IOCallback *mIO;
  bool mDeleteIO;
  std::size_t mSize;

public:
  SafeReadIOCallback(IOCallback *IO, bool DeleteIO);
  SafeReadIOCallback(void const *Mem, std::size_t Size);
  explicit SafeReadIOCallback(EbmlBinary const &Binary);
  ~SafeReadIOCallback() override;
  SafeReadIOCallback(const SafeReadIOCallback&) = delete;
  SafeReadIOCallback& operator=(const SafeReadIOCallback&) = delete;

  std::size_t GetPosition() const;
  std::size_t GetSize() const;
  std::size_t GetRemainingBytes() const;
  bool IsEmpty() const;

  std::uint8_t  GetUInt8();
  std::uint64_t GetUIntBE(std::size_t NumBytes);
  std::uint16_t GetUInt16BE();
  std::uint32_t GetUInt24BE();
  std::uint32_t GetUInt32BE();
  std::uint64_t GetUInt64BE();

  void Read(void *Dst, std::size_t Count);

  void Skip(std::size_t Count);
  void Seek(std::size_t Position);

protected:
  void Init(IOCallback *IO, bool DeleteIO);
};

} // namespace libebml

#endif  // LIBEBML_SAFEREADIOCALLBACK_H
