// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
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

  virtual ~UTFstring() = default;
  bool operator==(const UTFstring&) const;
  inline bool operator!=(const UTFstring &cmp) const
  {
    return !(*this == cmp);
  }
  UTFstring & operator=(const UTFstring &);
  UTFstring & operator=(const wchar_t *);
  UTFstring & operator=(wchar_t);

  /// Return length of string in bytes not counting the trailing nul character
  std::size_t length() const {return UTF8string.length();}

  explicit operator const wchar_t*() const {return WString.c_str();};
  const wchar_t* c_str() const {return WString.c_str();}

  const std::string & GetUTF8() const {return UTF8string;}
  void SetUTF8(const std::string &);

private:
  std::wstring WString; ///< internal UCS representation
  std::string UTF8string;
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
    EbmlUnicodeString(const EbmlCallbacks &);
    explicit EbmlUnicodeString(const EbmlCallbacks &, const UTFstring & DefaultValue);

    bool ValidateSize() const override {return true;} // any size is possible
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    EbmlUnicodeString & operator=(const UTFstring &); ///< platform dependant code
    using EbmlElement::operator const EbmlId &;
    explicit operator const UTFstring &() const;

    EbmlUnicodeString &SetValue(UTFstring const &NewValue);
    EbmlUnicodeString &SetValueUTF8(std::string const &NewValue);
    UTFstring GetValue() const;
    std::string GetValueUTF8() const;

    void SetDefaultValue(UTFstring &);

    const UTFstring & DefaultVal() const;

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

    private:
    UTFstring Value; /// The actual value of the element
    UTFstring DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_UNICODE_STRING_H
