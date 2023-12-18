// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlStream.h"

namespace libebml {

EbmlStream::EbmlStream(IOCallback & DataStream)
  :Stream(DataStream)
{}

EbmlElement * EbmlStream::FindNextID(const EbmlCallbacks & ClassInfos, std::uint64_t MaxDataSize) const
{
  return EbmlElement::FindNextID(Stream, ClassInfos, MaxDataSize);
}

EbmlElement * EbmlStream::FindNextElement(const EbmlSemanticContext & Context, int & UpperLevel, std::uint64_t MaxDataSize, bool AllowDummyElt, unsigned int MaxLowerLevel) const
{
  return EbmlElement::FindNextElement(Stream, Context, UpperLevel, MaxDataSize, AllowDummyElt, MaxLowerLevel);
}

} // namespace libebml
