#include "UnitTest++.h"
#include "Profile.h"
#include "Token.h"

using namespace std;

struct TestProfileFixture
{
	TestProfileFixture()
	{
		anId = make_pair("keystroke string", "word string");
	}

	pair<string, string> anId;
};

TEST_FIXTURE(TestProfileFixture, IdTest)
{	
	Profile profile(anId);
	CHECK_EQUAL("keystroke string", profile.id().first.c_str());
	CHECK_EQUAL("word string", profile.id().second.c_str());
}

TEST_FIXTURE(TestProfileFixture, HitRateTest)
{
	Profile profile(anId, tokens);
	CHECK_EQUAL(0, profile.hitRate);
	++profile.hitRate;
	CHECK_EQUAL(1, profile.hitRate);
	--profile.hitRate;
	CHECK_EQUAL(0, profile.hitRate);
}

TEST_FIXTURE(TestProfileFixture, IsCustomTest)
{
	Profile profile(anId, tokens);
	CHECK(!profile.isCustom);
	profile.isCustom = true;
	CHECK(profile.isCustom);
}
