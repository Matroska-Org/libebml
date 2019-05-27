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
  \version \$Id: EbmlVersion.cpp 639 2004-07-09 20:59:14Z mosu $
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/

#include "ebml/EbmlVersion.h"

START_LIBEBML_NAMESPACE

const std::string EbmlCodeVersion = "1.3.9";

// Up to version 1.3.3 this library exported a build date string. As
// this made the build non-reproducible, replace it by a placeholder to
// remain API compatible.
const std::string EbmlCodeDate = "Unknown";

END_LIBEBML_NAMESPACE
