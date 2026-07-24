// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

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

namespace libebml {

const int DEFAULT_UINT_SIZE = 0; ///< optimal size stored

/*!
    \class EbmlUInteger
    \brief Handle all operations on an unsigned integer EBML element
*/
class EBML_DLL_API EbmlUInteger : public EbmlElement {
  public:
    EbmlUInteger();
    EbmlUInteger(uint64 DefaultValue);
    EbmlUInteger(const EbmlUInteger & ElementToClone) = default;

    EbmlUInteger & operator=(uint64 NewValue) {Value = NewValue; SetValueIsSet(); return *this;}

    /*!
      Set the default size of the integer (usually 1,2,4 or 8)
    */
    void SetDefaultSize(uint64 nDefaultSize = DEFAULT_UINT_SIZE) override {EbmlElement::SetDefaultSize(nDefaultSize); SetSize_(nDefaultSize);}

    bool ValidateSize() const override {return IsFiniteSize() && (GetSize() <= 8);}
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    bool IsSmallerThan(const EbmlElement *Cmp) const override;

    operator uint8()  const;
    operator uint16() const;
    operator uint32() const;
    operator uint64() const;

    EbmlUInteger &SetValue(uint64 NewValue);
    uint64 GetValue() const;

    void SetDefaultValue(uint64);

    uint64 DefaultVal() const;

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    uint64 Value; /// The actual value of the element
    uint64 DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_UINTEGER_H
