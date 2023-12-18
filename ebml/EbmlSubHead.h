/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2010 Steve Lhomme.  All rights reserved.
**
** This file is part of libebml.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** See http://www.gnu.org/licenses/lgpl-2.1.html for LGPL licensing information.
**
** Contact license@matroska.org if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

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
