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
  \version \$Id: EbmlConfig.h 1241 2006-01-25 00:59:45Z robux4 $
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/

#ifndef LIBEBML_CONFIG_H
#define LIBEBML_CONFIG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ebml_export.h"

#if defined(__linux__)
#include <endian.h>
#if __BYTE_ORDER == __LITTLE_ENDIAN
#undef WORDS_BIGENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#define WORDS_BIGENDIAN 1
#endif
#else
// automatic endianess detection working on GCC
#if !defined(WORDS_BIGENDIAN)
#if (defined (__arm__) && ! defined (__ARMEB__)) || defined (__i386__) || defined (__i860__) || defined (__ns32000__) || defined (__vax__) || defined (__amd64__) || defined (__x86_64__)
#undef WORDS_BIGENDIAN
#elif defined (__sparc__) || defined (__alpha__) || defined (__PPC__) || defined (__mips__) || defined (__ppc__) || defined (__BIG_ENDIAN__)
#define WORDS_BIGENDIAN 1
#else
// not automatically detected, put it yourself
#undef WORDS_BIGENDIAN // for my testing platform (x86)
#endif
#endif // not autoconf
#endif

#define LIBEBML_NAMESPACE libebml
#define START_LIBEBML_NAMESPACE namespace LIBEBML_NAMESPACE {
#define END_LIBEBML_NAMESPACE   }


#ifndef countof
#define countof(x) (sizeof(x)/sizeof((x)[0]))
#endif


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

#ifdef __GNUC__
#define EBML_PRETTYLONGINT(c) (c ## ll)
#else // __GNUC__
#define EBML_PRETTYLONGINT(c) (c)
#endif // __GNUC__

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0581 //Borland C++ Builder 2006 preview
   #include <stdlib.h>  //malloc(), free()
   #include <memory.h> //memcpy()
#endif //__BORLANDC__

#endif // LIBEBML_CONFIG_H
