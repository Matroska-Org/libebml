// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlStream.h"

namespace libebml {

EbmlStream::EbmlStream(IOCallback & DataStream)
  :Stream(DataStream)
{}

EbmlElement * EbmlStream::FindNextID(const EbmlCallbacks & ClassInfos, uint64 MaxDataSize)
{
  return EbmlElement::FindNextID(Stream, ClassInfos, MaxDataSize);
}

EbmlElement * EbmlStream::FindNextElement(const EbmlSemanticContext & Context, int & UpperLevel, uint64 MaxDataSize, bool AllowDummyElt, unsigned int MaxLowerLevel)
{
  return EbmlElement::FindNextElement(Stream, Context, UpperLevel, MaxDataSize, AllowDummyElt, MaxLowerLevel);
}

} // namespace libebml
