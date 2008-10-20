//
// OVStringHelper.h
//
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

#ifndef OVStringHelper_h
#define OVStringHelper_h

#include <string>

namespace OpenVanilla {
    using namespace std;

    class OVStringHelper {
    public:
        static const vector<string> SplitBySpacesOrTabsWithDoubleQuoteSupport(const string& text)
        {
            vector<string> result;            
            size_t index = 0, last = 0, length = text.length();
            while (index < length) {
				if (text[index] == '\"') {
					index++;
					string tmp;
					while (index < length) {
						if (text[index] == '\"') {
							index++;
							break;
						}
						
						if (text[index] == '\\' && index + 1 < length) {
							index++;
							char c = text[index];
							switch (c) {
							case 'r':
								tmp += '\r';
								break;
							case 'n':
								tmp += '\n';
								break;
							case '\"':
								tmp += '\"';
								break;
							case '\\':
								tmp += '\\';
								break;
							}
						}
						else {
							tmp += text[index];
						}
						
						index++;
					}
					result.push_back(tmp);
				}
	
                if (text[index] != ' ' && text[index] != '\t') {                    
                    last = index;
                    while (index < length) {
                        if (text[index] == ' ' || text[index] == '\t') {
                            if (index - last)
                                result.push_back(text.substr(last, index - last));
                            break;
                        }
                        index++;
                    }
                    
                    if (index == length && index - last)
                        result.push_back(text.substr(last, index - last));
                }
                
                index++;
            }
            
            return result;
        }
	
        static const vector<string> SplitBySpacesOrTabs(const string& text)
        {
            vector<string> result;            
            size_t index = 0, last = 0, length = text.length();
            while (index < length) {
                if (text[index] != ' ' && text[index] != '\t') {                    
                    last = index;
                    while (index < length) {
                        if (text[index] == ' ' || text[index] == '\t') {
                            if (index - last)
                                result.push_back(text.substr(last, index - last));
                            break;
                        }
                        index++;
                    }
                    
                    if (index == length && index - last)
                        result.push_back(text.substr(last, index - last));
                }
                
                index++;
            }
            
            return result;
        }
        
        static const vector<string> Split(const string& text, char c)
        {
            vector<string> result;
            size_t index = 0, last = 0, length = text.length();
            while (index < length) {
                last = index;
                while (index < length) {
                    if (text[index] == c) {
                        result.push_back(text.substr(last, index - last));
                        break;
                    }
                    index++;
                    
                    if (index == length && index - last)
                        result.push_back(text.substr(last, index - last));           
                }

                index++;
            }

            return result;
        }
        
        // named after Cocoa's NSString -stringByReplacingOccurrencesOfString:WithString:, horrible
        static const string StringByReplacingOccurrencesOfStringWithString(const string& source, const string& substr, const string& replacement)
        {
            if (!substr.length())
                return source;
            
            size_t pos;
            if ((pos = source.find(substr)) >= source.length())
                return source;

            return source.substr(0, pos) + replacement + StringByReplacingOccurrencesOfStringWithString(source.substr(pos + substr.length()), substr, replacement);
        }

        static const string Join(const vector<string>& vec, const string& separator)
        {
            return Join(vec.begin(), vec.end(), separator);
        }

        static const string Join(const vector<string>& vec, size_t from, size_t size, const string& separator)
        {
            return Join(vec.begin() + from, vec.begin() + from + size, separator);
        }

        static const string Join(vector<string>::const_iterator begin, vector<string>::const_iterator end, const string& separator)
        {
            string result;
            for (vector<string>::const_iterator iter = begin ; iter != end ; ) {
                result += *iter;
                if (++iter != end)
                    result += separator;
            }
            return result;
        }    
    };
};

#endif