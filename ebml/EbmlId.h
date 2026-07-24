// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id: EbmlId.h 936 2004-11-10 20:46:28Z mosu $
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_ID_H
#define LIBEBML_ID_H

#include "EbmlTypes.h"

namespace libebml {


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
      unsigned int i;
      for (i=0; i<aLength; i++) {
        Value <<= 8;
        Value += aValue[i];
      }
    }

    EbmlId(const uint32 aValue, const unsigned int aLength)
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
      unsigned int i;
      for (i = 0; i<Length; i++) {
        Buffer[i] = (Value >> (8*(Length-i-1))) & 0xFF;
      }
    }

        inline size_t GetLength() const { return Length; }
        inline uint32 GetValue() const { return Value; }

#if defined(EBML_STRICT_API)
    private:
#endif
    uint32 Value;
    size_t Length;
};

} // namespace libebml

#endif // LIBEBML_ID_H
