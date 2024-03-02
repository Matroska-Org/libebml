// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_DATE_H
#define LIBEBML_DATE_H

#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlDate
    \brief Handle all operations related to an EBML date
*/
class EBML_DLL_API EbmlDate : public EbmlElementDefaultSameStorage<std::int64_t> {
  public:
    EbmlDate(const EbmlCallbacksDefault<std::int64_t> & classInfo) :EbmlElementDefaultSameStorage<std::int64_t>(classInfo, 8)
    {
      if (classInfo.HasDefault())
      {
        const auto& def = static_cast<const EbmlCallbacksWithDefault<std::int64_t> &>(classInfo);
        SetValue(def.DefaultValue());
      }
    }

    /*!
      \brief set the date with a UNIX/C/EPOCH form
      \param NewDate UNIX/C date in UTC (no timezone)
    */
    void SetEpochDate(std::int64_t NewDate) { EbmlElementDefaultSameStorage<std::int64_t>::SetValue( EpochToEbml(NewDate) ); }
    EbmlElementDefaultSameStorage<std::int64_t> &SetValue(const std::int64_t & NewValue) override {SetEpochDate(NewValue); return *this;}

    /*!
      \brief get the date with a UNIX/C/EPOCH form
      \note the date is in UTC (no timezone)
    */
    std::int64_t GetEpochDate() const {return EbmlToEpoch(EbmlElementDefaultSameStorage<std::int64_t>::GetValue());}
    std::int64_t GetValue() const {return GetEpochDate();}

    bool SizeIsValid(std::uint64_t size) const override {return size == 8 || size == 0;}

    /*!
      \note no Default date handled
    */
    filepos_t UpdateSize(const ShouldWrite & writeFilter, bool /* bForceRender = false */) override {
      if (!CanWrite(writeFilter))
        return 0;

      if(!ValueIsSet())
        SetSize_(0);
      else
        SetSize_(8);
      return GetSize();
    }

    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;

  protected:
    static std::int64_t EpochToEbml(std::int64_t epoch)
    {
      return (epoch - UnixEpochDelay) * 1'000'000'000;
    }

    static std::int64_t EbmlToEpoch(std::int64_t ebml)
    {
      return ebml/1'000'000'000 + UnixEpochDelay;
    }

    private:
    filepos_t RenderData(IOCallback & output, bool bForceRender, const ShouldWrite & writeFilter = WriteSkipDefault) override;

    static constexpr std::uint64_t UnixEpochDelay = 978'307'200; // 2001/01/01 00:00:00 UTC
};

} // namespace libebml

#endif // LIBEBML_DATE_H
