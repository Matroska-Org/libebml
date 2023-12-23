// Copyright © 2023 Steve Lhomme.
// SPDX-License-Identifier: ISC

#include <ebml/EbmlUnicodeString.h>

using namespace libebml;

constexpr char emoji_8[] = "\xF0\x9F\x98\x80";
constexpr wchar_t emoji_w[] = L"\U0001f600";

int main(void)
{
    UTFstring ascii{ L"latin1" };

    if (ascii != L"latin1")
        return 1;

    if (wcscmp(ascii.c_str(), L"latin1") != 0)
        return 1;

    if (ascii.GetUTF8() != "latin1")
        return 1;

    UTFstring utf8;
    utf8.SetUTF8( emoji_8 );

    if (utf8.length() != (4 / sizeof(wchar_t)))
        return 1;

    if (utf8 != emoji_w)
        return 1;

    // UTFstring invalid;
    // FIXME don't crash invalid.SetUTF8( "\x1\xF6\x00" );

    UTFstring empty{0};
    if (empty.length() != 0)
        return 1;

    UTFstring padded{L"padded\0\0"};
    if (padded.length() != 6)
        return 1;

    if (padded != L"padded")
        return 1;

    UTFstring copy = utf8;
    if (copy.length() != (4 / sizeof(wchar_t)))
        return 1;

    if (copy != emoji_w)
        return 1;

    if (copy.c_str() == utf8.c_str())
        return 1;

    UTFstring copy2(utf8);
    if (copy2 != emoji_w)
        return 1;

    if (copy2.c_str() == utf8.c_str())
        return 1;

    return 0;
}
