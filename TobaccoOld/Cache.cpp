#include <algorithm> // for "std::find() used on deque"
#include "Cache.h"

using namespace std;

const size_t Cache::MAX_CACHE_SIZE = 1000;
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

Cache::Cache()
{
	m_currentCacheSize = 0;
}

Cache::~Cache()
{
	//clearTable();
	m_table.clear();
	m_recentlyUsedList.clear();
	m_currentCacheSize = 0;
}

Cache* Cache::getInstance()
{
	if(!m_instance)
	{
		m_instance = new Cache();
	}

	return m_instance;
}

void Cache::releaseInstance()
{
	if(m_instance)
		Cache::~Cache();
}

/*
void Cache::clearTable()
{
	for(map<wstring, vector<Profile*> >::iterator iter=Cache::table.begin();
		iter != Cache::table.end();
		++iter)
			for_each(iter->second.begin(), iter->second.end(), DeleteObject);

	Cache::m_table.clear();
}
*/

void Cache::rank(const string& id, const string& word)
{
	if(m_table.count(id) > 0) {
		vector<Profile> profileVector = m_table[id];
		for(size_t i = 0; i < profileVector.size(); ++i)
		{
			Profile existedProfile = profileVector[i];
			size_t currentHitRate = existedProfile.hitRate;
			//@author b6s
			//@todo Refactoring is needed.
			/*
			if(existedProfile.word() == word)
				m_table[id][i].hitRate = ++currentHitRate;
			else if(currentHitRate > 0)
				m_table[id][i].hitRate = --currentHitRate;
			*/
		}
	}
}

void Cache::add(Profile& profile)
{
	string id = profile.id();
	if (m_table.count(id) == 0)
	{
		//先取消 MAX_CACHE_SIZE 的判斷
		m_recentlyUsedList.push_back(id);
//		/*
//		cache 還有空間的話就直接擺到最後面。
//		這裡只是先對排序用的 deque 作記錄。
//		*/
//		if (currentCacheSize < MAX_CACHE_SIZE || Cache::recentlyUsedList.size() == 0)
//		{
//			// 20060318
//			// Cache::recentlyUsedList.size() == 0 會發生在使用者之前存檔的記錄其實已超過 MAX_CACHE_SIZE 時
//
//			Cache::recentlyUsedList.push_back(id);
//		}
//		/*
//		如果 cache 爆了，就把 cache 裡最後一個元素替換掉。
//		先到排序用的 deque 去查最後一名的 ID 出來，再移除 cache 裡的元素。
//		*/
//		else
//		{
//			wstring leastRecentlyUsedID = Cache::recentlyUsedList.back();
//			wstring nullString(L"");
//			remove(leastRecentlyUsedID, nullString);
//		}

		/*
		這裡才是真的把東西放進 cache 裡。
		*/
		vector<Profile> currentVector;
		currentVector.push_back(profile);

		m_table.insert(make_pair(id, currentVector));
	}
	else
	{
		bool foundWord = false;
		vector<Profile> profileVector = m_table[id];
		for(size_t i = 0; i < profileVector.size(); ++i)
		{
			Profile existedProfile = profileVector[i];
			size_t currentHitRate = existedProfile.hitRate;
			//@author b6s
			//@todo Refactoring is needed.
			/*
			if(existedProfile.word() == profile.word())
			{
				m_table[id][i].setHitRate(++currentHitRate);
				foundWord = true;
				break;
			}
			else if(currentHitRate > 0)
				m_table[id][i].setHitRate(--currentHitRate);
			*/
		}

		if(!foundWord)
			m_table[id].push_back(profile);
	}

	m_currentCacheSize++;
}

Profile* Cache::fetch(const string& id)
{
	vector<Profile> profiles;
	size_t amount = fetchAll(id, profiles);
	bool hasCustomTerm = false;
	bool doUpdate = false;
	Profile* profile = 0;
	if(amount > 0)
	{
		///*
		size_t maxHitRate = 0;
		for(size_t i = 0; i < profiles.size(); ++i)
		{
			if(profiles[i].isCustom) {
				if(!hasCustomTerm) {
					hasCustomTerm = true;
					doUpdate = true;
				} else if (profiles[i].hitRate >= maxHitRate) {
					doUpdate = true;
				}
			}

			if(!hasCustomTerm && profiles[i].hitRate >= maxHitRate)
				doUpdate = true;

			if(doUpdate) {
				maxHitRate = profiles[i].hitRate;
				profile = &profile[i];
				doUpdate = false;
			}
		}

		//*/
		// 改回「後進先出」模式
		//return profileVector.back();
	}

	return profile;
}

size_t Cache::fetchAll(const string& id, vector<Profile>& profiles)
{
	size_t amount = count(id);
	if (amount > 0)
		profiles = m_table[id];

	return amount;
}

size_t Cache::count(const string& id)
{
	return m_table.count(id);
}

bool Cache::remove(const string& id, const string& word)
{
/*
移除 cache 裡的東西要完成以下三個步驟：
移除在排序用 deque 裡對應的 ID；
移除 cache 裡的元素；
將目前 cache 使用掉的空間減 1。
*/
	deque<string>::iterator current;
	current =
		find(m_recentlyUsedList.begin(), m_recentlyUsedList.end(), id);
	m_recentlyUsedList.erase(current);

	size_t counter = m_table.count(id);

	bool doEraseFlag = false;
	if(counter > 0)
	{
		for(size_t i = 0; i < m_table[id].size(); ++i)
		{
			//@author b6s
			//@todo Refactoring is needed.
			/*
			if(m_table[id][i].word() == word)
			{
				m_table[id].erase(m_table[id].begin() + i);
				m_currentCacheSize--;
				doEraseFlag = true;
				break;
			}
			*/
		}

		if(m_table[id].size() == 0)
			m_table.erase(m_table.find(id));
	}

	return doEraseFlag;
}
