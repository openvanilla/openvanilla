#include <algorithm> // for "std::find() used on deque"
#include "Cache.h"

using namespace std;

Cache* Cache::m_instance = 0;

/*
struct DeleteObject {
	template<typename T>
	void operator()(const T* ptr) const
	{
		delete ptr;
	}
};
*/

Cache* Cache::getInstance()
{
	if(!m_instance)	m_instance = new Cache();

	return m_instance;
}

void Cache::releaseInstance()
{
	if(m_instance) delete m_instance;
}

void Cache::add(Profile& theProfile)
{
	if(m_table.count(theProfile.id().first) == 0)
	{
		vector<Profile> theProfiles;
		theProfiles.push_back(theProfile);
		m_table.insert(make_pair(theProfile.id().first, theProfiles));
	}
	else
		m_table[theProfile.id().first].push_back(theProfile);
}

vector<Profile>* Cache::fetch(const string& theKey)
{
	if(m_table.count(theKey) > 0)
		return &m_table[theKey];
	else
		return 0;
}

void Cache::update(const pair<const string, const string>& theId)
{
	vector<Profile>* profiles = fetch(theId.first);
	if(profiles)
	{
		for(vector<Profile>::iterator i = profiles->begin();
			i != profiles->end();
			++i)
		{
			if(i->id() == theId) {
				++i->hitRate;
				return;
			}
		}
	}
	else
	{
		//@todo add new Profiles here
		return;
	}
}

bool Cache::remove(const pair<const string, const string>& theId)
{
	vector<Profile>* profiles = fetch(theId.first);
	if(profiles)
	{
		for(vector<Profile>::iterator i = profiles->begin();
			i != profiles->end();
			++i)
		{
			if(i->id() == theId) {
				profiles->erase(i);
				if(profiles->empty())
					m_table.erase(theId.first);
				return true;
			}
		}
	}

	return false;
}
