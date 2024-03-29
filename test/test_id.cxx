// Copyright © 2023 Steve Lhomme.
// SPDX-License-Identifier: ISC

#include <ebml/EbmlId.h>

using namespace libebml;

static constexpr EbmlId MyId = EbmlId{0x654321};

class KaxTracks {
public:
    static const EbmlId & ClassId() { return MyId; }
};

int main(void)
{
    constexpr binary buf[4] = { 0x12, 0x34, 0x56, 0x78 };
    auto frombuf = EbmlId(EbmlId::FromBuffer(buf, 4));

    EbmlId fromint(0x12345678);

    EbmlId copy(fromint);
    binary read[4];
    copy.Fill(read);
    if (read[0] != 0x12 || read[1] != 0x34 || read[2] != 0x56 || read[3] != 0x78)
        return 1;

    EbmlId eq = fromint;
    eq.Fill(read);
    if (read[0] != 0x12 || read[1] != 0x34 || read[2] != 0x56 || read[3] != 0x78)
        return 1;

    EbmlId id{42};
    if (id.GetLength() != 1)
        return 1;

    KaxTracks tracks;
    id = tracks.ClassId();
    if (id.GetLength() != 3)
        return 1;

    constexpr EbmlId test1{0xA3};
    static_assert(test1.GetLength() == 1, "nope");
    constexpr EbmlId test2{0x5854};
    static_assert(test2.GetLength() == 2, "nope");
    constexpr EbmlId test3{0x654000};
    static_assert(test3.GetLength() == 3, "nope");
    constexpr EbmlId test4{0x1A45DFA3};
    static_assert(test4.GetLength() == 4, "nope");

    return 0;
}
