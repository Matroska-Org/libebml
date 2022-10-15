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
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/

#ifndef LIBEBML_CONFIG_H
#define LIBEBML_CONFIG_H

#include "ebml_export.h"

#define LIBEBML_NAMESPACE libebml
#define START_LIBEBML_NAMESPACE namespace LIBEBML_NAMESPACE {
#define END_LIBEBML_NAMESPACE   }


// The LIBEBML_DEBUG symbol is defined, when we are creating a debug build. In this
// case the debug logging code is compiled in.
#if (defined(DEBUG)||defined(_DEBUG)) && !defined(LIBEBML_DEBUG)
#define LIBEBML_DEBUG
#endif

#ifdef HAVE_WINAPIFAMILY_H
# include <winapifamily.h>
# if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#  undef LIBEBML_DEBUG
# endif
#endif

#endif // LIBEBML_CONFIG_H
