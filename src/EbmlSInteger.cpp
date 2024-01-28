// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/
#include <array>
#include <cassert>
#include <cstdint>

#include "ebml/EbmlSInteger.h"

// Converting unsigned int types to signed ints assuming the
// underlying bits in memory should represent the 2's complement of a
// signed integer. See https://stackoverflow.com/a/13208789/507077

namespace {

std::int64_t
ToSigned(std::uint64_t u) {
  if (u <= static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()))
    return static_cast<std::int64_t>(u);

  return static_cast<std::int64_t>(u - std::numeric_limits<std::int64_t>::min()) + std::numeric_limits<std::int64_t>::min();
}

} // namespace

namespace libebml {

EbmlSInteger::EbmlSInteger(const EbmlCallbacksDefault<std::int64_t> & classInfo)
  :EbmlElementDefaultSameStorage(classInfo, DEFAULT_INT_SIZE)
{
  if (classInfo.HasDefault())
  {
    auto def = static_cast<const EbmlCallbacksWithDefault<std::int64_t> &>(classInfo);
    SetValue(def.DefaultValue());
  }
}

EbmlSInteger::operator std::int8_t() const {return  static_cast<std::int8_t>(GetValue());}
EbmlSInteger::operator std::int16_t() const {return static_cast<std::int16_t>(GetValue());}
EbmlSInteger::operator std::int32_t() const {return static_cast<std::int32_t>(GetValue());}
EbmlSInteger::operator std::int64_t() const {return GetValue();}

/*!
  \todo handle exception on errors
*/
filepos_t EbmlSInteger::RenderData(IOCallback & output, bool /* bForceRender */, const ShouldWrite & /* writeFilter */)
{
  std::array<binary, 8> FinalData; // we don't handle more than 64 bits integers
  unsigned int i;

  if (GetSizeLength() > 8)
    return 0; // integer bigger coded on more than 64 bits are not supported

  std::int64_t TempValue = GetValue();
  for (i=0; i<GetSize();i++) {
    FinalData.at(GetSize()-i-1) = static_cast<binary>(TempValue & 0xFF);
    TempValue >>= 8;
  }

  output.writeFully(FinalData.data(),GetSize());

  return GetSize();
}

std::uint64_t EbmlSInteger::UpdateSize(const ShouldWrite & writeFilter, bool /* bForceRender */)
{
  if (!CanWrite(writeFilter))
    return 0;

  const auto Value = GetValue();
  if (Value <= 0x7F && Value >= (-0x80)) {
    SetSize_(1);
  } else if (Value <= 0x7FFF && Value >= (-0x8000)) {
    SetSize_(2);
  } else if (Value <= 0x7FFFFF && Value >= (-0x800000)) {
    SetSize_(3);
  } else if (Value <= INT64_C(0x7FFFFFFF) && Value >= (INT64_C(-0x80000000))) {
    SetSize_(4);
  } else if (Value <= INT64_C(0x7FFFFFFFFF) &&
             Value >= INT64_C(-0x8000000000)) {
    SetSize_(5);
  } else if (Value <= INT64_C(0x7FFFFFFFFFFF) &&
             Value >= INT64_C(-0x800000000000)) {
    SetSize_(6);
  } else if (Value <= INT64_C(0x7FFFFFFFFFFFFF) &&
             Value >= INT64_C(-0x80000000000000)) {
    SetSize_(7);
  } else {
    SetSize_(8);
  }

  if (GetDefaultSize() > GetSize()) {
    SetSize_(GetDefaultSize());
  }

  return GetSize();
}

filepos_t EbmlSInteger::ReadData(IOCallback & input, ScopeMode ReadFully)
{
  if (ReadFully == SCOPE_NO_DATA)
    return GetSize();

  if (GetSize() > 8) {
    // impossible to read, skip it
    input.setFilePointer(GetSize(), seek_current);
    return GetSize();
  }

  std::array<binary, 8> Buffer;
  input.readFully(Buffer.data(), GetSize());

  std::uint64_t TempValue = Buffer[0] & 0x80 ? std::numeric_limits<std::uint64_t>::max() : 0;

  for (unsigned int i=0; i<GetSize(); i++) {
    TempValue <<= 8;
    TempValue |= Buffer.at(i);
  }

  SetValue(ToSigned(TempValue));
  return GetSize();
}

} // namespace libebml
