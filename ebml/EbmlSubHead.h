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

DECLARE_EBML_UINTEGER_DEF(EVersion)
  public:
        EBML_CONCRETE_CLASS(EVersion)
};

DECLARE_EBML_UINTEGER_DEF(EReadVersion)
  public:
        EBML_CONCRETE_CLASS(EReadVersion)
};

DECLARE_EBML_UINTEGER_DEF(EMaxIdLength)
  public:
        EBML_CONCRETE_CLASS(EMaxIdLength)
};

DECLARE_EBML_UINTEGER_DEF(EMaxSizeLength)
  public:
        EBML_CONCRETE_CLASS(EMaxSizeLength)
};

DECLARE_EBML_STRING_DEF(EDocType)
  public:
        EBML_CONCRETE_CLASS(EDocType)
};

DECLARE_EBML_UINTEGER_DEF(EDocTypeVersion)
  public:
        EBML_CONCRETE_CLASS(EDocTypeVersion)
};

DECLARE_EBML_UINTEGER_DEF(EDocTypeReadVersion)
  public:
        EBML_CONCRETE_CLASS(EDocTypeReadVersion)
};

} // namespace libebml

#endif // LIBEBML_SUBHEAD_H
