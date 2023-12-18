// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
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
    explicit EbmlUInteger(std::uint64_t DefaultValue);

    EbmlUInteger & operator=(std::uint64_t NewValue) {Value = NewValue; SetValueIsSet(); return *this;}

    /*!
      Set the default size of the integer (usually 1,2,4 or 8)
    */
    void SetDefaultSize(std::uint64_t nDefaultSize = DEFAULT_UINT_SIZE) override {EbmlElement::SetDefaultSize(nDefaultSize); SetSize_(nDefaultSize);}

    bool ValidateSize() const override {return IsFiniteSize() && (GetSize() <= 8);}
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    bool IsSmallerThan(const EbmlElement *Cmp) const override;

    using EbmlElement::operator const EbmlId &;
    explicit operator std::uint8_t()  const;
    explicit operator std::uint16_t() const;
    explicit operator std::uint32_t() const;
    explicit operator std::uint64_t() const;

    EbmlUInteger &SetValue(std::uint64_t NewValue);
    std::uint64_t GetValue() const;

    void SetDefaultValue(std::uint64_t);

    std::uint64_t DefaultVal() const;

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

    private:
    std::uint64_t Value; /// The actual value of the element
    std::uint64_t DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_UINTEGER_H
