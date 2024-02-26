// Copyright © 2024 Steve Lhomme.
// SPDX-License-Identifier: ISC

#include <ebml/EbmlString.h>
#include <ebml/EbmlHead.h>
#include <ebml/EbmlContexts.h>

using namespace libebml;

static constexpr const char * TEST_NS = {"test_versioning"};

static constexpr EbmlDocVersion AllVersions{TEST_NS};

DECLARE_xxx_STRING(StringClass,)
    EBML_CONCRETE_CLASS(StringClass)
};
DEFINE_xxx_STRING(StringClass, 0x4321, EbmlHead, "StringClass", AllVersions, GetEbmlGlobal_Context)


static constexpr EbmlDocVersion Deprecated{TEST_NS, EbmlDocVersion::ANY_VERSION};

DECLARE_xxx_UINTEGER(DeprecatedClass,)
    EBML_CONCRETE_CLASS(DeprecatedClass)
};
DEFINE_xxx_UINTEGER(DeprecatedClass, 0x4320, EbmlHead, "DeprecatedClass", Deprecated, GetEbmlGlobal_Context)


static constexpr EbmlDocVersion FromThree{TEST_NS, 3};

DECLARE_xxx_UINTEGER(FromThreeClass,)
    EBML_CONCRETE_CLASS(FromThreeClass)
};
DEFINE_xxx_UINTEGER(FromThreeClass, 0x4323, EbmlHead, "FromThreeClass", FromThree, GetEbmlGlobal_Context)


static constexpr EbmlDocVersion UpToTwo{TEST_NS, 0,2};

DECLARE_xxx_UINTEGER(UpToTwoClass,)
    EBML_CONCRETE_CLASS(UpToTwoClass)
};
DEFINE_xxx_UINTEGER(UpToTwoClass, 0x4322, EbmlHead, "UpToTwoClass", UpToTwo, GetEbmlGlobal_Context)


class ExtraVersioning : public EbmlDocVersion
{
public:
    constexpr ExtraVersioning(bool webm, version_type min = 0, version_type max = ANY_VERSION)
        :EbmlDocVersion(NS, min, max)
        ,InWebM(webm)
    {}

    static constexpr std::string_view NS{"other NS"};
    const bool InWebM;
};

constexpr ExtraVersioning ExtraAllVersions{true};

DECLARE_xxx_STRING(ExtraStringClass,)
    EBML_CONCRETE_CLASS(ExtraStringClass)
};
DEFINE_xxx_STRING(ExtraStringClass, 0x4321, EbmlHead, "ExtraStringClass", ExtraAllVersions, GetEbmlGlobal_Context)


constexpr ExtraVersioning ExtraDeprecated{false, EbmlDocVersion::ANY_VERSION};

DECLARE_xxx_UINTEGER(ExtraDeprecatedClass,)
    EBML_CONCRETE_CLASS(ExtraDeprecatedClass)
};
DEFINE_xxx_UINTEGER(ExtraDeprecatedClass, 0x4320, EbmlHead, "ExtraDeprecatedClass", ExtraDeprecated, GetEbmlGlobal_Context)

constexpr ExtraVersioning ExtraFromThreeVersion{true, 3};

DECLARE_xxx_UINTEGER(ExtraFromThreeClass,)
    EBML_CONCRETE_CLASS(ExtraFromThreeClass)
};
DEFINE_xxx_UINTEGER(ExtraFromThreeClass, 0x4323, EbmlHead, "ExtraFromThreeClass", ExtraFromThreeVersion, GetEbmlGlobal_Context)


constexpr ExtraVersioning ExtraUpToTwoVersion{false, 0, 2};

DECLARE_xxx_UINTEGER(ExtraUpToTwoClass,)
    EBML_CONCRETE_CLASS(ExtraUpToTwoClass)
};
DEFINE_xxx_UINTEGER(ExtraUpToTwoClass, 0x4322, EbmlHead, "ExtraUpToTwoClass", ExtraUpToTwoVersion, GetEbmlGlobal_Context)


int main(void)
{
    StringClass StringClass;
    const auto & specAll = StringClass.ElementSpec();
    if (!specAll.GetVersions().IsValidInVersion(1))
        return 1;

    if (specAll.GetVersions().IsAlwaysDeprecated())
        return 1;


    DeprecatedClass DeprecatedClass;
    const auto & specDeprecated = DeprecatedClass.ElementSpec();
    if (specDeprecated.GetVersions().IsValidInVersion(1))
        return 1;

    if (!specDeprecated.GetVersions().IsAlwaysDeprecated())
        return 1;


    FromThreeClass FromThreeClass;
    const auto & specFromThree = FromThreeClass.ElementSpec();
    if (specFromThree.GetVersions().IsValidInVersion(1))
        return 1;

    if (!specFromThree.GetVersions().IsValidInVersion(3))
        return 1;

    if (!specFromThree.GetVersions().IsValidInVersion(4))
        return 1;

    if (specFromThree.GetVersions().IsAlwaysDeprecated())
        return 1;


    const auto & specUpToTwo = UpToTwoClass::GetElementSpec();
    if (!specUpToTwo.GetVersions().IsValidInVersion(1))
        return 1;

    if (!specUpToTwo.GetVersions().IsValidInVersion(2))
        return 1;

    if (specUpToTwo.GetVersions().IsValidInVersion(3))
        return 1;

    if (specUpToTwo.GetVersions().IsAlwaysDeprecated())
        return 1;



    ExtraStringClass ExtraStringClass;
    const auto & ExtraVersions = ExtraStringClass.ElementSpec().GetVersions();
    if (ExtraVersions.GetNameSpace() != "other NS")
        return 1;
    const auto & specAllExtra = static_cast<const ExtraVersioning &>(ExtraVersions);
    if (!specAllExtra.IsValidInVersion(1))
        return 1;

    if (specAllExtra.IsAlwaysDeprecated())
        return 1;

    if (specAllExtra.IsAlwaysDeprecated())
        return 1;

    if (!specAllExtra.InWebM)
        return 1;


    constexpr const auto & specExtraVersion = ExtraDeprecatedClass::GetElementSpec().GetVersions();
    static_assert(specExtraVersion.GetNameSpace() == ExtraVersioning::NS, "nope");
    const auto * specDeprecatedExtra = static_cast<const ExtraVersioning *>(&specExtraVersion);
    if (specDeprecatedExtra->IsValidInVersion(1))
        return 1;

    if (!specDeprecatedExtra->IsAlwaysDeprecated())
        return 1;

    if (specDeprecatedExtra->InWebM)
        return 1;


    ExtraFromThreeClass ExtraFromThreeClass;
    const auto & specFromThreeExtra = ExtraFromThreeClass.ElementSpec();
    if (specFromThreeExtra.GetVersions().IsValidInVersion(1))
        return 1;

    if (!specFromThreeExtra.GetVersions().IsValidInVersion(3))
        return 1;

    if (!specFromThreeExtra.GetVersions().IsValidInVersion(4))
        return 1;

    if (specFromThreeExtra.GetVersions().IsAlwaysDeprecated())
        return 1;

    if (specFromThreeExtra.GetVersions().GetNameSpace() != "other NS")
        return 1;
    const ExtraVersioning * p_specFromThreeExtra = static_cast<const ExtraVersioning *>(&specFromThreeExtra.GetVersions());
    if (p_specFromThreeExtra->IsValidInVersion(1))
        return 1;

    if (!p_specFromThreeExtra->IsValidInVersion(3))
        return 1;

    if (!p_specFromThreeExtra->IsValidInVersion(4))
        return 1;

    if (p_specFromThreeExtra->IsAlwaysDeprecated())
        return 1;

    if (!p_specFromThreeExtra->InWebM)
        return 1;


    const auto & specUpToTwoExtra = ExtraUpToTwoClass::GetElementSpec();
    if (!specUpToTwoExtra.GetVersions().IsValidInVersion(1))
        return 1;

    if (!specUpToTwoExtra.GetVersions().IsValidInVersion(2))
        return 1;

    if (specUpToTwoExtra.GetVersions().IsValidInVersion(3))
        return 1;

    if (specUpToTwoExtra.GetVersions().IsAlwaysDeprecated())
        return 1;

    return 0;
}
