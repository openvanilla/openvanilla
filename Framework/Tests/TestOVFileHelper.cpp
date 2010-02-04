//
// TestOVFileHelper.cpp
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
#include <fstream>

using namespace OpenVanilla;

TEST(FileHelper)
{
    string tmp = OVDirectoryHelper::TempDirectory();
    cout << tmp << endl;
    
    // filename is in Kanji "document" (2 chars) and a spring symbol
    string mytmp = OVPathHelper::PathCat(tmp, "___TestOVFileHelper-\xe6\x96\x87\xe4\xbb\xb6\xe2\x99\xa8");
    // cout << mytmp << endl;

    CHECK(!OVPathHelper::PathExists(mytmp));    
    CHECK(OVDirectoryHelper::MakeDirectory(mytmp));
    CHECK(OVPathHelper::PathExists(mytmp));    

    string path1 = OVPathHelper::PathCat(mytmp, "1");
    string path2 = OVPathHelper::PathCat(mytmp, "2");

    // cout << path1 << endl;
    // cout << path2 << endl;
    
    CHECK(OVPathHelper::IsDirectory(mytmp));
    CHECK(!OVPathHelper::IsDirectory(path1));
    CHECK(!OVPathHelper::IsDirectory(path2));
    CHECK(!OVPathHelper::PathExists(path1));
    CHECK(!OVPathHelper::PathExists(path2));
    
	FILE* f1 = 0;
	FILE* f2 = 0;
	#ifndef WIN32
	f1 = fopen(path1.c_str(), "w");
	f2 = fopen(path2.c_str(), "w");
	#else
	wstring wpath1 = OVUTF16::FromUTF8(path1);
	wstring wpath2 = OVUTF16::FromUTF8(path2);
	int e1 = _wfopen_s(&f1, wpath1.c_str(), L"w");
	int e2 = _wfopen_s(&f2, wpath2.c_str(), L"w");
	#endif

	CHECK(f1);
	CHECK(f2);

	if (f1 && f2) {
		fprintf(f1, "hello, world\n");
		fprintf(f2, "lorem ipsum\n");
		fclose(f1);
		#ifndef WIN32
		sleep(1);
		#else
		Sleep(1000);
		#endif
		fclose(f2);

		CHECK(!OVPathHelper::IsDirectory(path1));
		CHECK(!OVPathHelper::IsDirectory(path2));
		CHECK(OVPathHelper::PathExists(path1));
		CHECK(OVPathHelper::PathExists(path2));
	    
		OVFileTimestamp t1 = OVPathHelper::TimestampForPath(path1);
		OVFileTimestamp t2 = OVPathHelper::TimestampForPath(path2);
		CHECK(t1 < t2);

		#ifndef WIN32
		sleep(1);
		#else
		Sleep(1000);
		#endif

	    
		f1 = 0;
		#ifndef WIN32
		f1 = fopen(path1.c_str(), "w");
		#else
		e1 = _wfopen_s(&f1, wpath1.c_str(), L"w");
		#endif

		if (f1) {
			fprintf(f1, "new string\n");
			fclose(f1);
			t1 = OVPathHelper::TimestampForPath(path1);
			CHECK(t1 > t2);
		}
	}
    
    CHECK(OVPathHelper::RemoveEverythingAtPath(mytmp));
    CHECK(!OVPathHelper::PathExists(mytmp));        
}
