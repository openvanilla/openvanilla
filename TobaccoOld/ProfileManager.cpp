#include "ProfileManager.h"

//@author b6s
//@todo needs bridging to the storage.
//#include "DictionarySingleton.h"

ProfileManager* ProfileManager::m_instance = 0;

ProfileManager* ProfileManager::getInstance()
{
	if(!m_instance) m_instance = new ProfileManager();

	return m_instance;
}

void ProfileManager::releaseInstance()
{
	if(m_instance) delete m_instance;
}

ProfileManager::ProfileManager()
{
	m_cache = Cache::getInstance();
}

ProfileManager::~ProfileManager()
{
	m_cache->releaseInstance();
}

vector<Profile>* ProfileManager::fetch(const string& theKey)
{
	vector<Profile>* profiles = m_cache->fetch(theKey);
	if(profiles) return profiles;

	//@todo collects required data from Tobacco's Tokens here.
	//m_creator.create(theKey, theWord, theTokens);
	profiles = m_cache->fetch(theKey);
	if(profiles) return profiles;

	return 0;
}

void ProfileManager::writeBack()
{
	//@todo writes cache back to SQLite db here.
}
