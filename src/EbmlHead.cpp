// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlHead.h"
#include "ebml/EbmlContexts.h"

namespace libebml {

static constexpr EbmlDocVersion AllEbmlVersions{};

DEFINE_EBML_UINTEGER_DEF(EVersion,            0x4286, EbmlHead, "EBMLVersion", 1, AllEbmlVersions)
DEFINE_EBML_UINTEGER_DEF(EReadVersion,        0x42F7, EbmlHead, "EBMLReadVersion", 1, AllEbmlVersions)
DEFINE_EBML_UINTEGER_DEF(EMaxIdLength,        0x42F2, EbmlHead, "EBMLMaxIdLength", 4, AllEbmlVersions)
DEFINE_EBML_UINTEGER_DEF(EMaxSizeLength,      0x42F3, EbmlHead, "EBMLMaxSizeLength", 8, AllEbmlVersions)
DEFINE_EBML_STRING_DEF  (EDocType,            0x4282, EbmlHead, "EBMLDocType", "matroska", AllEbmlVersions)
DEFINE_EBML_UINTEGER_DEF(EDocTypeVersion,     0x4287, EbmlHead, "EBMLDocTypeVersion", 1, AllEbmlVersions)
DEFINE_EBML_UINTEGER_DEF(EDocTypeReadVersion, 0x4285, EbmlHead, "EBMLDocTypeReadVersion", 1, AllEbmlVersions)

DEFINE_START_SEMANTIC(EbmlHead)
DEFINE_SEMANTIC_ITEM(true, true, EVersion)        ///< EBMLVersion
DEFINE_SEMANTIC_ITEM(true, true, EReadVersion)    ///< EBMLReadVersion
DEFINE_SEMANTIC_ITEM(true, true, EMaxIdLength)    ///< EBMLMaxIdLength
DEFINE_SEMANTIC_ITEM(true, true, EMaxSizeLength)  ///< EBMLMaxSizeLength
DEFINE_SEMANTIC_ITEM(true, true, EDocType)        ///< DocType
DEFINE_SEMANTIC_ITEM(true, true, EDocTypeVersion) ///< DocTypeVersion
DEFINE_SEMANTIC_ITEM(true, true, EDocTypeReadVersion) ///< DocTypeReadVersion
DEFINE_END_SEMANTIC(EbmlHead)

DEFINE_EBML_MASTER_ORPHAN(EbmlHead, 0x1A45DFA3, false, "EBMLHead\0ratamapaga", AllEbmlVersions)

EbmlHead::EbmlHead()
  :EbmlMaster(EbmlHead::ClassInfos)
{}

} // namespace libebml
