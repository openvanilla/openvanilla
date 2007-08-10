#ifndef PROFILEFETCHER_H
#define PROFILEFETCHER_H

#include "Profile.h"

using namespace std;

class ProfileFetcher
{
	public:
		Profile* fetch(const string& id);
};

#endif