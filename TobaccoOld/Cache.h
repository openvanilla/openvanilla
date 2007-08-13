#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <map>
#include <deque>

#include "Profile.h"

using namespace std;

class Cache
{
public:
	bool isSaved;

	static Cache* getInstance();
	void releaseInstance();

	void add(Profile& theProfile);
 	bool remove(const pair<const string, const string>& theId);
	vector<Profile>* fetch(const string& theKey);

private:
	static Cache* m_instance;
	map<string, vector<Profile> > m_table;

	static const size_t MAX_CACHE_SIZE = 1000;
};

#endif