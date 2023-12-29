// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/
#include <array>
#include <cassert>

#include "ebml/EbmlUInteger.h"

namespace libebml {

EbmlUInteger::EbmlUInteger(const EbmlCallbacksDefault<std::uint64_t> & classInfo)
  :EbmlElementDefaultSameStorage(classInfo, DEFAULT_UINT_SIZE)
{
  if (classInfo.HasDefault())
  {
    auto def = static_cast<const EbmlCallbacksWithDefault<std::uint64_t> &>(classInfo);
    SetValue(def.DefaultValue());
  }
}

EbmlUInteger::operator std::uint8_t()  const {return static_cast<std::uint8_t>(GetValue()); }
EbmlUInteger::operator std::uint16_t() const {return static_cast<std::uint16_t>(GetValue());}
EbmlUInteger::operator std::uint32_t() const {return static_cast<std::uint32_t>(GetValue());}
EbmlUInteger::operator std::uint64_t() const {return GetValue();}

/*!
  \todo handle exception on errors
*/
filepos_t EbmlUInteger::RenderData(IOCallback & output, bool /* bForceRender */, ShouldWrite /* writeFilter */)
{
  std::array<binary, 8> FinalData; // we don't handle more than 64 bits integers

  if (GetSizeLength() > 8)
    return 0; // integer bigger coded on more than 64 bits are not supported

  std::uint64_t TempValue = GetValue();
  for (unsigned int i=0; i<GetSize();i++) {
    FinalData.at(GetSize()-i-1) = TempValue & 0xFF;
    TempValue >>= 8;
  }

  output.writeFully(FinalData.data(),GetSize());

  return GetSize();
}

std::uint64_t EbmlUInteger::UpdateSize(ShouldWrite writeFilter, bool /* bForceRender */)
{
  if (!writeFilter(*this))
    return 0;

  const auto Value = GetValue();
  if (Value <= 0xFF) {
    SetSize_(1);
  } else if (Value <= 0xFFFF) {
    SetSize_(2);
  } else if (Value <= 0xFFFFFF) {
    SetSize_(3);
  } else if (Value <= 0xFFFFFFFF) {
    SetSize_(4);
  } else if (Value <= 0xFFFFFFFFFFLL) {
    SetSize_(5);
  } else if (Value <= 0xFFFFFFFFFFFFLL) {
    SetSize_(6);
  } else if (Value <= 0xFFFFFFFFFFFFFFLL) {
    SetSize_(7);
  } else {
    SetSize_(8);
  }

  if (GetDefaultSize() > GetSize()) {
    SetSize_(GetDefaultSize());
  }

  return GetSize();
}

filepos_t EbmlUInteger::ReadData(IOCallback & input, ScopeMode ReadFully)
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
  std::uint64_t Value = 0;

  for (unsigned int i=0; i<GetSize(); i++) {
    Value <<= 8;
    Value |= Buffer.at(i);
  }
  SetValue(Value);

  return GetSize();
}

} // namespace libebml
