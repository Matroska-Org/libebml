// Copyright © 2002-2004 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id: EbmlHead.h 639 2004-07-09 20:59:14Z mosu $
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_HEAD_H
#define LIBEBML_HEAD_H

#include "EbmlTypes.h"
#include "EbmlMaster.h"

namespace libebml {

DECLARE_EBML_MASTER(EbmlHead)
  public:
    EbmlHead(const EbmlHead & ElementToClone)  = default;

        EBML_CONCRETE_CLASS(EbmlHead)

    bool SetSizeInfinite(bool finite = true) override { return !finite; }
};

} // namespace libebml

#endif // LIBEBML_HEAD_H
