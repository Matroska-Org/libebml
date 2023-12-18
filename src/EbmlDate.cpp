// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include <cassert>

#include "ebml/EbmlDate.h"

namespace libebml {

filepos_t EbmlDate::ReadData(IOCallback & input, ScopeMode ReadFully)
{
  if ((ReadFully == SCOPE_NO_DATA) || (GetSize() == 0))
    return GetSize();

  assert(GetSize() == 8);
  if (GetSize() != 8) {
    // impossible to read, skip it
    input.setFilePointer(GetSize(), seek_current);
    return GetSize();
  }

  binary Buffer[8];
  input.readFully(Buffer, GetSize());

  myDate = endian::from_big64(Buffer);
  SetValueIsSet();
  return GetSize();
}

filepos_t EbmlDate::RenderData(IOCallback & output, bool /* bForceRender */, bool  /* bWithDefault */)
{
  assert(GetSize() == 8 || GetSize() == 0);
  if (GetSize() == 8) {
    binary b64[8];
    endian::to_big64(myDate, b64);

    output.writeFully(b64,8);
  }

  return GetSize();
}

bool EbmlDate::IsSmallerThan(const EbmlElement *Cmp) const
{
  if (EbmlId(*this) == EbmlId(*Cmp))
    return this->myDate < static_cast<const EbmlDate *>(Cmp)->myDate;

  return false;
}

} // namespace libebml
