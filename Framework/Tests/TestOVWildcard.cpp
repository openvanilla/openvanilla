//
// TestOVWildcard.cpp
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

TEST(WildcardCorrectness)
{
    CHECK(OVWildcard::Match("foobarfoobarfoobar123", "*bar?2", '?', '*', false));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "f*"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "*bar"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar123", "*bar123"));
    CHECK(OVWildcard::Match("foobar123foobarfoobar123", "*bar?2*bar?o", '?', '*', false));
    CHECK(OVWildcard::Match("foobarfoobarfoobar123", "*ar*"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "*ar*"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "*ar*ar"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar2", "*ar?"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "f*r"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "f*ar"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "foo*"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "foobar*foobar"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "?o*"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "??*"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "f*bar*oo*a*r"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "f*bar*oo*a*"));
    CHECK(OVWildcard::Match("foobarfoobarfoobar", "f*a??", '?', '*', false));    
    CHECK(!OVWildcard::Match("foobarfoobarfoobar", "f*1"));
    CHECK(!OVWildcard::Match("foobarfoobarfoobar", "f*a??"));
}
