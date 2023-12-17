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
#ifndef LIBEBML_DUMMY_H
#define LIBEBML_DUMMY_H

#include "EbmlBinary.h"

namespace libebml {

class EBML_DLL_API EbmlDummy : public EbmlBinary {
  public:
    EbmlDummy() :DummyId(DummyRawId) {}
    EbmlDummy(const EbmlId & aId) : DummyId(aId) {}

    bool IsDummy() const override {return true;}
    bool IsDefaultValue() const override {return true;}

    EbmlId const &GetClassId() const override {
      return DummyId;
    }

    const EbmlSemanticContext &Context() const override;
    const char *DebugName() const override { return "DummyElement"; }
    EbmlElement & CreateElement() const override { return Create(); }
    EbmlElement * Clone() const override { return &Create(); }

    static EbmlElement & Create() { return *(new EbmlDummy); }
    static const EbmlId & ClassId() { return DummyRawId; };
    static const EbmlCallbacks ClassInfos;

  private:
    const EbmlId DummyId;
    static const EbmlId DummyRawId;
};

} // namespace libebml

#endif // LIBEBML_DUMMY_H
