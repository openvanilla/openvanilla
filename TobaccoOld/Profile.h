#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include "token.h"

using namespace std;

class Profile 
{
	friend class ProfileFetcher;
	friend class ProfileManager;

public:
 		wstring GetID();
		wstring GetWord();
		int	GetCurTokenPos();
		const Token *GetTokenAt(int iPos) ;
		int	GetTokenSize();
		int GetHitRate();
		void SetHitRate(int);
		bool GetCustomizeFlag();
		void SetCustomizeFlag(bool setFlag);
		Profile(const wstring strID, int iPos, vector<Token> &tokenvec);
		
private:
		wstring m_ID;
		vector<Token> m_Tokens;
		int	m_iCurTokenPos;
		int m_hitsRate;
		bool _isCustomized;
};

#endif