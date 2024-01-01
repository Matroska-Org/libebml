// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_DUMMY_H
#define LIBEBML_DUMMY_H

#include "EbmlBinary.h"

namespace libebml {

class EBML_DLL_API EbmlDummy : public EbmlBinary {
  public:
    EbmlDummy(const EbmlId & aId) : EbmlBinary(EbmlDummy::ClassInfos), DummyId(aId) {}

    bool IsDummy() const override {return true;}
    bool IsDefaultValue() const override {return true;}

    EbmlId const &GetClassId() const override {
      return DummyId;
    }

    EbmlElement & CreateElement() const override { return Create(); }
    EbmlElement * Clone() const override { return new EbmlDummy(DummyId); }

    static EbmlElement & Create() { return *(new EbmlDummy(DummyRawId)); }
    static const EbmlId & ClassId() { return DummyRawId; };
    static const EbmlCallbacks ClassInfos;

  private:
    const EbmlId DummyId;
    static const EbmlId DummyRawId;
};

} // namespace libebml

#endif // LIBEBML_DUMMY_H
