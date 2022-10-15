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
  \author Julien Coloos  <suiryc @ users.sf.net>
*/
#include <cassert>
#include <limits>
#include <string>

#include "ebml/EbmlBinary.h"
#include "ebml/StdIOCallback.h"

namespace libebml {

EbmlBinary::EbmlBinary()
  :EbmlElement(0, false) 
{}

EbmlBinary::EbmlBinary(const EbmlBinary & ElementToClone)
  :EbmlElement(ElementToClone)
{
  *this = ElementToClone;
}

EbmlBinary &
EbmlBinary::operator=(const EbmlBinary & ElementToClone)
{
  if (this == &ElementToClone)  // check for self-assigment
    return *this;

  free(Data);
  if (ElementToClone.Data == nullptr)
    Data = nullptr;
  else {
    Data = static_cast<binary *>(malloc(GetSize()));
    if(Data != nullptr)
      memcpy(Data, ElementToClone.Data, GetSize());
  }
  return *this;
}

EbmlBinary::~EbmlBinary() {
  if(Data)
    free(Data);
}

EbmlBinary::operator const binary &() const {return *Data;}


filepos_t EbmlBinary::RenderData(IOCallback & output, bool /* bForceRender */, bool /* bWithDefault */)
{
  output.writeFully(Data,GetSize());

  return GetSize();
}

/*!
  \note no Default binary value handled
*/
std::uint64_t EbmlBinary::UpdateSize(bool /* bWithDefault */, bool /* bForceRender */)
{
  return GetSize();
}

filepos_t EbmlBinary::ReadData(IOCallback & input, ScopeMode ReadFully)
{
  if (Data != nullptr) {
    free(Data);
    Data = nullptr;
  }

  if (ReadFully == SCOPE_NO_DATA) {
    return GetSize();
  }

  if (!GetSize()) {
    SetValueIsSet();
    return 0;
  }

  Data = (GetSize() < std::numeric_limits<std::size_t>::max()) ? static_cast<binary *>(malloc(GetSize())) : nullptr;
  if (Data == nullptr)
    throw CRTError(std::string("Error allocating data"));
  SetValueIsSet();
  return input.read(Data, GetSize());
}

bool EbmlBinary::operator==(const EbmlBinary & ElementToCompare) const
{
  return ((GetSize() == ElementToCompare.GetSize()) && (GetSize() == 0 || !memcmp(Data, ElementToCompare.Data, GetSize())));
}

} // namespace libebml
