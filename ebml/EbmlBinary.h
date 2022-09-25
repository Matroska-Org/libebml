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
#ifndef LIBEBML_BINARY_H
#define LIBEBML_BINARY_H

#include <cstdlib>
#include <cstring>

#include "EbmlTypes.h"
#include "EbmlElement.h"

// ----- Added 10/15/2003 by jcsston from Zen -----
#if defined (__BORLANDC__) //Maybe other compilers?
  #include <mem.h>
#endif //__BORLANDC__
// ------------------------------------------------

namespace libebml {

/*!
    \class EbmlBinary
    \brief Handle all operations on an EBML element that contains "unknown" binary data

  \todo handle fix sized elements (like UID of CodecID)
*/
class EBML_DLL_API EbmlBinary : public EbmlElement {
  public:
    EbmlBinary();
    EbmlBinary(const EbmlBinary & ElementToClone);
    ~EbmlBinary() override;

    bool ValidateSize() const override {return IsFiniteSize() && GetSize() < 0x7FFFFFFF;} // we don't mind about what's inside

    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    void SetBuffer(const binary *Buffer, const uint32 BufferSize) {
      Data = const_cast<binary *>(Buffer);
      SetSize_(BufferSize);
      SetValueIsSet();
    }

    binary *GetBuffer() const {return Data;}

    void CopyBuffer(const binary *Buffer, const uint32 BufferSize) {
      if (Data != nullptr)
        free(Data);
      Data = static_cast<binary *>(malloc(BufferSize * sizeof(binary)));
      memcpy(Data, Buffer, BufferSize);
      SetSize_(BufferSize);
      SetValueIsSet();
    }

    operator const binary &() const;

    bool IsDefaultValue() const override {
      return false;
    }

    bool operator==(const EbmlBinary & ElementToCompare) const;

#if defined(EBML_STRICT_API)
  private:
#else
  protected:
#endif
    binary *Data{nullptr}; // the binary data inside the element
};

} // namespace libebml

#endif // LIBEBML_BINARY_H
