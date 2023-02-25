#include <ebml/EbmlElement.h>
#include <ebml/EbmlSInteger.h>
#include <ebml/MemIOCallback.h>

#include <iostream>
#include <iomanip>

using namespace libebml;

DECLARE_EBML_SINTEGER(EbmlInt)
  public:
        EBML_CONCRETE_CLASS(EbmlInt)
};

EbmlInt::EbmlInt()
{}
DEFINE_EBML_CLASS_GLOBAL(EbmlInt, 0xAA, 1, "EbmlSInt test")

static void printValue(std::int64_t i, int coded_size, const binary *read)
{
    std::cout << std::dec << i << ':' << coded_size << " [";
    for (int j=0; j<coded_size; j++)
        std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)read[j];
    std::cout << "]" << std::endl;
}

static void testValue(std::int64_t i)
{
    binary read[8];
    int coded_size = CodedSizeLengthSigned(i, 0);
    CodedValueLengthSigned(i, coded_size, read);
    printValue(i, coded_size, read);

    EbmlInt test;
    MemIOCallback memWriter;
    test.SetValue(i);
    test.UpdateSize(false, true);
    test.RenderData(memWriter, true);
    printValue(i, test.GetSize(), memWriter.GetDataBuffer());
}

int main(void)
{
    for (std::int64_t i = -129; i <= 129; i++)
    {
        testValue(i);
    }
    testValue(-254);
    testValue(-300);

    return 0;
}
