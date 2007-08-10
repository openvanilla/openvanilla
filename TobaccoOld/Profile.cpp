#include "Profile.h"

using namespace std;

Profile::Profile(const string& id, size_t position, const vector<Token>& tokens)
{
	m_id = id;
	m_tokens = tokens;
	/*
	for(vector<Token>::const_iterator iter = tokens.begin();
		iter != tokens.end(); ++iter)
	{
		m_tokens.push_back(*iter);	
	}
	*/

	m_currentPosition = position;
	m_hitRate = 1;
	m_isCustom = false;
}

const string& Profile::id() {return m_id;}
/*
Profile 裡面現在只有個叫 id 的 string，
事實上可能還要有 weight 才恰當。
*/

const string& Profile::word()
{
	m_word.clear();
	for(size_t i = 0; i < m_tokens.size(); ++i)
		m_word.append(m_tokens[i].word);

	return m_word;
}

size_t Profile::currentPosition() { return m_currentPosition; }

Token* Profile::tokenAt(size_t position)
{
	if(position >= m_tokens.size()) return 0;

	return &m_tokens.at(position);
}

size_t Profile::tokenSize()
{
	return m_tokens.size();
}

size_t Profile::hitRate()
{
	return m_hitRate;
}

void Profile::setHitRate(size_t hits)
{	
	m_hitRate = hits;
}

bool Profile::isCustom()
{
	return m_isCustom;
}

void Profile::setCustomFlag(bool isCustom)
{
	m_isCustom = isCustom;
}
