// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_SUBHEAD_H
#define LIBEBML_SUBHEAD_H

#include <string>

#include "EbmlUInteger.h"
#include "EbmlString.h"

namespace libebml {

DECLARE_EBML_UINTEGER(EVersion)
  public:
        EBML_CONCRETE_CLASS(EVersion)
};

DECLARE_EBML_UINTEGER(EReadVersion)
  public:
        EBML_CONCRETE_CLASS(EReadVersion)
};

DECLARE_EBML_UINTEGER(EMaxIdLength)
  public:
        EBML_CONCRETE_CLASS(EMaxIdLength)
};

DECLARE_EBML_UINTEGER(EMaxSizeLength)
  public:
        EBML_CONCRETE_CLASS(EMaxSizeLength)
};

DECLARE_EBML_STRING(EDocType)
  public:
        EBML_CONCRETE_CLASS(EDocType)
};

DECLARE_EBML_UINTEGER(EDocTypeVersion)
  public:
        EBML_CONCRETE_CLASS(EDocTypeVersion)
};

DECLARE_EBML_UINTEGER(EDocTypeReadVersion)
  public:
        EBML_CONCRETE_CLASS(EDocTypeReadVersion)
};

} // namespace libebml

#endif // LIBEBML_SUBHEAD_H
