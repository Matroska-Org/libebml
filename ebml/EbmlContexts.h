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

namespace libebml {

extern const EbmlSemanticContext EBML_DLL_API EbmlHead_Context;
extern const EbmlSemanticContext EBML_DLL_API EVersion_Context;
extern const EbmlSemanticContext EBML_DLL_API EReadVersion_Context;
extern const EbmlSemanticContext EBML_DLL_API EMaxIdLength_Context;
extern const EbmlSemanticContext EBML_DLL_API EMaxSizeLength_Context;
extern const EbmlSemanticContext EBML_DLL_API EDocType_Context;
extern const EbmlSemanticContext EBML_DLL_API EDocTypeVersion_Context;
extern const EbmlSemanticContext EBML_DLL_API EDocTypeReadVersion_Context;

#define Context_EbmlHead EbmlHead_Context

// global elements
extern const EbmlSemanticContext EBML_DLL_API & GetEbmlGlobal_Context();

} // namespace libebml

#endif // LIBEBML_CONTEXTS_H
