#include "Profile.h"

using namespace std;

ProfileId::ProfileId(const string& theKeystrokes, const string& thePattern) :
	keystrokes(theKeystrokes), pattern(thePattern)
{}

ProfileId::ProfileId(const ProfileId& theId) :
	keystrokes(theId.keystrokes), pattern(theId.pattern)
{}

bool ProfileId::operator==(const ProfileId& rhsProfileId)
{
	return
		keystrokes == rhsProfileId.keystrokes &&
			pattern == rhsProfileId.pattern;
}

Profile::Profile(const ProfileId& theId) :
	id(theId), hitRate(0), isCustom(false)
{}

Profile::Profile(const Profile& theProfile) :
	id(theProfile.id), hitRate(theProfile.hitRate),
	isCustom(theProfile.isCustom)
{}
