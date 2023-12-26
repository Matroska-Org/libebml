// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include <cassert>
#include <limits>

#include "ebml/EbmlString.h"

namespace libebml {

EbmlString::EbmlString(const EbmlCallbacksDefault<const char *> & classInfo)
  :EbmlElementDefault(classInfo, 0)
{
  if (classInfo.HasDefault())
  {
    auto def = static_cast<const EbmlCallbacksWithDefault<const char *> &>(classInfo);
    SetValue(def.DefaultValue());
  }
}

/*!
  \todo handle exception on errors
*/
filepos_t EbmlString::RenderData(IOCallback & output, bool /* bForceRender */, ShouldWrite /* writeFilter */)
{
  filepos_t Result;
  output.writeFully(Value.c_str(), Value.length());
  Result = Value.length();

  if (Result < GetDefaultSize()) {
    // pad the rest with 0
    std::string Pad(static_cast<std::string::size_type>(GetDefaultSize() - Result), static_cast<char>(0));
    output.writeFully(Pad.c_str(), Pad.size());
    Result = GetDefaultSize();
  }

  return Result;
}

EbmlString::operator const std::string &() const {return Value;}

EbmlElementDefault<const char *> &EbmlString::SetValue(const char * const & NewValue)
{
  Value = NewValue;
  SetValueIsSet();
  return *this;
}

EbmlElementDefault<const char *> &EbmlString::SetValue(std::string const &NewValue) {
  Value = NewValue;
  SetValueIsSet();
  return *this;
}

std::string EbmlString::GetValue() const {
  return Value;
}

std::uint64_t EbmlString::UpdateSize(ShouldWrite writeFilter, bool /* bForceRender */)
{
  if (!writeFilter(*this))
    return 0;

  if (Value.length() < GetDefaultSize()) {
    SetSize_(GetDefaultSize());
  } else {
    SetSize_(Value.length());
  }
  return GetSize();
}

filepos_t EbmlString::ReadData(IOCallback & input, ScopeMode ReadFully)
{
  if (ReadFully == SCOPE_NO_DATA)
    return GetSize();

  if (GetSize() == 0) {
    Value.clear();

  } else {
    Value.resize(GetSize());
    std::memset(&Value[0], 0, GetSize());
    input.readFully(&Value[0], GetSize());

    auto PosNull = Value.find('\0');
    if (PosNull != std::string::npos)
      Value.resize(PosNull);
  }

  SetValueIsSet();

  return GetSize();
}

} // namespace libebml
