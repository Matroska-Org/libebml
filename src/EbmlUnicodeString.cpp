// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Jory Stone       <jcsston @ toughguy.net>
*/

#include <cassert>
#include <limits>

#include "ebml/EbmlUnicodeString.h"

#include "lib/utf8-cpp/source/utf8/checked.h"

namespace libebml {

// ===================== UTFstring class ===================

UTFstring::UTFstring(const wchar_t * _aBuf)
{
  *this = _aBuf;
}

UTFstring::UTFstring(std::wstring const &_aBuf)
{
  *this = _aBuf.c_str();
}

UTFstring::UTFstring(const UTFstring & _aBuf)
{
  *this = _aBuf.c_str();
}

UTFstring & UTFstring::operator=(const UTFstring & _aBuf)
{
  *this = _aBuf.c_str();
  return *this;
}

UTFstring & UTFstring::operator=(const wchar_t * _aBuf)
{
  if (_aBuf != nullptr)
    WString = _aBuf;
  else
    WString.clear();

  UpdateFromUCS2();
  return *this;
}

UTFstring & UTFstring::operator=(wchar_t _aChar)
{
  WString = _aChar;
  UpdateFromUCS2();
  return *this;
}

bool UTFstring::operator==(const UTFstring& _aStr) const
{
  // Only compare up to the first 0 char in both strings.
  auto LengthThis  = std::distance(WString.begin(),       std::find(WString.begin(),       WString.end(),       L'\0'));
  auto LengthOther = std::distance(_aStr.WString.begin(), std::find(_aStr.WString.begin(), _aStr.WString.end(), L'\0'));

  if (LengthThis != LengthOther)
    return false;

  return std::memcmp(WString.c_str(), _aStr.WString.c_str(), LengthThis * sizeof(wchar_t)) == 0;
}

void UTFstring::SetUTF8(const std::string & _aStr)
{
  UTF8string = _aStr;
  UpdateFromUTF8();
}

/*!
  \see RFC 2279
*/
void UTFstring::UpdateFromUTF8()
{
  // Only convert up to the first \0 character if present.
  auto Current = std::find(UTF8string.begin(), UTF8string.end(), '\0');

  WString.clear();
  try {
    // Even though the function names hint at UCS2, the internal
    // representation must actually be compatible with the C++
    // library's implementation. Implementations with sizeof(wchar_t)
    // == 4 are using UCS4.
    if (sizeof(wchar_t) == 2)
      ::utf8::utf8to16(UTF8string.begin(), Current, std::back_inserter(WString));
    else
      ::utf8::utf8to32(UTF8string.begin(), Current, std::back_inserter(WString));
  } catch (::utf8::invalid_code_point &) {
  } catch (::utf8::invalid_utf8 &) {
  }
}

void UTFstring::UpdateFromUCS2()
{
  // Only convert up to the first \0 character if present.
  auto Current = std::find(WString.begin(), WString.end(), L'\0');

  UTF8string.clear();

  try {
    // Even though the function is called UCS2, the internal
    // representation must actually be compatible with the C++
    // library's implementation. Implementations with sizeof(wchar_t)
    // == 4 are using UCS4.
    if (sizeof(wchar_t) == 2)
      ::utf8::utf16to8(WString.begin(), Current, std::back_inserter(UTF8string));
    else
      ::utf8::utf32to8(WString.begin(), Current, std::back_inserter(UTF8string));
  } catch (::utf8::invalid_code_point &) {
  } catch (::utf8::invalid_utf16 &) {
  }
}

// ===================== EbmlUnicodeString class ===================

EbmlUnicodeString::EbmlUnicodeString()
  :EbmlElement(0, false)
{
  SetDefaultSize(0);
}

EbmlUnicodeString::EbmlUnicodeString(const UTFstring & aDefaultValue)
  :EbmlElement(0, true), Value(aDefaultValue), DefaultValue(aDefaultValue)
{
  SetDefaultSize(0);
  SetDefaultIsSet();
}

void EbmlUnicodeString::SetDefaultValue(UTFstring & aValue)
{
  assert(!DefaultISset());
  DefaultValue = aValue;
  SetDefaultIsSet();
}

const UTFstring & EbmlUnicodeString::DefaultVal() const
{
  assert(DefaultISset());
  return DefaultValue;
}


/*!
\note limited to UCS-2
\todo handle exception on errors
*/
filepos_t EbmlUnicodeString::RenderData(IOCallback & output, bool /* bForceRender */, bool /* bWithDefault */)
{
  std::size_t Result = Value.GetUTF8().length();

  if (Result != 0) {
    output.writeFully(Value.GetUTF8().c_str(), Result);
  }

  if (Result < GetDefaultSize()) {
    // pad the rest with 0
    std::string Pad(static_cast<std::string::size_type>(GetDefaultSize() - Result), static_cast<char>(0));
    output.writeFully(Pad.c_str(), Pad.size());
    Result = GetDefaultSize();
  }

  return Result;
}

EbmlUnicodeString::operator const UTFstring &() const {return Value;}

EbmlUnicodeString & EbmlUnicodeString::operator=(const UTFstring & NewString)
{
  Value = NewString;
  SetValueIsSet();
  return *this;
}

EbmlUnicodeString &EbmlUnicodeString::SetValue(UTFstring const &NewValue) {
  return *this = NewValue;
}

EbmlUnicodeString &EbmlUnicodeString::SetValueUTF8(std::string const &NewValue) {
  UTFstring NewValueUTFstring;
  NewValueUTFstring.SetUTF8(NewValue);
  return *this = NewValueUTFstring;
}

UTFstring EbmlUnicodeString::GetValue() const {
  return Value;
}

std::string EbmlUnicodeString::GetValueUTF8() const {
  return Value.GetUTF8();
}

/*!
\note limited to UCS-2
*/
std::uint64_t EbmlUnicodeString::UpdateSize(bool bWithDefault, bool /* bForceRender */)
{
  if (!bWithDefault && IsDefaultValue())
    return 0;

  SetSize_(Value.GetUTF8().length());
  if (GetSize() < GetDefaultSize())
    SetSize_(GetDefaultSize());

  return GetSize();
}

/*!
  \note limited to UCS-2
*/
filepos_t EbmlUnicodeString::ReadData(IOCallback & input, ScopeMode ReadFully)
{
  if (ReadFully == SCOPE_NO_DATA)
    return GetSize();

  if (GetSize() == 0) {
    Value = static_cast<UTFstring::value_type>(0);

  } else {
    std::string Buffer(static_cast<std::string::size_type>(GetSize()), static_cast<char>(0));
    input.readFully(&Buffer[0], GetSize());

    Value.SetUTF8(Buffer.c_str()); // Let conversion to std::string cut off at the first 0
  }

  SetValueIsSet();

  return GetSize();
}

} // namespace libebml
