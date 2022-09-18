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
  \version \$Id$
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_STREAM_H
#define LIBEBML_STREAM_H

#include "EbmlTypes.h"
#include "IOCallback.h"
#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlStream
    \brief Handle an input/output stream of EBML elements
*/
class EBML_DLL_API EbmlStream {
  public:
    EbmlStream(IOCallback & DataStream);
    ~EbmlStream() = default;

    /*!
      \brief Find a possible next ID in the data stream
      \param MaxDataSize The maximum possible of the data in the element (for sanity checks)
      \note the user will have to delete that element later
    */
    EbmlElement * FindNextID(const EbmlCallbacks & ClassInfos, uint64 MaxDataSize);

    EbmlElement * FindNextElement(const EbmlSemanticContext & Context, int & UpperLevel, uint64 MaxDataSize, bool AllowDummyElt, unsigned int MaxLowerLevel = 1);

    inline IOCallback & I_O() {return Stream;}
        operator IOCallback &() {return Stream;}

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    IOCallback & Stream;
};

} // namespace libebml

#endif // LIBEBML_STREAM_H
