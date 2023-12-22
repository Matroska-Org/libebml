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

EbmlString::EbmlString(const EbmlCallbacks & classInfo)
  :EbmlElement(classInfo, 0, false)
{
  SetDefaultSize(0);
/* done automatically
  SetSize_(Value.length());
  if (GetDefaultSize() > GetSize())
    SetSize_(GetDefaultSize());*/
}

EbmlString::EbmlString(const EbmlCallbacks & classInfo, const std::string & aDefaultValue)
  :EbmlElement(classInfo, 0, true), Value(aDefaultValue), DefaultValue(aDefaultValue)
{
  SetDefaultSize(0);
  SetDefaultIsSet();
/* done automatically
  SetSize_(Value.length());
  if (GetDefaultSize() > GetSize())
    SetSize_(GetDefaultSize());*/
}

/*!
  \todo Cloning should be on the same exact type !
*/
void EbmlString::SetDefaultValue(std::string & aValue)
{
  assert(!DefaultISset());
  DefaultValue = aValue;
  SetDefaultIsSet();
}

const std::string & EbmlString::DefaultVal() const
{
  assert(DefaultISset());
  return DefaultValue;
}


/*!
  \todo handle exception on errors
*/
filepos_t EbmlString::RenderData(IOCallback & output, bool /* bForceRender */, bool /* bWithDefault */)
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

EbmlString & EbmlString::operator=(const std::string & NewString)
{
  Value = NewString;
  SetValueIsSet();
/* done automatically
  SetSize_(Value.length());
  if (GetDefaultSize() > GetSize())
    SetSize_(GetDefaultSize());*/
  return *this;
}

EbmlString &EbmlString::SetValue(std::string const &NewValue) {
  return *this = NewValue;
}

std::string EbmlString::GetValue() const {
  return Value;
}

std::uint64_t EbmlString::UpdateSize(bool bWithDefault, bool /* bForceRender */)
{
  if (!bWithDefault && IsDefaultValue())
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
    Value = "";
    SetValueIsSet();
  } else {
    auto Buffer = (GetSize() + 1 < std::numeric_limits<std::size_t>::max()) ? new (std::nothrow) char[GetSize() + 1] : nullptr;
    if (Buffer == nullptr) {
      // unable to store the data, skip it
      input.setFilePointer(GetSize(), seek_current);
    } else {
      input.readFully(Buffer, GetSize());
      if (Buffer[GetSize()-1] != '\0') {
        Buffer[GetSize()] = '\0';
      }
      Value = Buffer;
      delete [] Buffer;
      SetValueIsSet();
    }
  }

  return GetSize();
}

} // namespace libebml
