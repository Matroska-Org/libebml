/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2010 Steve Lhomme.  All rights reserved.
**
** This file is part of libebml.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** See http://www.gnu.org/licenses/lgpl-2.1.html for LGPL licensing information.
**
** Contact license@matroska.org if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

/*!
  \file
  \version \$Id$
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Jory Stone       <jcsston @ toughguy.net>
*/

#include <cassert>

#include "ebml/EbmlUnicodeString.h"

#include "lib/utf8-cpp/source/utf8/checked.h"

START_LIBEBML_NAMESPACE

// ===================== UTFstring class ===================

UTFstring::UTFstring()
  :_Length(0)
  ,_Data(NULL)
{}

UTFstring::UTFstring(const wchar_t * _aBuf)
  :_Length(0)
  ,_Data(NULL)
{
  *this = _aBuf;
}

UTFstring::UTFstring(std::wstring const &_aBuf)
  :_Length(0)
  ,_Data(NULL)
{
  *this = _aBuf.c_str();
}

UTFstring::~UTFstring()
{
  delete [] _Data;
}

UTFstring::UTFstring(const UTFstring & _aBuf)
  :_Length(0)
  ,_Data(NULL)
{
  *this = _aBuf.c_str();
}

UTFstring & UTFstring::operator=(const UTFstring & _aBuf)
{
  *this = _aBuf.c_str();
  return *this;
}

UTFstring::operator const wchar_t*() const {return _Data;}


UTFstring & UTFstring::operator=(const wchar_t * _aBuf)
{
  delete [] _Data;
  if (_aBuf == NULL) {
    _Data = new wchar_t[1];
    _Data[0] = 0;
    UpdateFromUCS2();
    return *this;
  }

  size_t aLen;
  for (aLen=0; _aBuf[aLen] != 0; aLen++);
  _Length = aLen;
  _Data = new wchar_t[_Length+1];
  for (aLen=0; _aBuf[aLen] != 0; aLen++) {
    _Data[aLen] = _aBuf[aLen];
  }
  _Data[aLen] = 0;
  UpdateFromUCS2();
  return *this;
}

UTFstring & UTFstring::operator=(wchar_t _aChar)
{
  delete [] _Data;
  _Data = new wchar_t[2];
  _Length = 1;
  _Data[0] = _aChar;
  _Data[1] = 0;
  UpdateFromUCS2();
  return *this;
}

bool UTFstring::operator==(const UTFstring& _aStr) const
{
  if ((_Data == NULL) && (_aStr._Data == NULL))
    return true;
  if ((_Data == NULL) || (_aStr._Data == NULL))
    return false;
  return wcscmp_internal(_Data, _aStr._Data);
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
  std::string::iterator End = UTF8string.end(), Current = UTF8string.begin();
  while ((Current != End) && *Current)
    ++Current;

  std::wstring Temp;
  try {
    // Even though the function names hint at UCS2, the internal
    // representation must actually be compatible with the C++
    // library's implementation. Implementations with sizeof(wchar_t)
    // == 4 are using UCS4.
    if (sizeof(wchar_t) == 2)
      ::utf8::utf8to16(UTF8string.begin(), Current, std::back_inserter(Temp));
    else
      ::utf8::utf8to32(UTF8string.begin(), Current, std::back_inserter(Temp));
  } catch (::utf8::invalid_code_point &) {
  } catch (::utf8::invalid_utf8 &) {
  }

  delete [] _Data;
  _Length = Temp.length();
  _Data   = new wchar_t[_Length + 1];

  std::memcpy(_Data, Temp.c_str(), sizeof(wchar_t) * (_Length + 1));
}

void UTFstring::UpdateFromUCS2()
{
  UTF8string.clear();

  if (!_Data)
    return;

  // Only convert up to the first \0 character if present.
  size_t Current = 0;
  while ((Current < _Length) && _Data[Current])
    ++Current;

  try {
    // Even though the function is called UCS2, the internal
    // representation must actually be compatible with the C++
    // library's implementation. Implementations with sizeof(wchar_t)
    // == 4 are using UCS4.
    if (sizeof(wchar_t) == 2)
      ::utf8::utf16to8(_Data, _Data + Current, std::back_inserter(UTF8string));
    else
      ::utf8::utf32to8(_Data, _Data + Current, std::back_inserter(UTF8string));
  } catch (::utf8::invalid_code_point &) {
  } catch (::utf8::invalid_utf16 &) {
  }
}

bool UTFstring::wcscmp_internal(const wchar_t *str1, const wchar_t *str2)
{
  size_t Index=0;
  while (str1[Index] == str2[Index] && str1[Index] != 0) {
    Index++;
  }
  return (str1[Index] == str2[Index]);
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

EbmlUnicodeString::EbmlUnicodeString(const EbmlUnicodeString & ElementToClone)
  :EbmlElement(ElementToClone)
  ,Value(ElementToClone.Value)
  ,DefaultValue(ElementToClone.DefaultValue)
{
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
  uint32 Result = Value.GetUTF8().length();

  if (Result != 0) {
    output.writeFully(Value.GetUTF8().c_str(), Result);
  }

  if (Result < GetDefaultSize()) {
    // pad the rest with 0
    binary *Pad = new (std::nothrow) binary[GetDefaultSize() - Result];
    if (Pad != NULL) {
      memset(Pad, 0x00, GetDefaultSize() - Result);
      output.writeFully(Pad, GetDefaultSize() - Result);

      Result = GetDefaultSize();
      delete [] Pad;
    }
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
uint64 EbmlUnicodeString::UpdateSize(bool bWithDefault, bool /* bForceRender */)
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
  if (ReadFully != SCOPE_NO_DATA) {
    if (GetSize() == 0) {
      Value = UTFstring::value_type(0);
      SetValueIsSet();
    } else {
      char *Buffer = new (std::nothrow) char[GetSize()+1];
      if (Buffer == NULL) {
        // impossible to read, skip it
        input.setFilePointer(GetSize(), seek_current);
      } else {
        input.readFully(Buffer, GetSize());
        if (Buffer[GetSize()-1] != 0) {
          Buffer[GetSize()] = 0;
        }

        Value.SetUTF8(Buffer); // implicit conversion to std::string
        delete [] Buffer;
        SetValueIsSet();
      }
    }
  }

  return GetSize();
}

END_LIBEBML_NAMESPACE
