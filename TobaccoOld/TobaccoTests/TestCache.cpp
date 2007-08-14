#include "UnitTest++.h"
#include "Cache.h"

struct TestCacheFixture
{
	TestCacheFixture()
	{
		cache = Cache::getInstance();

		aToken.characterStringVector.push_back("syllable string 1");
		aToken.characterStringVector.push_back("syllable string 2");
		aToken.keystrokes = "keystroke string";
		aToken.word = "word string";
		tokens.push_back(aToken);

		anId = make_pair(aToken.keystrokes, aToken.word);
	}

	~TestCacheFixture()
	{
		cache->releaseInstance();
	}

	Cache* cache;

	pair<string, string> anId;
	Token aToken;
	vector<Token> tokens;
};

TEST_FIXTURE(TestCacheFixture, CacheTest)
{
	Profile profile(anId, tokens);
	cache->add(profile);

	vector<Profile>* profiles = cache->fetch("keystroke string");
	if(profiles)
	{
		CHECK(profiles->at(0).id().first == "keystroke string");
		CHECK(profiles->at(0).id().second == "word string");
		CHECK_EQUAL(0, profiles->at(0).hitRate);
	}

	cache->update(make_pair("keystroke string", "word string"));
	profiles = cache->fetch("keystroke string");
	if(profiles)
	{
		CHECK(profiles->at(0).id().first == "keystroke string");
		CHECK(profiles->at(0).id().second == "word string");
		CHECK_EQUAL(1, profiles->at(0).hitRate);
	}

	CHECK(cache->remove(make_pair("keystroke string", "word string")));
}
