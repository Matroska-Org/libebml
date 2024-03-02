// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_STREAM_H
#define LIBEBML_STREAM_H

#include "IOCallback.h"
#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlStream
    \brief Handle an input/output stream of EBML elements
*/
class EBML_DLL_API EbmlStream {
  public:
    EbmlStream(IOCallback & DataStream);
    /*!
      \brief Find a possible next ID in the data stream
      \param MaxDataSize The maximum possible of the data in the element (for sanity checks)
      \note the user will have to delete that element later
    */
    EbmlElement * FindNextID(const EbmlCallbacks & ClassInfos, std::uint64_t MaxDataSize) const;

    template <typename Type>
    Type * FindNextID(std::uint64_t MaxDataSize)
    {
      return static_cast<Type *>(FindNextID(EBML_INFO(Type), MaxDataSize));
    }

    EbmlElement * FindNextElement(const EbmlSemanticContext & Context, int & UpperLevel, std::uint64_t MaxDataSize, bool AllowDummyElt, unsigned int MaxLowerLevel = 1) const;

    inline IOCallback & I_O() {return Stream;}
        operator IOCallback &() {return Stream;}

    private:
    IOCallback & Stream;
};

} // namespace libebml

#endif // LIBEBML_STREAM_H
