// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_VOID_H
#define LIBEBML_VOID_H

#include "EbmlBinary.h"

namespace libebml {

DECLARE_EBML_BINARY(EbmlVoid)
  public:
    /*!
      \brief Set the size of the data (not the complete size of the element)
    */
    void SetSize(std::uint64_t aSize) {SetSize_(aSize);}

    /*!
      \note overwrite to write fake data
    */
    filepos_t RenderData(IOCallback & output, bool bForceRender, ShouldWrite writeFilter = WriteSkipDefault) override;

    /*!
      \brief Replace the void element content (written) with this one
    */
    std::uint64_t ReplaceWith(EbmlElement & EltToReplaceWith, IOCallback & output, bool ComeBackAfterward = true, ShouldWrite writeFilter = WriteSkipDefault);

    /*!
      \brief Void the content of an element
    */
    std::uint64_t Overwrite(const EbmlElement & EltToVoid, IOCallback & output, bool ComeBackAfterward = true, ShouldWrite writeFilter = WriteSkipDefault);

        EBML_CONCRETE_CLASS(EbmlVoid)
};

} // namespace libebml

#endif // LIBEBML_VOID_H
