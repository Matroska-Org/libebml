// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlDummy.h"
#include "ebml/EbmlContexts.h"

namespace libebml {

DEFINE_EBML_CLASS_ORPHAN(EbmlDummy, 0xFF, 1, "DummyElement")

const EbmlId EbmlDummy::DummyRawId = Id_EbmlDummy;

} // namespace libebml
