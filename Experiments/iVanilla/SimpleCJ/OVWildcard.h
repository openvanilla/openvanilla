// OVWildcard.h: Wildcard support (? and * in Regular Expression)
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
#include <ctype.h>

namespace OpenVanilla {
    using namespace std;

	class OVWildcard {
	public:
		enum Directive {
			Exact,
			AnyOne,
			AnyUntil
		};
		
		typedef pair<Directive, int> State;
		
		OVWildcard(const string& expression, char matchOneChar = 0, char matchZeroOrMoreChar = 0, bool matchEndOfLine = false)
		{
			m_matchEndOfLine = matchEndOfLine;
			
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
		
		bool match(const string& target)
		{
			string::size_type i = 0, slength = target.length();
			vector<State>::size_type j, vlength = m_states.size();
			
			for (j = 0; j < vlength; j++) {
				State state = m_states[j];
				Directive d = state.first;
				int k = state.second;

				if (i >= slength) {
					if (d == AnyUntil && !k) return true;
					return false;
				}
											
				switch (d) {
					case Exact:
						if (tolower(target[i]) != tolower(k)) return false;
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
							
							while (i < slength) {
								if (tolower(target[i]) == tolower(k)) {
									i++;
									found = true;
									break;
								}							
								i++;
							}
							
							if (!found) return false;
						}						
						
						break;
				}				
			}
			
			if (m_matchEndOfLine && i != slength) return false;
			return true;
		}
		
		friend ostream& operator<<(ostream& stream, const OVWildcard& wildcard);
		
	protected:
		bool m_matchEndOfLine;
		vector<State> m_states;
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