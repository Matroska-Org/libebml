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
  std::uint8_t const *mStart, *mEnd, *mPtr;

public:
  MemReadIOCallback(void const *Ptr, std::size_t Size);
  explicit MemReadIOCallback(EbmlBinary const &Binary);
  MemReadIOCallback(MemReadIOCallback const &Mem);
  ~MemReadIOCallback() override = default;
  MemReadIOCallback& operator=(const MemReadIOCallback&) = delete;

  std::size_t read(void *Buffer, std::size_t Size) override;
  void setFilePointer(std::int64_t Offset, seek_mode Mode = seek_beginning) override;
  std::size_t write(void const *, std::size_t) override { return 0; }
  std::uint64_t getFilePointer() override { return mPtr - mStart; }
  void close() override {}
  binary const *GetDataBuffer() const { return mPtr; }
  std::uint64_t GetDataBufferSize() const { return mEnd - mStart; }

protected:
  void Init(void const *Ptr, std::size_t Size);
};

} // namespace libebml

#endif // LIBEBML_MEMREADIOCALLBACK_H
