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
#ifndef LIBEBML_STRING_H
#define LIBEBML_STRING_H

#include <string>

#include "EbmlTypes.h"
#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlString
    \brief Handle all operations on a printable string EBML element
*/
class EBML_DLL_API EbmlString : public EbmlElement {
  public:
    EbmlString();
    explicit EbmlString(const std::string & aDefaultValue);

    bool ValidateSize() const override {return IsFiniteSize() && GetSize() < 0x7FFFFFFF;} // any size is possible
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    EbmlString & operator=(const std::string &);
    using EbmlElement::operator const EbmlId &;
    explicit operator const std::string &() const;

    EbmlString &SetValue(std::string const &NewValue);
    std::string GetValue() const;

    void SetDefaultValue(std::string &);

    const std::string & DefaultVal() const;

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

    private:
    std::string Value;  /// The actual value of the element
    std::string DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_STRING_H
