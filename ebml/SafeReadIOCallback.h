// Copyright © 2002-2014 Moritz Bunkus.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
  \author Moritz Bunkus <moritz@bunkus.org>
*/
#ifndef LIBEBML_SAFEREADIOCALLBACK_H
#define LIBEBML_SAFEREADIOCALLBACK_H

#include "EbmlBinary.h"
#include "EbmlTypes.h"
#include "IOCallback.h"

namespace libebml {

class EBML_DLL_API SafeReadIOCallback {
public:
  class EBML_DLL_API EndOfStreamX {
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

} // namespace libebml

#endif  // LIBEBML_SAFEREADIOCALLBACK_H
