// Copyright © 2024 Steve Lhomme.
// SPDX-License-Identifier: ISC

#include <ebml/EbmlHead.h>
#include <ebml/EbmlMaster.h>
#include <ebml/EbmlStream.h>
#include <ebml/MemIOCallback.h>

int main(int /*argc*/, char** /*argv*/)
{
    ///// Writing test
    libebml::MemIOCallback Ebml_file;
    libebml::EbmlHead TestHead;

    TestHead.EnableChecksum();

    libebml::EDocTypeVersion & MyDocTypeVer = libebml::GetChild<libebml::EDocTypeVersion>(TestHead);
    MyDocTypeVer.SetValue(1);

    libebml::GetChild<libebml::EMaxSizeLength>(TestHead).SetValue(7);

    auto length = TestHead.Render(Ebml_file, libebml::EbmlElement::WriteAll);
    if (length != 46)
        return 1;

    ///// Reading test
    Ebml_file.setFilePointer(0);
    libebml::EbmlStream aStream(Ebml_file);
    libebml::EbmlElement * ElementLevel0;

    // find the EBML head in the file
    ElementLevel0 = aStream.FindNextID(EBML_INFO(libebml::EbmlHead), 0xFFFFFFFFL);
    if (ElementLevel0 == nullptr)
        return 1;

    if (ElementLevel0->GetClassId() != libebml::EbmlHead::ClassId())
        return 1;

    libebml::EbmlHead &ReadHead = static_cast<libebml::EbmlHead &>(*ElementLevel0);

    ReadHead.ReadData(aStream, libebml::SCOPE_ALL_DATA);

    if (!ReadHead.VerifyChecksum())
        return 1;

    libebml::EDocType & ReadDocType = libebml::GetChild<libebml::EDocType>(ReadHead);
    const std::string & DocTypeStr = static_cast<const std::string &>(ReadDocType);
    if (DocTypeStr != "matroska")
        return 1;

    auto & ReadReadVersion = libebml::GetChild<const libebml::EDocTypeReadVersion>(ReadHead);
    if (static_cast<std::uint64_t>(ReadReadVersion) != 1)
        return 1;

    auto & ReadMaxLength = libebml::GetChild<const libebml::EMaxSizeLength>(ReadHead);
    if (static_cast<std::uint64_t>(ReadMaxLength) != 7)
        return 1;

    return 0;
}
