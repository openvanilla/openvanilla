// OVUtility.h: Inline utilities
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __OVUtility_h
#define __OVUtility_h

#include <ctime>
using namespace std;

class Watch
{
public:
	void start()
	{
		tick_ = 0;
		sec_ = 0.0;		
		from_ = clock();
	}

	void stop()
	{
		tick_ = clock()-from_;		
		sec_ = double(tick_)/CLOCKS_PER_SEC;
	}

	clock_t getTick() { return tick_; }
	double getSec() { return sec_; }

private:
	clock_t from_;
	clock_t tick_;
	double sec_;
};

// debug routine
#ifdef OV_DEBUG 
    #include <stdarg.h>
    #include <stdio.h>

    // Thanks to modern C++ compiler, we can write this in the header file. 
    // No extra compiling/library making/linking is even needed!

    inline static void murmur(const char* format,...) {
#ifdef WIN32
		FILE *fp;
		if (fp=fopen("C:\\WINDOWS\\OpenVanilla\\orz.txt", "a")) {
			va_list args;
			va_start (args, format);
			vfprintf (fp, format, args);
			va_end (args);
			fprintf (fp, "\n");
			fclose(fp);
		}
#else
        va_list args;
        va_start (args, format);
        vfprintf (stderr, format, args);
        va_end (args);
        fprintf (stderr, "\n");
#endif
    }
#else
    inline static void murmur(const char* format,...) {
    }
#endif

#endif
