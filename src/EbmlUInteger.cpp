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
  \author Moritz Bunkus <moritz @ bunkus.org>
*/
#include <array>
#include <cassert>

#include "ebml/EbmlUInteger.h"

namespace libebml {

EbmlUInteger::EbmlUInteger()
  :EbmlElement(DEFAULT_UINT_SIZE, false)
{}

EbmlUInteger::EbmlUInteger(std::uint64_t aDefaultValue)
  :EbmlElement(DEFAULT_UINT_SIZE, true), Value(aDefaultValue), DefaultValue(aDefaultValue)
{
  SetDefaultIsSet();
}

void EbmlUInteger::SetDefaultValue(std::uint64_t aValue)
{
  assert(!DefaultISset());
  DefaultValue = aValue;
  SetDefaultIsSet();
}

std::uint64_t EbmlUInteger::DefaultVal() const
{
  assert(DefaultISset());
  return DefaultValue;
}

EbmlUInteger::operator std::uint8_t()  const {return static_cast<std::uint8_t>(Value); }
EbmlUInteger::operator std::uint16_t() const {return static_cast<std::uint16_t>(Value);}
EbmlUInteger::operator std::uint32_t() const {return static_cast<std::uint32_t>(Value);}
EbmlUInteger::operator std::uint64_t() const {return Value;}

std::uint64_t EbmlUInteger::GetValue() const {return Value;}

EbmlUInteger & EbmlUInteger::SetValue(std::uint64_t NewValue) {
  return *this = NewValue;
}

/*!
  \todo handle exception on errors
*/
filepos_t EbmlUInteger::RenderData(IOCallback & output, bool /* bForceRender */, bool /* bWithDefault */)
{
  std::array<binary, 8> FinalData; // we don't handle more than 64 bits integers

  if (GetSizeLength() > 8)
    return 0; // integer bigger coded on more than 64 bits are not supported

  std::uint64_t TempValue = Value;
  for (unsigned int i=0; i<GetSize();i++) {
    FinalData.at(GetSize()-i-1) = TempValue & 0xFF;
    TempValue >>= 8;
  }

  output.writeFully(FinalData.data(),GetSize());

  return GetSize();
}

std::uint64_t EbmlUInteger::UpdateSize(bool bWithDefault, bool /* bForceRender */)
{
  if (!bWithDefault && IsDefaultValue())
    return 0;

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
  Value = 0;

  for (unsigned int i=0; i<GetSize(); i++) {
    Value <<= 8;
    Value |= Buffer.at(i);
  }
  SetValueIsSet();

  return GetSize();
}

bool EbmlUInteger::IsSmallerThan(const EbmlElement *Cmp) const
{
  if (EbmlId(*this) == EbmlId(*Cmp))
    return this->Value < static_cast<const EbmlUInteger *>(Cmp)->Value;

  return false;
}

} // namespace libebml
