#ifndef PROFILE_H
#define PROFILE_H

#include <string>

using namespace std;

struct ProfileId
{
	ProfileId(const string& theKeystrokes, const string& thePattern);
	ProfileId(const ProfileId& theId);

	bool operator==(const ProfileId& rhsProfileId);

	string keystrokes;
	string pattern;
};

struct Profile 
{
	Profile(const ProfileId& theId);
	Profile(const Profile& theProfile);

 	ProfileId id;
	size_t hitRate;
	bool isCustom;
};

#endif
