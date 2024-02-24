// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlContexts.h"
#include "ebml/EbmlCrc32.h"
#include "ebml/EbmlVoid.h"

namespace libebml {

DEFINE_START_SEMANTIC(EbmlGlobal)
DEFINE_SEMANTIC_ITEM(false, false, EbmlCrc32)
DEFINE_SEMANTIC_ITEM(false, false, EbmlVoid)
DEFINE_END_SEMANTIC(EbmlGlobal)

static DEFINE_xxx_CONTEXT(EbmlGlobal, GetEbmlGlobal_Context)

const EbmlSemanticContextMaster & GetEbmlGlobal_Context()
{
  return Context_EbmlGlobal;
}

} // namespace libebml
