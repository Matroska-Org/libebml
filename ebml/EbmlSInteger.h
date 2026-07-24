// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
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
    EbmlSInteger();
    EbmlSInteger(int64 DefaultValue);
    EbmlSInteger(const EbmlSInteger & ElementToClone) = default;

    EbmlSInteger & operator = (int64 NewValue) {Value = NewValue; SetValueIsSet(); return *this;}

    /*!
      Set the default size of the integer (usually 1,2,4 or 8)
    */
        void SetDefaultSize(uint64 nDefaultSize = DEFAULT_INT_SIZE) override {EbmlElement::SetDefaultSize(nDefaultSize); SetSize_(nDefaultSize);}

    bool ValidateSize() const override {return IsFiniteSize() && (GetSize() <= 8);}
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    bool IsSmallerThan(const EbmlElement *Cmp) const override;

    operator int8() const;
    operator int16() const;
    operator int32() const;
    operator int64() const;

    EbmlSInteger &SetValue(int64 NewValue);
    int64 GetValue() const;

    void SetDefaultValue(int64 aValue) {assert(!DefaultISset()); DefaultValue = aValue; SetDefaultIsSet();}

    int64 DefaultVal() const {assert(DefaultISset()); return DefaultValue;}

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    int64 Value; /// The actual value of the element
    int64 DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_SINTEGER_H
