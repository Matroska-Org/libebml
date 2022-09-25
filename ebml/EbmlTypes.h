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
  \version \$Id: EbmlTypes.h 639 2004-07-09 20:59:14Z mosu $
*/
#ifndef LIBEBML_TYPES_H
#define LIBEBML_TYPES_H

#include "ebml/c/libebml_t.h"
#include "ebml/EbmlConfig.h"
#include "EbmlEndian.h" // binary needs to be defined

namespace libebml {

using utf16 = wchar_t;
using utf32 = uint32;
using utf8 = char;

using bits80 = binary[10];

using lil_int16 = Endian<int16, little_endian>;
using lil_int32 = Endian<int32, little_endian>;
using lil_int64 = Endian<int64, little_endian>;
using lil_uint16 = Endian<uint16, little_endian>;
using lil_uint32 = Endian<uint32, little_endian>;
using lil_uint64 = Endian<uint64, little_endian>;
using big_int16 = Endian<int16, big_endian>;
using big_int32 = Endian<int32, big_endian>;
using big_int64 = Endian<int64, big_endian>;
using big_uint16 = Endian<uint16, big_endian>;
using big_uint32 = Endian<uint32, big_endian>;
using big_uint64 = Endian<uint64, big_endian>;
using checksum = Endian<uint32, big_endian>;
using big_80bits = Endian<bits80, big_endian>;


enum ScopeMode {
  SCOPE_PARTIAL_DATA = 0,
  SCOPE_ALL_DATA,
  SCOPE_NO_DATA
};

} // namespace libebml

#endif
