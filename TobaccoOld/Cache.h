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

	void add(const Profile& profile);
 	Profile* fetch(string id);
	vector<Profile>* fetchAll(string id);
 	bool remove(string id, string word);

	size_t count(string id);

	void rank(const Profile& profile);

	Cache::Cache();
	Cache::~Cache();

private:
	//void clearTable();

	static Cache* m_instance;
	map<string, vector<Profile> > m_table;
	deque<string> m_recentlyUsedList;	//±Æ§Ç¥Î

	size_t m_currentCacheSize;
	static const size_t MAX_CACHE_SIZE;
};

#endif