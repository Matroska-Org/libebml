// Copyright © 2002-2014 Moritz Bunkus.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Moritz Bunkus <moritz@bunkus.org>
*/
#ifndef LIBEBML_SAFEREADIOCALLBACK_H
#define LIBEBML_SAFEREADIOCALLBACK_H

#include "EbmlBinary.h"
#include "IOCallback.h"

#include <memory>
#include <exception>

namespace libebml {

class EBML_DLL_API SafeReadIOCallback : public std::exception {
public:
  class EBML_DLL_API EndOfStreamX : public std::exception {
  public:
    std::size_t mMissingBytes;
    explicit EndOfStreamX(std::size_t MissingBytes);
  };

private:
  std::unique_ptr<IOCallback> mIO;
  std::size_t mSize;

public:
  SafeReadIOCallback(std::unique_ptr<IOCallback> IO);
  SafeReadIOCallback(void const *Mem, std::size_t Size);
  explicit SafeReadIOCallback(EbmlBinary const &Binary);
  ~SafeReadIOCallback() override = default;
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
  void Init(std::unique_ptr<IOCallback> IO);
};

} // namespace libebml

#endif  // LIBEBML_SAFEREADIOCALLBACK_H
