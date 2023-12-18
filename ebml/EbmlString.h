// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
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
    EbmlString(const std::string & aDefaultValue);
    EbmlString(const EbmlString & ElementToClone) = default;

    ~EbmlString() override = default;

    bool ValidateSize() const override {return IsFiniteSize() && GetSize() < 0x7FFFFFFF;} // any size is possible
    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    EbmlString & operator=(const std::string &);
    operator const std::string &() const;

    EbmlString &SetValue(std::string const &NewValue);
    std::string GetValue() const;

    void SetDefaultValue(std::string &);

    const std::string & DefaultVal() const;

    bool IsDefaultValue() const override {
      return (DefaultISset() && Value == DefaultValue);
    }

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    std::string Value;  /// The actual value of the element
    std::string DefaultValue;
};

} // namespace libebml

#endif // LIBEBML_STRING_H
