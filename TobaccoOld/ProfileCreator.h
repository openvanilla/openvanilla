#ifndef PROFILECREATOR_H
#define PROFILECREATOR_H

#include "Profile.h"

using namespace std;

class ProfileCreator
{
public:
	void create(
		const string& theKey, const string& theWord,
		const vector<Token>& tokens);
};

#endif