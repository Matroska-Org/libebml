// Copyright © 2002-2004 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_HEAD_H
#define LIBEBML_HEAD_H

#include "EbmlMaster.h"
#include "EbmlUInteger.h"
#include "EbmlString.h"

namespace libebml {

DECLARE_EBML_MASTER(EbmlHead)
        EBML_CONCRETE_CLASS(EbmlHead)
};

DECLARE_EBML_UINTEGER_DEF(EVersion)
        EBML_CONCRETE_CLASS(EVersion)
};

DECLARE_EBML_UINTEGER_DEF(EReadVersion)
        EBML_CONCRETE_CLASS(EReadVersion)
};

DECLARE_EBML_UINTEGER_DEF(EMaxIdLength)
        EBML_CONCRETE_CLASS(EMaxIdLength)
};

DECLARE_EBML_UINTEGER_DEF(EMaxSizeLength)
        EBML_CONCRETE_CLASS(EMaxSizeLength)
};

DECLARE_EBML_STRING_DEF(EDocType)
        EBML_CONCRETE_CLASS(EDocType)
};

DECLARE_EBML_UINTEGER_DEF(EDocTypeVersion)
        EBML_CONCRETE_CLASS(EDocTypeVersion)
};

DECLARE_EBML_UINTEGER_DEF(EDocTypeReadVersion)
        EBML_CONCRETE_CLASS(EDocTypeReadVersion)
};

} // namespace libebml

#endif // LIBEBML_HEAD_H
