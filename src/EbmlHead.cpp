// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlHead.h"
#include "ebml/EbmlSubHead.h"
#include "ebml/EbmlContexts.h"

namespace libebml {

DEFINE_START_SEMANTIC(EbmlHead)
DEFINE_SEMANTIC_ITEM_UINT(true, true, 1, EVersion)        ///< EBMLVersion
DEFINE_SEMANTIC_ITEM_UINT(true, true, 1, EReadVersion)    ///< EBMLReadVersion
DEFINE_SEMANTIC_ITEM_UINT(true, true, 4, EMaxIdLength)    ///< EBMLMaxIdLength
DEFINE_SEMANTIC_ITEM_UINT(true, true, 8, EMaxSizeLength)  ///< EBMLMaxSizeLength
DEFINE_SEMANTIC_ITEM(true, true, EDocType)        ///< DocType
DEFINE_SEMANTIC_ITEM_UINT(true, true, 1, EDocTypeVersion) ///< DocTypeVersion
DEFINE_SEMANTIC_ITEM_UINT(true, true, 1, EDocTypeReadVersion) ///< DocTypeReadVersion
DEFINE_END_SEMANTIC(EbmlHead)

DEFINE_EBML_MASTER_ORPHAN(EbmlHead, 0x1A45DFA3, 4, "EBMLHead\0ratamapaga")

EbmlHead::EbmlHead()
  :EbmlMaster(EbmlHead_Context)
{}

} // namespace libebml
