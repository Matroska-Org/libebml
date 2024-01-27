#include <ebml/EbmlContexts.h>
#include <ebml/EbmlString.h>
#include <ebml/EbmlUInteger.h>
#include <ebml/EbmlUnicodeString.h>

using namespace libebml;

DECLARE_EBML_STRING_DEF(StringWithDefault)
    EBML_CONCRETE_CLASS(StringWithDefault)
};
DEFINE_EBML_STRING_DEF(StringWithDefault, 0x4321, EbmlHead, "StringWithDefault", "Default Value", EbmlDocVersion{})

DECLARE_xxx_STRING(StringWithoutDefault,)
    EBML_CONCRETE_CLASS(StringWithoutDefault)
};
DEFINE_xxx_STRING(StringWithoutDefault, 0x4123, EbmlHead, "StringWithoutDefault", EbmlDocVersion{}, GetEbmlGlobal_Context)

DECLARE_xxx_UNISTRING_DEF(UniStringWithDefault,)
    EBML_CONCRETE_CLASS(UniStringWithDefault)
};
DEFINE_xxx_UNISTRING_DEF(UniStringWithDefault, 0x4321, EbmlHead, "UniStringWithDefault", EbmlDocVersion{}, GetEbmlGlobal_Context, L"Default Value")

DECLARE_xxx_UNISTRING(UniStringWithoutDefault,)
    EBML_CONCRETE_CLASS(UniStringWithoutDefault)
};
DEFINE_xxx_UNISTRING(UniStringWithoutDefault, 0x4123, EbmlHead, "UniStringWithoutDefault", EbmlDocVersion{}, GetEbmlGlobal_Context)

DECLARE_EBML_UINTEGER_DEF(UIntWithDefault)
    EBML_CONCRETE_CLASS(UIntWithDefault)
};
DEFINE_EBML_UINTEGER_DEF(UIntWithDefault, 0x654321, EbmlHead, "UIntWithDefault", 42, EbmlDocVersion{})

DECLARE_xxx_UINTEGER(UIntWithoutDefault,)
    EBML_CONCRETE_CLASS(UIntWithoutDefault)
};
DEFINE_xxx_UINTEGER(UIntWithoutDefault, 0x612345, EbmlHead, "UIntWithoutDefault", EbmlDocVersion{}, GetEbmlGlobal_Context)

int main(void)
{
    if (!StringWithDefault::ClassInfo().HasDefault())
        return 1;
    if (StringWithoutDefault::ClassInfo().HasDefault())
        return 1;
    if (!UniStringWithDefault::ClassInfo().HasDefault())
        return 1;
    if (UniStringWithoutDefault::ClassInfo().HasDefault())
        return 1;
    if (!UIntWithDefault::ClassInfo().HasDefault())
        return 1;
    if (UIntWithoutDefault::ClassInfo().HasDefault())
        return 1;

    if (UIntWithDefault::GetElementSpec().DefaultValue() != 42)
        return 1;

    UIntWithDefault test0;
    auto ClassSpecWithDefault = test0.ElementSpec();
    if (!ClassSpecWithDefault.HasDefault())
        return 1;

    test0.SetValue(0);
    if (test0.IsDefaultValue())
        return 1;

    UIntWithDefault test42;
    test42.SetValue(42);
    if (!test42.IsDefaultValue())
        return 1;

    if (!test42.ElementSpec().HasDefault())
        return 1;

    if (test42.ElementSpec().DefaultValue() != 42)
        return 1;

    uint64_t val = static_cast<uint64_t>(test42);

    test42.SetValue(val + 1);
    if (test42.IsDefaultValue())
        return 1;

    test42.SetValue(0);
    if (test42.IsDefaultValue())
        return 1;

    test42.SetValue(42);
    if (!test42.IsDefaultValue())
        return 1;

    UIntWithoutDefault testNoDefault0;
    auto ClassSpecWithoutDefault = testNoDefault0.ElementSpec();
    if (ClassSpecWithoutDefault.HasDefault())
        return 1;
#ifdef TEST_COMPILATION_ERRORS
    if (ClassSpecWithoutDefault.DefaultValue())
        return 1;
#else
    (void)ClassSpecWithoutDefault;
#endif

    StringWithDefault strdef;
    if (!strdef.IsDefaultValue())
        return 1;

    if (!strdef.GetElementSpec().HasDefault())
        return 1;

    strdef.SetValue("Other");
    if (strdef.IsDefaultValue())
        return 1;

    strdef.SetValue("Default Value");
    if (!strdef.IsDefaultValue())
        return 1;

    StringWithoutDefault strnodef;
    if (strnodef.IsDefaultValue())
        return 1;

    if (strnodef.GetElementSpec().HasDefault())
        return 1;

    strnodef.SetValue("Other");
    if (strnodef.IsDefaultValue())
        return 1;

    strnodef.SetValue("Default Value");
    if (strnodef.IsDefaultValue())
        return 1;

    EbmlElement & genericdef = test42;
    if (!genericdef.ElementSpec().HasDefault())
        return 1;

    if (!genericdef.IsDefaultValue())
        return 1;

    EbmlElement & genericnodef = strnodef;
    if (genericnodef.ElementSpec().HasDefault())
        return 1;

    if (genericnodef.IsDefaultValue())
        return 1;

    return 0;
}
