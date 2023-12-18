// Copyright © 2014 Moritz Bunkus.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Moritz Bunkus <moritz@bunkus.org>
*/
#ifndef LIBEBML_MEMREADIOCALLBACK_H
#define LIBEBML_MEMREADIOCALLBACK_H

#include "EbmlBinary.h"
#include "IOCallback.h"

namespace libebml {

class EBML_DLL_API MemReadIOCallback : public IOCallback {
protected:
  uint8 const *mStart, *mEnd, *mPtr;

public:
  MemReadIOCallback(void const *Ptr, size_t Size);
  MemReadIOCallback(EbmlBinary const &Binary);
  MemReadIOCallback(MemReadIOCallback const &Mem);
  ~MemReadIOCallback() override = default;

  uint32 read(void *Buffer, size_t Size) override;
  void setFilePointer(int64 Offset, seek_mode Mode = seek_beginning) override;
  size_t write(void const *, size_t) override { return 0; }
  uint64 getFilePointer() override { return mPtr - mStart; }
  void close() override {}
  binary const *GetDataBuffer() const { return mPtr; }
  uint64 GetDataBufferSize() const { return mEnd - mStart; }

protected:
  void Init(void const *Ptr, size_t Size);
};

} // namespace libebml

#endif // LIBEBML_MEMREADIOCALLBACK_H
