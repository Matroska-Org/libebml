// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_ID_H
#define LIBEBML_ID_H

#include "EbmlTypes.h"

namespace libebml {


#define EBML_ID_VALUE(id)  (id).GetValue()
#define EBML_ID_LENGTH(id) (id).GetLength()

/*!
  \class EbmlId
*/
class EBML_DLL_API EbmlId {
  public:
    constexpr EbmlId(const binary aValue[4], const unsigned int aLength)
      :Value(FromBuffer(aValue, aLength))
      ,Length(LengthFromValue(Value))
    {
    }

    constexpr EbmlId(const std::uint32_t aValue)
      :Value(aValue), Length(LengthFromValue(aValue)) {}

    inline bool operator==(const EbmlId & TestId) const
    {
      return ((TestId.Length == Length) && (TestId.Value == Value));
    }
    inline bool operator!=(const EbmlId & TestId) const
    {
      return !(*this == TestId);
    }

    inline void Fill(binary * Buffer) const {
      unsigned int i;
      for (i = 0; i<Length; i++) {
        Buffer[i] = (Value >> (8*(Length-i-1))) & 0xFF;
      }
    }

    constexpr std::size_t GetLength() const { return Length; }
    constexpr std::uint32_t GetValue() const { return Value; }

  private:
    std::uint32_t Value;
    std::size_t Length;

    static constexpr unsigned int LengthFromValue(std::uint32_t Value) {
      if (Value < 0x100)
        return 1;
      if (Value < 0x10000)
        return 2;
      if (Value < 0x1000000)
        return 3;
      return 4;
    }

    static constexpr std::uint32_t FromBuffer(const binary aValue[4], const unsigned int aLength)
    {
      std::uint32_t Value = 0;
      for (unsigned int i=0; i<aLength; i++) {
        Value <<= 8;
        Value += aValue[i];
      }
      return Value;
    }
};

} // namespace libebml

#endif // LIBEBML_ID_H
