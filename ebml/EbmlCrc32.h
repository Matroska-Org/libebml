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
  \author Jory Stone       <jcsston @ toughguy.net>
*/
#ifndef LIBEBML_CRC32_H
#define LIBEBML_CRC32_H

#include <array>
#include <cassert>

#include "EbmlTypes.h"
#include "EbmlBinary.h"

namespace libebml {

DECLARE_EBML_BINARY(EbmlCrc32)
  public:
    bool ValidateSize() const override {return IsFiniteSize() && (GetSize() == 4);}
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
//    filepos_t UpdateSize(bool bWithDefault = false);

    bool IsDefaultValue() const override {
      return false;
    }

    void AddElementCRC32(EbmlElement &ElementToCRC);
    bool CheckElementCRC32(EbmlElement &ElementToCRC) const;

    /*!
      Use this to quickly check a CRC32 with some data
      \return True if inputCRC matches CRC32 generated from input data
    */
    static bool CheckCRC(std::uint32_t inputCRC, const binary *input, std::uint32_t length);
    /*!
      Calls Update() and Finalize(), use to create a CRC32 in one go
    */
    void FillCRC32(const binary *input, std::uint32_t length);
    /*!
      Add data to the CRC table, in other words process some data bit by bit
    */
    void Update(const binary *input, std::uint32_t length);
    /*!
      Use this with Update() to Finalize() or Complete the CRC32
    */
    void Finalize();
    /*!
      Returns a std::uint32_t that has the value of the CRC32
    */
    std::uint32_t GetCrc32() const {
      return m_crc_final;
    }

    void ForceCrc32(std::uint32_t NewValue) { m_crc_final = NewValue; SetValueIsSet();}

    private:
    void ResetCRC();
    void UpdateByte(binary b);

    static const std::array<std::uint32_t, 256> m_tab;
    std::uint32_t m_crc;
    std::uint32_t m_crc_final{0};

    EBML_CONCRETE_CLASS(EbmlCrc32)
};

template <class T>
inline unsigned int GetAlignment(T * /* dummy */=nullptr) // VC60 workaround
{
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
  return __alignof(T);
#elif defined(__GNUC__)
  return __alignof__(T);
#else
  return sizeof(T);
#endif
}

template <class T>
inline bool IsPowerOf2(T n)
{
  return n > 0 && (n & (n-1)) == 0;
}

template <class T1, class T2>
inline T2 ModPowerOf2(T1 a, T2 b)
{
  assert(IsPowerOf2(b));
  return static_cast<T2>(a) & (b-1);
}

inline bool IsAlignedOn(const void *p, unsigned int alignment)
{
  return IsPowerOf2(alignment) ? ModPowerOf2(reinterpret_cast<uintptr_t>(p), alignment) == 0 : reinterpret_cast<uintptr_t>(p) % alignment == 0;
}

template <class T>
inline bool IsAligned(const void *p, T * /* dummy */=nullptr)  // VC60 workaround
{
  return IsAlignedOn(p, GetAlignment<T>());
}

} // namespace libebml

#endif // LIBEBML_CRC32_H
