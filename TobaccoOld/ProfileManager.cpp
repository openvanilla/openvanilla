#include "ProfileManager.h"

#include "DictionarySingleton.h"

ProfileManager* ProfileManager::_instance = 0;
Profile* ProfileManager::profileWaitingForHit = 0;

ProfileManager::ProfileManager()
{
	cache = Cache::getInstance();
//	LoadCacheFile();  // comment out by Aileen to synchronize with utility program
}

ProfileManager::~ProfileManager()
{
//	SaveCacheFile();  // // comment out by Aileen to synchronize with utility program
	cache->lostInstance();
}

ProfileManager* ProfileManager::getInstance()
{
	if(ProfileManager::_instance == 0)
	{
		_instance = new ProfileManager();
	}

	return _instance;
}

void ProfileManager::lostInstance()
{
	if(ProfileManager::_instance != 0)
		ProfileManager::~ProfileManager();
}

void ProfileManager::clearSession()
{
	ProfileManager::session.clear();
}

Profile* ProfileManager::query(std::wstring id)
{
	Profile* profile = 0;
	profile = cache->fetch(id);		// 到 cache 裡看看有沒有符合的物件。
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

Profile* ProfileManager::fetch(std::wstring id, int currentPos)
{
	Profile* profile = query(id);

	if(profile != 0) {
		wstring currentWord = profile->GetWord();
		int count = session.count(currentWord);
		bool doRank = true;
		if(count > 0) {
			multimap<wstring, int>::iterator iter = session.find(currentWord);
			for(int i = 0; i < count; ++i, ++iter)
			{
				if(iter->second == currentPos) {
					doRank = false;
					break;
				}
			}
		}

		if(doRank) {
			session.insert(make_pair(currentWord, currentPos));
			cache->rank(profile);
		}
	}

	return profile;
}

void ProfileManager::LoadCacheFile()
{
	LoadCacheFile(false);
}

void ProfileManager::LoadCacheFile(bool doSync)
{
	DictionarySingleton* dictionary = DictionarySingleton::getInstance();
	wstring cacheFilePath =dictionary->productInstalledPath + L"\\cache.txt";
	cache->LoadFromFile(cacheFilePath, doSync);
}

void ProfileManager::SaveCacheFile()
{
	DictionarySingleton* dictionary = DictionarySingleton::getInstance();
	wstring cacheFilePath =dictionary->productInstalledPath + L"\\cache.txt";
	cache->SaveToFile(cacheFilePath);
}