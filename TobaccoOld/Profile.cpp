#include "Profile.h"

using namespace std;

Profile::Profile(
	const pair<const string, const string>& theId,
	const vector<Token>& theTokens) :
		m_id(theId), m_tokens(theTokens),
		hitRate(0), isCustom(false)
{}

Profile::Profile(const Profile& theProfile) :
	m_id(theProfile.id()), m_tokens(theProfile.tokens()),
	hitRate(theProfile.hitRate), isCustom(theProfile.isCustom)
{}

//@{
//@note for Cache::add(Profile&) { vector<Profile>... }
Profile& Profile::operator=(const Profile& rhsProfile)
{
	if(this != &rhsProfile) *this = rhsProfile;

	return *this;
}
//@}

const pair<const string, const string>& Profile::id() const { return m_id; }

const vector<Token>& Profile::tokens() const { return m_tokens; }