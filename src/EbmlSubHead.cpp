// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlSubHead.h"
#include "ebml/EbmlContexts.h"

namespace libebml {

DEFINE_EBML_UINTEGER_DEF(EVersion,            0x4286, 2, EbmlHead, "EBMLVersion", 1)
DEFINE_EBML_UINTEGER_DEF(EReadVersion,        0x42F7, 2, EbmlHead, "EBMLReadVersion", 1)
DEFINE_EBML_UINTEGER_DEF(EMaxIdLength,        0x42F2, 2, EbmlHead, "EBMLMaxIdLength", 4)
DEFINE_EBML_UINTEGER_DEF(EMaxSizeLength,      0x42F3, 2, EbmlHead, "EBMLMaxSizeLength", 8)
DEFINE_EBML_STRING_DEF  (EDocType,            0x4282, 2, EbmlHead, "EBMLDocType", "matroska")
DEFINE_EBML_UINTEGER_DEF(EDocTypeVersion,     0x4287, 2, EbmlHead, "EBMLDocTypeVersion", 1)
DEFINE_EBML_UINTEGER_DEF(EDocTypeReadVersion, 0x4285, 2, EbmlHead, "EBMLDocTypeReadVersion", 1)

} // namespace libebml
