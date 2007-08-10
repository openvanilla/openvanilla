#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "Cache.h"
#include "ProfileFetcher.h"
#include "Token.h"

#include <map>

using namespace std;

class ProfileManager {
	public:
		static ProfileManager* getInstance();
		void releaseInstance();
  
		Profile* query(const string& id);
		Profile* fetch(const string& id, size_t position);
		void writeBack();
		
		void clearSession();
		
		ProfileManager();
		~ProfileManager();

	private:
		static ProfileManager* m_instance;

		Cache* m_cache;
		ProfileFetcher m_server;
		/*
		以上兩個 new class object 的動作應該要套 singleton pattern 才好，
		暫時先不去改它們。
		*/
		
		multimap<string, size_t> m_session;
};

#endif