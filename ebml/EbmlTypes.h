/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2004 Steve Lhomme.  All rights reserved.
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
*/
#ifndef LIBEBML_TYPES_H
#define LIBEBML_TYPES_H

#include <cstdint>

#include "ebml/EbmlConfig.h"

using int64  = std::int64_t;
using int32  = std::int32_t;
using int16  = std::int16_t;
using int8   = std::int8_t;
using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8  = std::uint8_t;
using binary = std::uint8_t;
using filepos_t = std::uint64_t;

enum open_mode {
    MODE_READ,
    MODE_WRITE,
    MODE_CREATE,
    MODE_SAFE
};

#include "EbmlEndian.h" // binary needs to be defined

namespace libebml {

enum ScopeMode {
  SCOPE_PARTIAL_DATA = 0,
  SCOPE_ALL_DATA,
  SCOPE_NO_DATA
};

} // namespace libebml

#endif
