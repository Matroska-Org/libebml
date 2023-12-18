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

static constexpr EbmlSemantic EbmlGlobal_ContextList[2] =
{
  EbmlSemantic(false, false, EBML_INFO(EbmlCrc32)),   ///< EbmlCrc32
  EbmlSemantic(false, false, EBML_INFO(EbmlVoid)),    ///< EbmlVoid
};

const EbmlSemanticContext Context_EbmlGlobal = EbmlSemanticContext(0, nullptr, nullptr, GetEbmlGlobal_Context, nullptr);

static const EbmlSemanticContext EbmlGlobal_Context = EbmlSemanticContext(countof(EbmlGlobal_ContextList), EbmlGlobal_ContextList, nullptr, GetEbmlGlobal_Context, nullptr);

const EbmlSemanticContext & GetEbmlGlobal_Context()
{
  return EbmlGlobal_Context;
}

} // namespace libebml
