// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Jory Stone       <jcsston @ toughguy.net>
*/


#include "ebml/EbmlUnicodeString.h"

#include "lib/utf8-cpp/source/utf8/checked.h"

#include <cstring>

namespace libebml {

namespace {

std::size_t lengthToFirstNulll(std::string_view s)
{
  auto PosNull = s.find('\0');
  return PosNull != std::string_view::npos ? PosNull : s.size();
}

}

// ===================== UTFstring class ===================

UTFstring::UTFstring(const char * _aBuf)
{
  if (_aBuf != nullptr)
    UTF8string = _aBuf;
}

UTFstring::UTFstring(std::wstring const &_aBuf)
{
  *this = _aBuf.c_str();
}

UTFstring & UTFstring::operator=(const wchar_t * _aBuf)
{
  if (_aBuf != nullptr)
  {
    UpdateFromUCS2(_aBuf);
  }
  else
  {
    UTF8string.clear();
  }

  return *this;
}

UTFstring & UTFstring::operator=(wchar_t _aChar)
{
  UpdateFromUCS2(std::wstring{_aChar});
  return *this;
}

bool UTFstring::operator==(const UTFstring& _aStr) const
{
  // Only compare up to the first 0 char in both strings.
  auto LengthThis  = lengthToFirstNulll(UTF8string);
  auto LengthOther = lengthToFirstNulll(_aStr.UTF8string);

  if (LengthThis != LengthOther)
    return false;

  return std::memcmp(UTF8string.c_str(), _aStr.UTF8string.c_str(), LengthThis) == 0;
}

void UTFstring::SetUTF8(std::string_view _aStr)
{
  UTF8string = _aStr.substr(0, lengthToFirstNulll(_aStr));
}

void UTFstring::UpdateFromUCS2(std::wstring_view WString)
{
  // Only convert up to the first \0 character if present.
  auto Current = std::find(WString.cbegin(), WString.cend(), L'\0');

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

EbmlUnicodeString::EbmlUnicodeString(const EbmlCallbacksDefault<const wchar_t *> & classInfo)
  :EbmlElementDefaultStorage<const wchar_t *, UTFstring>(classInfo, 0)
{
  if (classInfo.HasDefault())
  {
    auto def = static_cast<const EbmlCallbacksWithDefault<const wchar_t *> &>(classInfo);
    SetValue(UTFstring{def.DefaultValue()});
  }
}

/*!
\note limited to UCS-2
\todo handle exception on errors
*/
filepos_t EbmlUnicodeString::RenderData(IOCallback & output, bool /* bForceRender */, const ShouldWrite & /* writeFilter */)
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

EbmlUnicodeString &EbmlUnicodeString::SetValueUTF8(std::string const &NewValue) {
  Value.SetUTF8(NewValue);
  SetValueIsSet();
  return *this;
}

std::string EbmlUnicodeString::GetValueUTF8() const {
  return Value.GetUTF8();
}

/*!
\note limited to UCS-2
*/
std::uint64_t EbmlUnicodeString::UpdateSize(const ShouldWrite & writeFilter, bool /* bForceRender */)
{
  if (!CanWrite(writeFilter))
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
    Value = UTFstring{};

  } else {
    std::string Buffer(static_cast<std::string::size_type>(GetSize()), static_cast<char>(0));
    input.readFully(Buffer.data(), GetSize());

    Value.SetUTF8(Buffer); // SetUTF8 will cut off at the first 0
  }

  SetValueIsSet();

  return GetSize();
}

} // namespace libebml
