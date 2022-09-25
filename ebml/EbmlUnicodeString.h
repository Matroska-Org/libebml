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
  \author Moritz Bunkus <moritz @ bunkus.org>
  \author Jory Stone       <jcsston @ toughguy.net>
*/
#ifndef LIBEBML_UNICODE_STRING_H
#define LIBEBML_UNICODE_STRING_H

#include <string>

#include "EbmlTypes.h"
#include "EbmlElement.h"

namespace libebml {

/*!
  \class UTFstring
  A class storing strings in a wchar_t (ie, in UCS-2 or UCS-4)
  \note inspired by wstring which is not available everywhere
*/
class EBML_DLL_API UTFstring {
public:
  using value_type = wchar_t;

  UTFstring() = default;
  UTFstring(const wchar_t *); // should be NULL terminated
  UTFstring(const UTFstring &);
  UTFstring(std::wstring const &);

  virtual ~UTFstring();
  bool operator==(const UTFstring&) const;
  inline bool operator!=(const UTFstring &cmp) const
  {
    return !(*this == cmp);
  }
  UTFstring & operator=(const UTFstring &);
  UTFstring & operator=(const wchar_t *);
  UTFstring & operator=(wchar_t);

  /// Return length of string
  size_t length() const {return _Length;}

  operator const wchar_t*() const;
  const wchar_t* c_str() const {return _Data;}

  const std::string & GetUTF8() const {return UTF8string;}
  void SetUTF8(const std::string &);

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
  size_t _Length{0}; ///< length of the UCS string excluding the \0
  wchar_t* _Data{nullptr}; ///< internal UCS representation
  std::string UTF8string;
  static bool wcscmp_internal(const wchar_t *str1, const wchar_t *str2);
  void UpdateFromUTF8();
  void UpdateFromUCS2();
};


/*!
    \class EbmlUnicodeString
    \brief Handle all operations on a Unicode string EBML element
  \note internally treated as a string made of wide characters (ie UCS-2 or UCS-4 depending on the machine)
*/
class EBML_DLL_API EbmlUnicodeString : public EbmlElement {
  public:
    EbmlUnicodeString();
    EbmlUnicodeString(const UTFstring & DefaultValue);
    EbmlUnicodeString(const EbmlUnicodeString & ElementToClone) = default;

    ~EbmlUnicodeString() override = default;

    bool ValidateSize() const override {return IsFiniteSize();} // any size is possible
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    EbmlUnicodeString & operator=(const UTFstring &); ///< platform dependant code
    operator const UTFstring &() const;

    EbmlUnicodeString &SetValue(UTFstring const &NewValue);
    EbmlUnicodeString &SetValueUTF8(std::string const &NewValue);
    UTFstring GetValue() const;
    std::string GetValueUTF8() const;

    void SetDefaultValue(UTFstring &);

    const UTFstring & DefaultVal() const;

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    UTFstring Value; /// The actual value of the element
    UTFstring DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_UNICODE_STRING_H
