/****************************************************************************
** LIBEBML : parse EBML files, see http://ebml.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2003 Steve Lhomme.  All rights reserved.
**
** This file is part of LIBEBML.
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
    \file libebml_t.h
    \version \$Id: libebml_t.h 1298 2008-02-21 22:14:18Z mosu $
    \author Steve Lhomme     <robux4 @ users.sf.net>
    \author Ingo Ralf Blum   <ingoralfblum @ users.sf.net>
    \author Moritz Bunkus <moritz@bunkus.org>

    \brief Misc type definitions for the C API of LIBEBML

    \note These types should be compiler/language independant (just platform dependant)
    \todo recover the sized types (uint16, int32, etc) here too (or maybe here only)
*/

#ifndef _LIBEBML_T_H_INCLUDED_
#define _LIBEBML_T_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

using binary = std::uint8_t;
using filepos_t = std::uint64_t;

enum open_mode {
    MODE_READ,
    MODE_WRITE,
    MODE_CREATE,
    MODE_SAFE
};

#define EBML_MIN(x,y) ((x)<(y) ? (x) : (y))

#ifdef __cplusplus
}
#endif

#endif /* _LIBEBML_T_H_INCLUDED_ */
