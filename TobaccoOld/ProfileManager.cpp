#include "ProfileManager.h"

#include "DictionarySingleton.h"

ProfileManager* ProfileManager::m_instance = 0;

ProfileManager::ProfileManager()
{
	m_cache = Cache::getInstance();
}

ProfileManager::~ProfileManager()
{
	m_cache->releaseInstance();
}

ProfileManager* ProfileManager::getInstance()
{
	if(ProfileManager::m_instance == 0)
	{
		m_instance = new ProfileManager();
	}

	return m_instance;
}

void ProfileManager::releaseInstance()
{
	if(ProfileManager::m_instance != 0)
		ProfileManager::~ProfileManager();
}

Profile* ProfileManager::query(const string& id)
{
	Profile* profile = 0;
	profile = m_cache->fetch(id);		// 到 cache 裡看看有沒有符合的物件。
	/*
	if (profile == 0)					// 如果沒有，就叫後端去把需要的物件找出來。
	{									// 對輸入法來說，後端動作也可以換成是切出學習到的字串。
		//profile = server.fetch(id);		// 對一般 cache 系統來說，後端不保證有對應結果，
		//if (profile != 0)				// 但對輸入法裡的學習來說一定有，所以這裡的 if 總是 true。
		//	cache.add(profile);			// 把前面 server->fetch(id) 裡處理好的物件放進 cache。
		// simulate add Profile for test only ; if not test,mark it
		//else
		//{
		//	vector<Token> tokenVec;
		//  Token token[2];//ㄅㄚ-ㄅㄧˇ	芭比	3
		//  token[0].characterStringVector.push_back ( L"ㄅㄚ");
		//  token[0].characterStringVector.push_back ( L"ㄅㄧˇ");
		//  token[1].word = L"芭";
		//  token[1].word = L"比"; 
		//	tokenVec.push_back(token[0]);
		//	tokenVec.push_back(token[1]);
		//	profile = new Profile(id,0,tokenVec);
		//	cache.add(profile);
		}//end of test
	}
	*/

	return profile;
}

Profile* ProfileManager::fetch(const string& id, size_t position)
{
	Profile* profile = query(id);

	if(profile != 0) {
		//@author b6s
		//@todo Refactoring is needed.
		//string word = profile->word();
		string word("");
		size_t count = m_session.count(word);
		bool doRank = true;
		if(count > 0) {
			multimap<string, size_t>::iterator iter = m_session.find(word);
			for(size_t i = 0; i < count; ++i, ++iter)
			{
				if(iter->second == position) {
					doRank = false;
					break;
				}
			}
		}

		if(doRank) {
			m_session.insert(make_pair(word, position));
			m_cache->rank(id, word);
		}
	}

	return profile;
}

void ProfileManager::writeBack()
{
	//@todo writes cache back to SQLite db here.
}

void ProfileManager::clearSession()
{
	ProfileManager::m_session.clear();
}
