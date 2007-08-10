#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include "Token.h"

using namespace std;

class Profile 
{
	friend class ProfileFetcher;
	friend class ProfileManager;

public:
		Profile(const string& id, size_t position, const vector<Token>& tokens);
 		const string& id();
		const string& word();
		size_t currentPosition();
		Token* tokenAt(size_t position);
		size_t tokenSize();
		size_t hitRate();
		void setHitRate(size_t hits);
		bool isCustom();
		void setCustomFlag(bool isCustom);
		
private:
		string m_id;
		string m_word;
		vector<Token> m_tokens;
		size_t m_currentPosition;
		size_t m_hitRate;
		bool m_isCustom;
};

#endif