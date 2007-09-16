#ifndef PROFILEFETCHER_H
#define PROFILEFETCHER_H

#include <vector>

#include "Profile.h"

using namespace std;

class ProfileFetcher
{
public:
	vector<Profile>* fetch(const string& theKey);
};

#endif
