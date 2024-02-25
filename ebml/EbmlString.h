// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_STRING_H
#define LIBEBML_STRING_H

#include <string>

#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlString
    \brief Handle all operations on a printable string EBML element
*/
class EBML_DLL_API EbmlString : public EbmlElementDefaultStorage<const char *, std::string> {
  public:
    EbmlString(const EbmlCallbacksDefault<const char *> &);

    static inline bool SizeIsValid(std::uint64_t size)  {return size < 0x7FFFFFFF;} // any size is possible
    filepos_t RenderData(IOCallback & output, bool bForceRender, const ShouldWrite & writeFilter = WriteSkipDefault) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(const ShouldWrite & writeFilter = WriteSkipDefault, bool bForceRender = false) override;

    using EbmlElement::operator const EbmlId &;

    bool operator==(const char * const & val) const override {
      return val == Value;
    }
};

} // namespace libebml

#endif // LIBEBML_STRING_H
