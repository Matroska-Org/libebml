// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlDummy.h"
#include "ebml/EbmlContexts.h"

namespace libebml {

static constexpr EbmlDocVersion AllEbmlVersions{"ebml"};

DEFINE_EBML_CLASS_ORPHAN(EbmlDummy, 0xFF, "DummyElement", AllEbmlVersions )

EbmlDummy::EbmlDummy(const EbmlId & aId) : EbmlBinary(EbmlDummy::ClassInfos), DummyId(aId) {}

} // namespace libebml
