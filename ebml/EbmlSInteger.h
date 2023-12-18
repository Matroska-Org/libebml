// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Julien Coloos    <suiryc @ users.sf.net>
  \author Moritz Bunkus    <moritz @ bunkus.org>
*/
#ifndef LIBEBML_SINTEGER_H
#define LIBEBML_SINTEGER_H

#include <cassert>

#include "EbmlTypes.h"
#include "EbmlElement.h"

namespace libebml {

const int DEFAULT_INT_SIZE = 1; ///< optimal size stored

/*!
    \class EbmlSInteger
    \brief Handle all operations on a signed integer EBML element
*/
class EBML_DLL_API EbmlSInteger : public EbmlElement {
  public:
    EbmlSInteger(const EbmlCallbacks &);
    explicit EbmlSInteger(const EbmlCallbacks &, std::int64_t DefaultValue);

    EbmlSInteger & operator = (std::int64_t NewValue) {Value = NewValue; SetValueIsSet(); return *this;}

    /*!
      Set the default size of the integer (usually 1,2,4 or 8)
    */
        void SetDefaultSize(std::uint64_t nDefaultSize = DEFAULT_INT_SIZE) override {EbmlElement::SetDefaultSize(nDefaultSize); SetSize_(nDefaultSize);}

    bool ValidateSize() const override {return GetSize() <= 8;}
    filepos_t RenderData(IOCallback & output, bool bForceRender, ShouldWrite writeFilter = WriteSkipDefault) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(ShouldWrite writeFilter = WriteSkipDefault, bool bForceRender = false) override;

    bool IsSmallerThan(const EbmlElement *Cmp) const override;

    using EbmlElement::operator const EbmlId &;
    explicit operator std::int8_t() const;
    explicit operator std::int16_t() const;
    explicit operator std::int32_t() const;
    explicit operator std::int64_t() const;

    EbmlSInteger &SetValue(std::int64_t NewValue);
    std::int64_t GetValue() const;

    void SetDefaultValue(std::int64_t aValue) {assert(!DefaultISset()); DefaultValue = aValue; SetDefaultIsSet();}

    std::int64_t DefaultVal() const {assert(DefaultISset()); return DefaultValue;}

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

    private:
    std::int64_t Value; /// The actual value of the element
    std::int64_t DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_SINTEGER_H
