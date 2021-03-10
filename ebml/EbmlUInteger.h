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
  \author Julien Coloos    <suiryc @ users.sf.net>
  \author Moritz Bunkus    <moritz @ bunkus.org>
*/
#ifndef LIBEBML_UINTEGER_H
#define LIBEBML_UINTEGER_H

#include "EbmlTypes.h"
#include "EbmlElement.h"

START_LIBEBML_NAMESPACE

const int DEFAULT_UINT_SIZE = 0; ///< optimal size stored

/*!
    \class EbmlUInteger
    \brief Handle all operations on an unsigned integer EBML element
*/
class EBML_DLL_API EbmlUInteger : public EbmlElement {
  public:
    EbmlUInteger();
    EbmlUInteger(std::uint64_t DefaultValue);
    EbmlUInteger(const EbmlUInteger & ElementToClone) = default;

    EbmlUInteger & operator=(std::uint64_t NewValue) {Value = NewValue; SetValueIsSet(); return *this;}

    /*!
      Set the default size of the integer (usually 1,2,4 or 8)
    */
    virtual void SetDefaultSize(std::uint64_t nDefaultSize = DEFAULT_UINT_SIZE) {EbmlElement::SetDefaultSize(nDefaultSize); SetSize_(nDefaultSize);}

    virtual bool ValidateSize() const {return IsFiniteSize() && (GetSize() <= 8);}
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false);
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA);
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false);

    virtual bool IsSmallerThan(const EbmlElement *Cmp) const;

    operator std::uint8_t()  const;
    operator std::uint16_t() const;
    operator std::uint32_t() const;
    operator std::uint64_t() const;

    EbmlUInteger &SetValue(std::uint64_t NewValue);
    std::uint64_t GetValue() const;

    void SetDefaultValue(std::uint64_t);

    std::uint64_t DefaultVal() const;

    bool IsDefaultValue() const {
      return (DefaultISset() && Value == DefaultValue);
    }

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    std::uint64_t Value; /// The actual value of the element
    std::uint64_t DefaultValue;
};

END_LIBEBML_NAMESPACE

#endif // LIBEBML_UINTEGER_H
