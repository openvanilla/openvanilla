#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "Cache.h"
#include "ProfileFetcher.h"

using namespace std;

class ProfileManager {
public:
	static ProfileManager* getInstance();
	void releaseInstance();
  
	vector<Profile>* fetch(const string& theKey);

	void writeBack();
			
	ProfileManager();
	~ProfileManager();

private:
	static ProfileManager* m_instance;

	Cache* m_cache;
	ProfileFetcher m_fetcher;
};

#endif