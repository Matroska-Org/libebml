/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2010 Steve Lhomme.  All rights reserved.
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
  \author Moritz Bunkus <moritz @ bunkus.org>
*/
#include <array>
#include <cassert>
#include <limits>
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

EbmlSInteger::EbmlSInteger()
  :EbmlElement(DEFAULT_INT_SIZE, false)
{}

EbmlSInteger::EbmlSInteger(std::int64_t aDefaultValue)
  :EbmlElement(DEFAULT_INT_SIZE, true), Value(aDefaultValue)
{
  SetDefaultIsSet();
}

EbmlSInteger::operator std::int8_t() const {return  static_cast<std::int8_t>(Value);}
EbmlSInteger::operator std::int16_t() const {return static_cast<std::int16_t>(Value);}
EbmlSInteger::operator std::int32_t() const {return static_cast<std::int32_t>(Value);}
EbmlSInteger::operator std::int64_t() const {return Value;}

std::int64_t EbmlSInteger::GetValue() const {return Value;}

EbmlSInteger & EbmlSInteger::SetValue(std::int64_t NewValue) {
  return *this = NewValue;
}

/*!
  \todo handle exception on errors
*/
filepos_t EbmlSInteger::RenderData(IOCallback & output, bool /* bForceRender */, bool /* bWithDefault */)
{
  std::array<binary, 8> FinalData; // we don't handle more than 64 bits integers

  if (GetSizeLength() > 8)
    return 0; // integer bigger coded on more than 64 bits are not supported

  std::int64_t TempValue = Value;
  for (unsigned int i=0; i<GetSize();i++) {
    FinalData.at(GetSize()-i-1) = static_cast<binary>(TempValue & 0xFF);
    TempValue >>= 8;
  }

  output.writeFully(FinalData.data(),GetSize());

  return GetSize();
}

std::uint64_t EbmlSInteger::UpdateSize(bool bWithDefault, bool /* bForceRender */)
{
  if (!bWithDefault && IsDefaultValue())
    return 0;

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

  Value = ToSigned(TempValue);

  SetValueIsSet();
  return GetSize();
}

bool EbmlSInteger::IsSmallerThan(const EbmlElement *Cmp) const
{
  if (EbmlId(*this) == EbmlId(*Cmp))
    return this->Value < static_cast<const EbmlSInteger *>(Cmp)->Value;

  return false;
}

} // namespace libebml
