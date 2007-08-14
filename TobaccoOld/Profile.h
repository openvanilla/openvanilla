#ifndef PROFILE_H
#define PROFILE_H

#include <string>

using namespace std;

class Profile 
{
public:
	Profile(const pair<const string, const string>& theId);
	Profile(const Profile& theProfile);
	Profile& operator=(const Profile& rhsProfile);

 	const pair<const string, const string>& id() const;

	size_t hitRate;
	bool isCustom;

private:
	const pair<const string, const string> m_id;
};

#endif
