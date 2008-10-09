// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef OVUTF8Helper_h
#define OVUTF8Helper_h

#include <cstddef>
#include <string>
#include <vector>

#include <iostream>

namespace OpenVanilla {
    using namespace std;
    
    class OVUTF8Helper {
    public:
        static const vector<string> SplitStringByCodePoint(const string& utf8String)
        {
            size_t index, length = utf8String.length();
            vector<string> result;
            
            for (index = 0; index < length; index++) {
                unsigned char c = utf8String[index];
                
                if (!(c & 0x80)) {
                    result.push_back(utf8String.substr(index, 1));
                }
                else if ((c & (0x80 | 0x40)) && !(c & 0x20)) {
                    result.push_back(utf8String.substr(index, 2));
                    index += 1;
                }
                else if ((c & (0x80 | 0x40 | 0x20)) && !(c & 0x10)) {
                    result.push_back(utf8String.substr(index, 3));
                    index += 2;
                }
                else if ((c & (0x80 | 0x40 | 0x20 | 0x10)) && !(c & 0x8)) {
                    result.push_back(utf8String.substr(index, 4));
                    index += 3;
                }                
            }
            
            return result;
        }
        
        static const string CombineCodePoints(const vector<string>& codePoints)
        {
            string result;
            for (vector<string>::const_iterator iter = codePoints.begin(); iter != codePoints.end() ; ++iter)
                result += *iter;
                
            return result;
        }
        
        static const wstring ConvertStringToUTF16WideString(const string& utf8String)
        {
            wstring wideString;
            vector<string> charVector = OVUTF8Helper::SplitStringByCodePoint(utf8String);
            
            size_t length = charVector.size();
            for (size_t index = 0; index < length; index++)
            {
                unsigned int codePoint = OVUTF8Helper::CodePointFromSingleUTF8String(charVector[index]);
                
                if (codePoint < 0x10000) {
                    wideString += (wchar_t)codePoint;
                }
                else {
                    const unsigned int leadOffset = 0xD800 - (0x10000 >> 10);
                    unsigned int lead = leadOffset + (codePoint >> 10);
                    unsigned int trail = 0xdc00 + (codePoint & 0x3ff);
                    wideString += (wchar_t)lead;
                    wideString += (wchar_t)trail;
                }
            }
            
            return wideString;
        }
                
        static const string StringFromUTF16WideString(const wstring& utf16WideString)
        {
            string utf8String;
            size_t length = utf16WideString.length();
            
            unsigned int u16;
            for (size_t index = 0; index < length; index++) {
                u16 = utf16WideString[index];
                
                if (u16 >= 0xd800 && u16 <= 0xdbff) {
                    if (index + 1 < length) {
                        unsigned int trailing = utf16WideString[index + 1];
                        if (trailing >= 0xdc00 && trailing <= 0xdfff) {
                            index++;
                            
                            const unsigned int surrogateOffset = 0x10000 - (0xD800 << 10) - 0xDC00;
                            unsigned int codePoint = (u16 << 10) + trailing + surrogateOffset;
                            utf8String += OVUTF8Helper::SingleUTF8StringFromCodePoint(codePoint);
                        }
                    }
                }
                else if (!(u16 >= 0xdc00 && u16 <= 0xdfff))
                {
                    utf8String += OVUTF8Helper::SingleUTF8StringFromCodePoint(u16);
                }
            }
            
            return utf8String;
        }
        
        static const string SingleUTF8StringFromCodePoint(unsigned int codePoint)
        {
            string utf8String;
            
            if (codePoint < 0x80) {
                utf8String += (unsigned char)codePoint;
            }
            else if (codePoint < 0x800) {
                utf8String += (unsigned char)((codePoint >> 6) | 0xc0);
                utf8String += (unsigned char)((codePoint & 0x3f) | 0x80);
            }
            else if (codePoint < 0x10000) {
                utf8String += (unsigned char)((codePoint >> 12) | 0xe0);
                utf8String += (unsigned char)((codePoint >> 6) & 0x3f | 0x80);
                utf8String += (unsigned char)((codePoint & 0x3f) | 0x80);                
            }
            else {
                utf8String += (unsigned char)((codePoint >> 18) | 0xf0);
                utf8String += (unsigned char)((codePoint >> 12) | 0x80);
                utf8String += (unsigned char)((codePoint >> 6) & 0x3f | 0x80);
                utf8String += (unsigned char)((codePoint & 0x3f) | 0x80);                
            }
            
            return utf8String;
        }
        
        static unsigned int CodePointFromSingleUTF8String(const string& utf8String)
        {
            switch(utf8String.length()) {
                case 1:
                    return (unsigned int)utf8String[0];
                case 2:
                    return (((unsigned char)utf8String[0] << 6) & 0x7ff) + ((unsigned char)utf8String[1] & 0x3f);
                case 3:
                    return (((unsigned char)utf8String[0] << 12) & 0xffff) + (((unsigned char)utf8String[1] << 6) & 0xfff) + ((unsigned char)utf8String[2] & 0x3f);
                case 4:
                    return (((unsigned char)utf8String[0] << 18) & 0x1fffff) + (((unsigned char)utf8String[1] << 12) & 0x3ffff) + (((unsigned char)utf8String[2] << 6) & 0xfff) + ((unsigned char)utf8String[3] & 0x3f); 
            }
        
            return 0;
        }
    };
    
    class OVUTF8 {
    public:
        static const string FromUTF16(const wstring& utf16String)
        {
            return OVUTF8Helper::StringFromUTF16WideString(utf16String);
        }
    };
    
    class OVUTF16 {
    public:
        static const wstring FromUTF8(const string& utf8String)
        {
            return OVUTF8Helper::ConvertStringToUTF16WideString(utf8String);
        }
    };
};

#endif
