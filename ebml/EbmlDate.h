// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_DATE_H
#define LIBEBML_DATE_H

#include "EbmlTypes.h"
#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlDate
    \brief Handle all operations related to an EBML date
*/
class EBML_DLL_API EbmlDate : public EbmlElement {
  public:
    EbmlDate() :EbmlElement(8, false), myDate(0) {}
    EbmlDate(const EbmlDate & ElementToClone);

    /*!
      \brief set the date with a UNIX/C/EPOCH form
      \param NewDate UNIX/C date in UTC (no timezone)
    */
    void SetEpochDate(int64 NewDate) {myDate = (NewDate - UnixEpochDelay) * 1000000000; SetValueIsSet();}
    EbmlDate &SetValue(int64 NewValue) {SetEpochDate(NewValue); return *this;}

    /*!
      \brief get the date with a UNIX/C/EPOCH form
      \note the date is in UTC (no timezone)
    */
    int64 GetEpochDate() const {return static_cast<int64>(myDate/1000000000 + UnixEpochDelay);}
    int64 GetValue() const {return GetEpochDate();}

    bool ValidateSize() const override {return IsFiniteSize() && ((GetSize() == 8) || (GetSize() == 0));}

    /*!
      \note no Default date handled
    */
    filepos_t UpdateSize(bool /* bWithDefault = false */, bool /* bForceRender = false */) override {
      if(!ValueIsSet())
        SetSize_(0);
      else
        SetSize_(8);
      return GetSize();
    }

    bool IsSmallerThan(const EbmlElement *Cmp) const override;

    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;

    bool IsDefaultValue() const override {
      return false;
    }

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;

    int64 myDate; ///< internal format of the date

    static const uint64 UnixEpochDelay;
};

} // namespace libebml

#endif // LIBEBML_DATE_H
