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
	Profile(const string& theId, const vector<Token>& theTokens);

 	const string& id() const;
	const vector<Token>& tokens() const;

	size_t position;
	size_t hitRate;
	bool isCustom;

private:
	const string m_id;
	const vector<Token> m_tokens;
};

#endif