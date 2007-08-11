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
	Profile(const string& id, const vector<Token>& tokens);

 	const string& id();
	const vector<Token>& tokens();

	size_t position;
	size_t hitRate;
	bool isCustom;

private:
	string m_id;
	vector<Token> m_tokens;
};

#endif