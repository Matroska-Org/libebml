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
  \version \$Id: EbmlId.h 936 2004-11-10 20:46:28Z mosu $
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_ID_H
#define LIBEBML_ID_H

#include "EbmlTypes.h"

START_LIBEBML_NAMESPACE


#if defined(EBML_STRICT_API)
#define EBML_ID_VALUE(id)  (id).GetValue()
#define EBML_ID_LENGTH(id) (id).GetLength()
#else
#define EBML_ID_VALUE(id)  (id).Value
#define EBML_ID_LENGTH(id) (id).Length
#endif

/*!
  \class EbmlId
*/
class EBML_DLL_API EbmlId {
  public:
    EbmlId(const binary aValue[4], const unsigned int aLength)
      :Length(aLength)
    {
      Value = 0;
      for (unsigned int i=0; i<aLength; i++) {
        Value <<= 8;
        Value += aValue[i];
      }
    }

    EbmlId(const std::uint32_t aValue, const unsigned int aLength)
      :Value(aValue), Length(aLength) {}

    inline bool operator==(const EbmlId & TestId) const
    {
      return ((TestId.Length == Length) && (TestId.Value == Value));
    }
    inline bool operator!=(const EbmlId & TestId) const
    {
      return !(*this == TestId);
    }

    inline void Fill(binary * Buffer) const {
      for (unsigned int i = 0; i<Length; i++) {
        Buffer[i] = (Value >> (8*(Length-i-1))) & 0xFF;
      }
    }

        inline std::size_t GetLength() const { return Length; }
        inline std::uint32_t GetValue() const { return Value; }

#if defined(EBML_STRICT_API)
    private:
#endif
    std::uint32_t Value;
    std::size_t Length;
};

END_LIBEBML_NAMESPACE

#endif // LIBEBML_ID_H
