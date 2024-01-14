// Copyright © 2023 Steve Lhomme.
// SPDX-License-Identifier: ISC

#include <ebml/EbmlMaster.h>
#include <ebml/EbmlUInteger.h>
#include <ebml/EbmlContexts.h>

using namespace libebml;

DECLARE_xxx_UINTEGER_DEF(DummyChild,)
EBML_CONCRETE_CLASS(DummyChild)
};

DEFINE_START_SEMANTIC(CanInfinite)
DEFINE_SEMANTIC_ITEM(true, true, DummyChild)
DEFINE_END_SEMANTIC(CanInfinite)

DECLARE_xxx_MASTER(CanInfinite,)
EBML_CONCRETE_CLASS(CanInfinite)
};

DEFINE_START_SEMANTIC(NoInfinite)
DEFINE_SEMANTIC_ITEM(true, true, DummyChild)
DEFINE_END_SEMANTIC(NoInfinite)

DECLARE_xxx_MASTER(NoInfinite,)
EBML_CONCRETE_CLASS(NoInfinite)
};

DEFINE_EBML_MASTER_ORPHAN(CanInfinite, 0xF0, 1, true, "CanInfinite", EbmlDocVersion{})
DEFINE_EBML_MASTER_ORPHAN(NoInfinite, 0xF0, 1, false, "NoInfinite", EbmlDocVersion{})

DEFINE_EBML_UINTEGER_DEF(DummyChild, 0x42F7, 2, CanInfinite, "DummyChild", 0, EbmlDocVersion{})

CanInfinite::CanInfinite()
    :EbmlMaster(CanInfinite::ClassInfos, Context_CanInfinite)
{}

NoInfinite::NoInfinite()
    :EbmlMaster(NoInfinite::ClassInfos, Context_NoInfinite)
{}

int main(void)
{
    CanInfinite infinite;
    infinite.SetSizeInfinite(true);
    if (infinite.IsFiniteSize())
        return 1;
    infinite.SetSizeInfinite(false);
    if (!infinite.IsFiniteSize())
        return 1;

    infinite.SetSizeInfinite();
    CanInfinite copy_infinite(infinite);
    if (copy_infinite.IsFiniteSize())
        return 1;

    NoInfinite finite;
#ifdef NDEBUG // would assert in debug builds
    finite.SetSizeInfinite(true);
    if (!finite.IsFiniteSize())
        return 1;
#endif
    finite.SetSizeInfinite(false);
    if (!finite.IsFiniteSize())
        return 1;

    DummyChild nonMaster;
#ifdef NDEBUG // would assert in debug builds
    nonMaster.SetSizeInfinite(true);
    if (!nonMaster.IsFiniteSize())
        return 1;
#endif
    nonMaster.SetSizeInfinite(false);
    if (!nonMaster.IsFiniteSize())
        return 1;

    return 0;
}
