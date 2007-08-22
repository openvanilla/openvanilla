#include "UnitTest++.h"
#include "Cache.h"

struct TestCacheFixture
{
	TestCacheFixture()
	{
		cache = Cache::getInstance();
	}

	~TestCacheFixture()
	{
		cache->releaseInstance();
	}

	Cache* cache;
};

//@note use cases of ProfileManager, actually.
TEST_FIXTURE(TestCacheFixture, CacheTest)
{
	ProfileId anId("keystroke string", "word string");

	Profile profile(anId); //< it should be done by ProfileFetcher.
	cache->add(profile);

	vector<Profile>* profiles = cache->fetch("keystroke string");
	if(profiles)
	{
		CHECK(profiles->at(0).id.keystrokes == "keystroke string");
		CHECK(profiles->at(0).id.pattern == "word string");
		CHECK_EQUAL(0, profiles->at(0).hitRate);
	}

	ProfileId anotherId("keystroke string", "word string");
	bool isUpdated = cache->update(anotherId, true);
	CHECK(isUpdated);
	if(isUpdated)
	{
		CHECK(profiles->at(0).isCustom);
		CHECK_EQUAL(1, profiles->at(0).hitRate);

		cache->update(anotherId);
		CHECK_EQUAL(2, profiles->at(0).hitRate);
	}

	CHECK(cache->remove(anotherId));
}
