#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "Cache.h"
#include "ProfileFetcher.h"
#include "token.h"

#include <map>

using namespace std;

class ProfileManager {
	public:
		static ProfileManager* getInstance();
		void lostInstance();
  
		Profile* query(std::wstring id);
		Profile* fetch(std::wstring id, int pos);
		void LoadCacheFile();
		void LoadCacheFile(bool doSync);
		void SaveCacheFile();
		
		void clearSession();

		ProfileManager();
		~ProfileManager();
	private:
		static ProfileManager* _instance;
		static Profile* profileWaitingForHit;

		Cache* cache;
		ProfileFetcher server;
		/*
		以上兩個 new class object 的動作應該要套 singleton pattern 才好，
		暫時先不去改它們。
		*/

		multimap<wstring, int> session;
};

#endif