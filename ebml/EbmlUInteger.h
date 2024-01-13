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

#include "EbmlElement.h"

namespace libebml {

const int DEFAULT_UINT_SIZE = 0; ///< optimal size stored

/*!
    \class EbmlUInteger
    \brief Handle all operations on an unsigned integer EBML element
*/
class EBML_DLL_API EbmlUInteger : public EbmlElementDefaultSameStorage<std::uint64_t> {
  public:
    EbmlUInteger(const EbmlCallbacksDefault<std::uint64_t> &);

    /*!
      Set the default size of the integer (usually 1,2,4 or 8)
    */
    void SetDefaultSize(std::uint64_t nDefaultSize = DEFAULT_UINT_SIZE) override {EbmlElement::SetDefaultSize(nDefaultSize); SetSize_(nDefaultSize);}

    bool ValidateSize() const override {return GetSize() <= 8;}
    filepos_t RenderData(IOCallback & output, bool bForceRender, ShouldWrite writeFilter = WriteSkipDefault) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(ShouldWrite writeFilter = WriteSkipDefault, bool bForceRender = false) override;

    using EbmlElement::operator const EbmlId &;
    explicit operator std::uint8_t()  const;
    explicit operator std::uint16_t() const;
    explicit operator std::uint32_t() const;
    explicit operator std::uint64_t() const;
};

} // namespace libebml

#endif // LIBEBML_UINTEGER_H
