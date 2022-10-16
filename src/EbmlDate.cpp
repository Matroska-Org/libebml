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
