//
// TestOVUTF8Helper.cpp
//
// Copyright (c) 2007-2010 Lukhnos D. Liu
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#if defined(__APPLE__)
    #include <OpenVanilla/OpenVanilla.h>
#else    
    #include "OpenVanilla.h"
#endif

#include "UnitTest++.h"

using namespace OpenVanilla;

ostream& operator<<(ostream&s, const wstring& str)
{
    ios_base::fmtflags flags = s.flags();
    
    size_t index, length = str.length();
    s << "wstring (length=" << length << ", content=";
    
    cout.width(4);
    cout.fill('0');
    s << hex;
    
    for (index = 0; index < length; index++) {
        s << str[index];
        
        if (index != length-1) {
            s << " ";
        }
    }
    
    s << ")";

    s.flags(flags);
    return s;
}

struct UTF8Fixture {
    UTF8Fixture()
    {
        // test string == aÃ¡?‚ð?‰ð??(UTF-8, this is hated by Visual Studio 2005)
        testString = "\x61";                // U+0061
        testString += "\xc3\xa1";           // U+00E1
        testString += "\xe3\x81\x82";       // U+3042
        testString += "\xf0\x90\x85\x89";   // U+10149 (D800 DD49)
        testString += "\xf0\xa0\x85\x98";   // U+20158 (D840 DD58)        
    }
    ~UTF8Fixture()
    {
    }
    
    string testString;
};

TEST_FIXTURE(UTF8Fixture, UTF8StringLength)
{
    CHECK_EQUAL(14, testString.length());
}

TEST_FIXTURE(UTF8Fixture, UTF8StringSplit)
{
    vector<string> splitted = OVUTF8Helper::SplitStringByCodePoint(testString);
    CHECK_EQUAL(5, splitted.size());
}

TEST_FIXTURE(UTF8Fixture, UTF16StringConversion)
{
    wstring utf16String = OVUTF8Helper::ConvertStringToUTF16WideString(testString);
    CHECK_EQUAL(7, utf16String.length());
    CHECK_EQUAL(0x0061, utf16String[0]);
    CHECK_EQUAL(0x00e1, utf16String[1]);
    CHECK_EQUAL(0x3042, utf16String[2]);
    CHECK_EQUAL(0xd800, utf16String[3]);
    CHECK_EQUAL(0xdd49, utf16String[4]);
    CHECK_EQUAL(0xd840, utf16String[5]);
    CHECK_EQUAL(0xdd58, utf16String[6]);
}

TEST_FIXTURE(UTF8Fixture, OVUTF16AliasEquality)
{
    wstring utf16String1 = OVUTF8Helper::ConvertStringToUTF16WideString(testString);
    wstring utf16String2 = OVUTF16::FromUTF8(testString);

    CHECK_EQUAL(utf16String1, utf16String2);
    
    CHECK_ARRAY_EQUAL(utf16String1, utf16String2, 7);
}

TEST_FIXTURE(UTF8Fixture, RoundTripConversion)
{
    wstring utf16String = OVUTF8Helper::ConvertStringToUTF16WideString(testString);
    string backString = OVUTF8Helper::StringFromUTF16WideString(utf16String);
    CHECK_EQUAL(testString, backString);
}

TEST_FIXTURE(UTF8Fixture, OVUTF8AliasEquality)
{
    wstring utf16String = OVUTF8Helper::ConvertStringToUTF16WideString(testString);
    string backString1 = OVUTF8Helper::StringFromUTF16WideString(utf16String);
    string backString2 = OVUTF8::FromUTF16(utf16String);
    CHECK_EQUAL(backString1, backString2);
}
