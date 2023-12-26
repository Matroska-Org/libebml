// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_FLOAT_H
#define LIBEBML_FLOAT_H

#include "EbmlTypes.h"
#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlFloat
    \brief Handle all operations on a float EBML element
*/
class EBML_DLL_API EbmlFloat : public EbmlElementDefault<double> {
  public:
    enum Precision {
       FLOAT_32
      ,FLOAT_64
    };

    EbmlFloat(const EbmlCallbacksDefault<double> &, Precision prec = FLOAT_32);

    bool ValidateSize() const override
    {
      return (GetSize() == 4 || GetSize() == 8);
    }

    filepos_t RenderData(IOCallback & output, bool bForceRender, ShouldWrite writeFilter = WriteSkipDefault) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(ShouldWrite writeFilter = WriteSkipDefault, bool bForceRender = false) override;

    void SetPrecision(const EbmlFloat::Precision prec = FLOAT_32)
    {
      if (prec == FLOAT_64)
        SetSize_(8);
      else
        SetSize_(4); // default size
    }


    bool IsSmallerThan(const EbmlElement *Cmp) const override;

    using EbmlElement::operator const EbmlId &;
    operator float() const;
    operator double() const;

    EbmlElementDefault<double> &SetValue(const double & NewValue) override;
    double GetValue() const;

    bool operator==(const double & val) const override {
      return val == Value;
    }

    private:
    double Value; /// The actual value of the element
};

} // namespace libebml

#endif // LIBEBML_FLOAT_H
