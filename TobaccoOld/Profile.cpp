#include "Profile.h"

using namespace std;

Profile::Profile(const pair<const string, const string>& theId) :
	m_id(theId), hitRate(0), isCustom(false)
{}

Profile::Profile(const Profile& theProfile) :
	m_id(theProfile.id()), hitRate(theProfile.hitRate),
	isCustom(theProfile.isCustom)
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

