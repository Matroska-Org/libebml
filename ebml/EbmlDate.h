// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
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
    EbmlDate(const EbmlCallbacks & classInfo) :EbmlElement(classInfo, 8, false) {}

    /*!
      \brief set the date with a UNIX/C/EPOCH form
      \param NewDate UNIX/C date in UTC (no timezone)
    */
    void SetEpochDate(std::int64_t NewDate) {myDate = (NewDate - UnixEpochDelay) * 1'000'000'000; SetValueIsSet();}
    EbmlDate &SetValue(std::int64_t NewValue) {SetEpochDate(NewValue); return *this;}

    /*!
      \brief get the date with a UNIX/C/EPOCH form
      \note the date is in UTC (no timezone)
    */
    std::int64_t GetEpochDate() const {return static_cast<std::int64_t>(myDate/1'000'000'000 + UnixEpochDelay);}
    std::int64_t GetValue() const {return GetEpochDate();}

    bool ValidateSize() const override {return GetSize() == 8 || GetSize() == 0;}

    /*!
      \note no Default date handled
    */
    filepos_t UpdateSize(ShouldWrite /* writeFilter */, bool /* bForceRender = false */) override {
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

    private:
    filepos_t RenderData(IOCallback & output, bool bForceRender, ShouldWrite writeFilter = WriteSkipDefault) override;

    std::int64_t myDate{0}; ///< internal format of the date

    static constexpr std::uint64_t UnixEpochDelay = 978'307'200; // 2001/01/01 00:00:00 UTC
};

} // namespace libebml

#endif // LIBEBML_DATE_H
