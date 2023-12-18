// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/

#include <cassert>

#include "ebml/EbmlFloat.h"

namespace libebml {

EbmlFloat::EbmlFloat(const EbmlFloat::Precision prec)
  :EbmlElement(0, false)
{
  SetPrecision(prec);
}

EbmlFloat::EbmlFloat(const double aDefaultValue, const EbmlFloat::Precision prec)
  :EbmlElement(0, true), Value(aDefaultValue), DefaultValue(aDefaultValue)
{
  SetDefaultIsSet();
  SetPrecision(prec);
}

void EbmlFloat::SetDefaultValue(double aValue)
{
  assert(!DefaultISset());
  DefaultValue = aValue;
  SetDefaultIsSet();
}

double EbmlFloat::DefaultVal() const
{
  assert(DefaultISset());
  return DefaultValue;
}

EbmlFloat::operator float() const {return static_cast<float>(Value);}
EbmlFloat::operator double() const {return (Value);}

double EbmlFloat::GetValue() const {return Value;}

EbmlFloat & EbmlFloat::SetValue(double NewValue) {
  return *this = NewValue;
}

/*!
  \todo handle exception on errors
  \todo handle 10 bits precision
*/
filepos_t EbmlFloat::RenderData(IOCallback & output, bool /* bForceRender */, bool /* bWithDefault */)
{
  assert(GetSize() == 4 || GetSize() == 8);

  if (GetSize() == 4) {
    float val = Value;
    int Tmp;
    memcpy(&Tmp, &val, 4);
    big_int32 TmpToWrite(Tmp);
    output.writeFully(&TmpToWrite.endian(), GetSize());
  } else if (GetSize() == 8) {
    double val = Value;
    int64 Tmp;
    memcpy(&Tmp, &val, 8);
    big_int64 TmpToWrite(Tmp);
    output.writeFully(&TmpToWrite.endian(), GetSize());
  }

  return GetSize();
}

uint64 EbmlFloat::UpdateSize(bool bWithDefault, bool  /* bForceRender */)
{
  if (!bWithDefault && IsDefaultValue())
    return 0;
  return GetSize();
}

/*!
  \todo remove the hack for possible endianess pb (test on little & big endian)
*/
filepos_t EbmlFloat::ReadData(IOCallback & input, ScopeMode ReadFully)
{
  if (ReadFully == SCOPE_NO_DATA)
    return GetSize();

  assert(GetSize() == 4 || GetSize() == 8);
  if (GetSize() != 4 && GetSize() != 8) {
    // impossible to read, skip it
    input.setFilePointer(GetSize(), seek_current);
    return GetSize();
  }

  binary Buffer[8];
  input.readFully(Buffer, GetSize());

  if (GetSize() == 4) {
    big_int32 TmpRead;
    TmpRead.Eval(Buffer);
    auto tmpp = static_cast<int32>(TmpRead);
    float val;
    memcpy(&val, &tmpp, 4);
    Value = static_cast<double>(val);
    SetValueIsSet();
  } else {
    big_int64 TmpRead;
    TmpRead.Eval(Buffer);
    auto tmpp = static_cast<int64>(TmpRead);
    double val;
    memcpy(&val, &tmpp, 8);
    Value = val;
    SetValueIsSet();
  }

  return GetSize();
}

bool EbmlFloat::IsSmallerThan(const EbmlElement *Cmp) const
{
  if (EbmlId(*this) == EbmlId(*Cmp))
    return this->Value < static_cast<const EbmlFloat *>(Cmp)->Value;

  return false;
}

} // namespace libebml
