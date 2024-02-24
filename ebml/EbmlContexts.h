// Copyright © 2002-2004 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_CONTEXTS_H
#define LIBEBML_CONTEXTS_H

#include "EbmlTypes.h"
#include "EbmlElement.h"
#include "EbmlHead.h"

namespace libebml {

#define Context_EbmlHead   EBML_MASTER_CLASS_CONTEXT(EbmlHead)

// global elements
extern const EbmlSemanticContextMaster EBML_DLL_API & GetEbmlGlobal_Context();

} // namespace libebml

#endif // LIBEBML_CONTEXTS_H
