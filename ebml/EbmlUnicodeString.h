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

#include "EbmlElement.h"

namespace libebml {

/*!
  \class UTFstring
  A class storing strings in a char that can receive wchar_t (ie, in UCS-2 or UCS-4)
*/
class EBML_DLL_API UTFstring {
public:
  using value_type = char;

  UTFstring() = default;
  UTFstring(const char *); // should be NULL terminated
  UTFstring(const UTFstring &) = default;
  UTFstring(std::wstring const &);

  virtual ~UTFstring() = default;
  bool operator==(const UTFstring&) const;
  inline bool operator==(const wchar_t *cmp) const
  {
    return *this == UTFstring(std::wstring{cmp});
  }
  inline bool operator!=(const UTFstring &cmp) const
  {
    return !(*this == cmp);
  }
  inline bool operator!=(const wchar_t *cmp) const
  {
    return !(*this == cmp);
  }
  UTFstring & operator=(const UTFstring &) = default;
  UTFstring & operator=(const wchar_t *);
  UTFstring & operator=(wchar_t);

  /// Return length of string in bytes not counting the trailing nul character
  std::size_t length() const {return UTF8string.length();}

  const std::string & GetUTF8() const {return UTF8string;}
  void SetUTF8(const std::string &);

private:
  std::string UTF8string;
  void UpdateFromUCS2(const std::wstring &);
};


/*!
    \class EbmlUnicodeString
    \brief Handle all operations on a Unicode string EBML element
  \note internally treated as a string made of wide characters (ie UCS-2 or UCS-4 depending on the machine)
*/
class EBML_DLL_API EbmlUnicodeString : public EbmlElementDefaultStorage<const wchar_t *, UTFstring> {
  public:
    EbmlUnicodeString(const EbmlCallbacksDefault<const wchar_t *> &);

    bool ValidateSize() const override {return true;} // any size is possible
    filepos_t RenderData(IOCallback & output, bool bForceRender, ShouldWrite writeFilter = WriteSkipDefault) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(ShouldWrite writeFilter = WriteSkipDefault, bool bForceRender = false) override;

    using EbmlElement::operator const EbmlId &;

    EbmlUnicodeString &SetValueUTF8(std::string const &NewValue);
    std::string GetValueUTF8() const;

    bool operator==(const wchar_t * const & val) const override {
      return static_cast<UTFstring>(val) == Value;
    }
};

} // namespace libebml

#endif // LIBEBML_UNICODE_STRING_H
