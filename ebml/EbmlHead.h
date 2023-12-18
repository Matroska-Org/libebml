// Copyright © 2002-2004 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_HEAD_H
#define LIBEBML_HEAD_H

#include "EbmlTypes.h"
#include "EbmlMaster.h"

namespace libebml {

DECLARE_EBML_MASTER(EbmlHead)
  public:
        EBML_CONCRETE_CLASS(EbmlHead)
};

} // namespace libebml

#endif // LIBEBML_HEAD_H
