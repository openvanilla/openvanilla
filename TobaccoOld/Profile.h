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
	Profile(
		const pair<const string, const string>& theId,
		const vector<Token>& theTokens);
	Profile& operator=(const Profile& rhsProfile);

 	const pair<const string, const string>& id() const;
	const vector<Token>& tokens() const;

	size_t hitRate;
	bool isCustom;

private:
	const pair<const string, const string> m_id;
	const vector<Token> m_tokens;
};

#endif