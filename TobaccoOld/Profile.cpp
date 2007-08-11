#include "Profile.h"

using namespace std;

Profile::Profile(const string& id, const vector<Token>& tokens) :
				m_id(id), m_tokens(tokens),
				position(-1), hitRate(0), isCustom(false)
{}

const string& Profile::id() { return m_id; }

const vector<Token>& Profile::tokens() { return m_tokens; }
