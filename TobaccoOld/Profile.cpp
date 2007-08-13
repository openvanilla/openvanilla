#include "Profile.h"

using namespace std;

Profile::Profile(
	const pair<const string, const string>& theId,
	const vector<Token>& theTokens) :
		m_id(theId), m_tokens(theTokens),
		position(-1), hitRate(0), isCustom(false)
{}

Profile& Profile::operator=(const Profile& rhs) { return *this; }

const pair<const string, const string>& Profile::id() const { return m_id; }

const vector<Token>& Profile::tokens() const { return m_tokens; }