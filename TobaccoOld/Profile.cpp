#include "Profile.h"

using namespace std;

Profile::Profile(const wstring strID, int iPos, vector<Token> &tokenvec)
{
	m_ID = strID;
	for( vector<Token>::const_iterator i= tokenvec.begin(); i!=tokenvec.end();i++)
	{
		m_Tokens.push_back(*i);

	}
	m_iCurTokenPos = iPos;
	m_hitsRate = 1;
	_isCustomized = false;
}


wstring Profile::GetID() {return m_ID;}
/*
Profile 裡面現在只有個叫 id 的 string，
事實上至少還要加上一個 weighting 才恰當。
*/

int Profile::GetCurTokenPos() { return m_iCurTokenPos; }

const Token *Profile::GetTokenAt(int iPos) 
{
	if( iPos >= m_Tokens.size() || iPos < 0 )
		return NULL;

	return &m_Tokens.at(iPos);
}

int Profile::GetTokenSize()
{
	return m_Tokens.size();
}

void Profile::SetHitRate(int hits)
{	
	m_hitsRate = hits;
}

int Profile::GetHitRate()
{
	return m_hitsRate;
}

wstring Profile::GetWord()
{
	wstring word(L"");
	int size = m_Tokens.size();
	for(int i = 0; i < size; ++i)
		word.append(m_Tokens[i].word);

	return word;
}

void Profile::SetCustomizeFlag(bool setFlag)
{
	_isCustomized = setFlag;
}

bool Profile::GetCustomizeFlag()
{
	return _isCustomized;
}