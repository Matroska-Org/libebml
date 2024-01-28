// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/

#include <cassert>

#include "ebml/EbmlFloat.h"

namespace libebml {

EbmlFloat::EbmlFloat(const EbmlCallbacksDefault<double> & classInfo, const EbmlFloat::Precision prec)
  :EbmlElementDefaultSameStorage<double>(classInfo, 0)
{
  SetPrecision(prec);
  if (classInfo.HasDefault())
  {
    auto def = static_cast<const EbmlCallbacksWithDefault<double> &>(classInfo);
    SetValue(def.DefaultValue());
  }
}

EbmlFloat::operator float() const {return static_cast<float>(GetValue());}
EbmlFloat::operator double() const {return (GetValue());}

/*!
  \todo handle exception on errors
  \todo handle 10 bits precision
*/
filepos_t EbmlFloat::RenderData(IOCallback & output, bool /* bForceRender */, ShouldWrite /* writeFilter */)
{
  assert(GetSize() == 4 || GetSize() == 8);

  if (GetSize() == 4) {
    auto val = static_cast<float>(GetValue());
    std::int32_t Tmp;
    memcpy(&Tmp, &val, 4);
    binary TmpToWrite[4];
    endian::to_big32(Tmp, TmpToWrite);
    output.writeFully(TmpToWrite, 4);
  } else if (GetSize() == 8) {
    double val = GetValue();
    std::int64_t Tmp;
    memcpy(&Tmp, &val, 8);
    binary TmpToWrite[8];
    endian::to_big64(Tmp, TmpToWrite);
    output.writeFully(TmpToWrite, 8);
  }

  return GetSize();
}

std::uint64_t EbmlFloat::UpdateSize(const ShouldWrite & writeFilter, bool /* bForceRender */)
{
  if (!CanWrite(writeFilter))
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

  double NewValue;
  if (GetSize() == 4) {
    auto tmpp = endian::from_big32(Buffer);
    float val;
    memcpy(&val, &tmpp, 4);
    NewValue = static_cast<double>(val);
  } else {
    auto tmpp = endian::from_big64(Buffer);
    memcpy(&NewValue, &tmpp, 8);
  }
  SetValue(NewValue);

  return GetSize();
}

} // namespace libebml
