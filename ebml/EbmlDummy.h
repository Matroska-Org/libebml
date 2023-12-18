// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

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
    EbmlDummy(const EbmlDummy & ElementToClone) = default;

    bool IsDummy() const override {return true;}
    bool IsDefaultValue() const override {return true;}

        operator const EbmlId &() const override {
            return DummyId;
        }

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
    const EbmlId DummyId;
    static const EbmlId DummyRawId;

        EBML_CONCRETE_DUMMY_CLASS(EbmlDummy)
};

} // namespace libebml

#endif // LIBEBML_DUMMY_H
