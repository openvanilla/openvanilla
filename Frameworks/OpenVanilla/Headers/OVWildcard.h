//
// OVWildcard.h
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

#ifndef OVWildcard_h
#define OVWildcard_h

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

namespace OpenVanilla {
    using namespace std;

	class OVWildcard {
	public:
		OVWildcard(const string& expression, char matchOneChar = '?', char matchZeroOrMoreChar = '*', bool matchEndOfLine = true, bool caseSensitive = false)
		    : m_caseSensitive(caseSensitive)
		    , m_expression(expression)
		    , m_matchEndOfLine(matchEndOfLine)
		    , m_matchOneChar(matchOneChar)
		    , m_matchZeroOrMoreChar(matchZeroOrMoreChar)
		{
            size_t index;
            for (index = 0; index < expression.length(); index++) {
                if (expression[index] == matchOneChar || expression[index] == matchZeroOrMoreChar) break;
            }
            
            m_longestHeadMatchString = expression.substr(0, index);
            
			for (string::size_type i = 0; i < expression.length(); i++) {
				char c = expression[i];
				if (c == matchOneChar) {
					m_states.push_back(State(AnyOne, 0));
				}
				else if (c == matchZeroOrMoreChar) {
					char nextChar = 0;
					string::size_type j;
					for (j = i + 1; j < expression.length(); j++) {
						char k = expression[j];
						if (k != matchZeroOrMoreChar) {
							if (k == matchOneChar) k = -1;
						
							nextChar = k;
							break;
						}		
					}

					i = j;
					m_states.push_back(State(AnyUntil, nextChar));					
				}
				else {
					m_states.push_back(State(Exact, c));
				}
			}
		}

		bool match(const string& target, size_t fromState = 0) const
		{
			string::size_type i = 0, slength = target.length();
			vector<State>::size_type j, vlength = m_states.size();
			
			for (j = fromState; j < vlength; j++) {
				State state = m_states[j];
				Directive d = state.first;
				int k = state.second;

				if (i >= slength) {
					if (d == AnyUntil && !k) return true;
					return false;
				}
				
				switch (d) {
					case Exact:
						if (!equalChars(target[i], k)) return false;
						i++;
						break;

					case AnyOne:
						i++;
						break;

					case AnyUntil:
						if (k == -1) {
							// means *?, equals ?, so just advance one character
							i++;
						}
						else if (k == 0) {
							// until end, always true
							return true;
						}
						else {
							bool found = false;
                            string::size_type backIndex;
                            
                            for (backIndex = slength - 1; backIndex >= i; backIndex--) {
								if (equalChars(target[backIndex], k)) {
                                    string substring = target.substr(backIndex + 1, slength - (backIndex + 1));
                                    
                                    if (match(substring, j + 1)) {
                                        found = true;
                                        i = backIndex + 1;
                                        break;
                                    }
                                }
                                
                                if (!backIndex)
                                    break;
                            }
                            
                            if (!found)
                                return false;
						}						
						
						break;
				}				
			}
			
			if (m_matchEndOfLine && i != slength)
			    return false;

			return true;
		}
		
		const string longestHeadMatchString() const
		{
            return m_longestHeadMatchString;
		}
		
		const string expression() const
		{
            return m_expression;
		}
		
		bool isCaseSensitive() const
		{
            return m_caseSensitive;
		}
		
		char matchOneChar() const
		{
            return m_matchOneChar;
		}
		
		char matchZeroOrMoreChar() const
		{
            return m_matchZeroOrMoreChar;
		}
		
		friend ostream& operator<<(ostream& stream, const OVWildcard& wildcard);
		
	protected:
		enum Directive {
			Exact,
			AnyOne,
			AnyUntil
		};
		
		typedef pair<Directive, int> State;
		
		bool equalChars(char a, char b) const
		{
		    if (m_caseSensitive)
                return a == b;
            else
                return tolower(a) == tolower(b);
		}
	    
        bool m_caseSensitive;
		bool m_matchEndOfLine;
        char m_matchOneChar;
        char m_matchZeroOrMoreChar;
		vector<State> m_states;
		
        string m_expression;
        string m_longestHeadMatchString;
        
    public:
        static const bool Match(const string& text, const string& expression, char matchOneChar = '?', char matchZeroOrMoreChar = '*', bool matchEndOfLine = true, bool caseSensitive = false)
        {
            OVWildcard exp(expression, matchOneChar, matchZeroOrMoreChar, matchEndOfLine, caseSensitive);            
            return exp.match(text);
        }
        
        static const vector<OVWildcard> WildcardsFromStrings(const vector<string>& expressions, char matchOneChar = '?', char matchZeroOrMoreChar = '*', bool matchEndOfLine = true, bool caseSensitive = false)
        {
            vector<OVWildcard> result;
            vector<string>::const_iterator iter = expressions.begin();
            for ( ; iter != expressions.end(); iter++)
                result.push_back(OVWildcard(*iter, matchOneChar, matchZeroOrMoreChar, matchEndOfLine, caseSensitive));
            
            return result;
        }
        
        static bool MultiWildcardMatchAny(const string& target, const vector<string>& expressions, char matchOneChar = '?', char matchZeroOrMoreChar = '*', bool matchEndOfLine = true, bool caseSensitive = false)
        {
            return MultiWildcardMatchAny(target, WildcardsFromStrings(expressions, matchOneChar, matchZeroOrMoreChar, matchEndOfLine, caseSensitive));
        }
        
        static bool MultiWildcardMatchAny(const string& target, const vector<OVWildcard>& expressions)
        {
            vector<OVWildcard>::const_iterator iter = expressions.begin();
            for ( ; iter != expressions.end(); iter++) {
                if ((*iter).match(target))
                    return true;
            }
            
            return false;
        }
	};

	inline ostream& operator<<(ostream& stream, const OVWildcard& wildcard)
	{
		vector<OVWildcard::State>::size_type i, size = wildcard.m_states.size();
		for (i = 0; i < size; i++) {
			const OVWildcard::State& state = wildcard.m_states[i];
			stream << "State " << i << ": " << state.first << ", " << state.second << endl;
		}
		
		return stream;
	}
};

#endif