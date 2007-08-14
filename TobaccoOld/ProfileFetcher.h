#ifndef PROFILEFETCHER_H
#define PROFILEFETCHER_H

#include "Profile.h"

using namespace std;

class ProfileFetcher
{
public:
	vector<Profile>* fetch(const string& theKey);
};

#endif