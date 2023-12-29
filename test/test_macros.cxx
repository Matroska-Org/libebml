#include <ebml/EbmlContexts.h>
#include <ebml/EbmlString.h>
#include <ebml/EbmlUInteger.h>
#include <ebml/EbmlUnicodeString.h>

using namespace libebml;

DECLARE_EBML_STRING(StringWithDefault)
    EBML_CONCRETE_CLASS(StringWithDefault)
};
DEFINE_EBML_STRING_DEF(StringWithDefault, 0x4321, 2, EbmlHead, "StringWithDefault", "Default Value")

DECLARE_EBML_STRING(StringWithoutDefault)
    EBML_CONCRETE_CLASS(StringWithoutDefault)
};
DEFINE_xxx_STRING(StringWithoutDefault, 0x4123, 2, EbmlHead, "StringWithoutDefault", GetEbmlGlobal_Context)

DECLARE_xxx_UNISTRING(UniStringWithDefault,)
    EBML_CONCRETE_CLASS(UniStringWithDefault)
};
DEFINE_xxx_UNISTRING_DEF(UniStringWithDefault, 0x4321, 2, EbmlHead, "UniStringWithDefault", GetEbmlGlobal_Context, UTFstring{L"Default Value"})

DECLARE_xxx_UNISTRING(UniStringWithoutDefault,)
    EBML_CONCRETE_CLASS(UniStringWithoutDefault)
};
DEFINE_xxx_UNISTRING(UniStringWithoutDefault, 0x4123, 2, EbmlHead, "UniStringWithoutDefault", GetEbmlGlobal_Context)

DECLARE_EBML_UINTEGER(UIntWithDefault)
    EBML_CONCRETE_CLASS(UIntWithDefault)
};
DEFINE_EBML_UINTEGER_DEF(UIntWithDefault, 0x654321, 3, EbmlHead, "UIntWithDefault", 42)

DECLARE_EBML_UINTEGER(UIntWithoutDefault)
    EBML_CONCRETE_CLASS(UIntWithoutDefault)
};
DEFINE_xxx_UINTEGER(UIntWithoutDefault, 0x612345, 3, EbmlHead, "UIntWithoutDefault", GetEbmlGlobal_Context)

int main(void)
{
    UIntWithDefault test0;
    test0.SetValue(0);
    if (test0.IsDefaultValue())
        return 1;

    UIntWithDefault test42;
    test42.SetValue(42);
    if (!test42.IsDefaultValue())
        return 1;

    if (!test42.DefaultISset())
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

    StringWithDefault strdef;
    if (!strdef.IsDefaultValue())
        return 1;

    if (!strdef.DefaultISset())
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

    if (strnodef.DefaultISset())
        return 1;

    strnodef.SetValue("Other");
    if (strnodef.IsDefaultValue())
        return 1;

    strnodef.SetValue("Default Value");
    if (strnodef.IsDefaultValue())
        return 1;

    EbmlElement & genericdef = test42;
    if (!genericdef.DefaultISset())
        return 1;

    if (!genericdef.IsDefaultValue())
        return 1;

    EbmlElement & genericnodef = strnodef;
    if (genericnodef.DefaultISset())
        return 1;

    if (genericnodef.IsDefaultValue())
        return 1;

    return 0;
}
