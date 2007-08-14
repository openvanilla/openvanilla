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

TEST_FIXTURE(TestProfileFixture, CopyConstructorTest)
{
	Profile* src = new Profile(anId);
	Profile* tgt = new Profile(*src);
	delete src;
	src = 0;
	if(tgt)
		CHECK_EQUAL("keystroke string", tgt->id().first.c_str());
	else
		CHECK(false);
}

TEST_FIXTURE(TestProfileFixture, CopyAssignmentTest)
{
	Profile* src = new Profile(anId);
	Profile tgt1 = *src;	
	delete src;
	src = 0;
	CHECK_EQUAL("keystroke string", tgt1.id().first.c_str());

	Profile tgt2 = tgt1;
	CHECK_EQUAL("keystroke string", tgt2.id().first.c_str());
}

TEST_FIXTURE(TestProfileFixture, IdTest)
{	
	Profile profile(anId);
	CHECK_EQUAL("keystroke string", profile.id().first.c_str());
	CHECK_EQUAL("word string", profile.id().second.c_str());
}

TEST_FIXTURE(TestProfileFixture, HitRateTest)
{
	Profile profile(anId);
	CHECK_EQUAL(0, profile.hitRate);
	++profile.hitRate;
	CHECK_EQUAL(1, profile.hitRate);
	--profile.hitRate;
	CHECK_EQUAL(0, profile.hitRate);
}

TEST_FIXTURE(TestProfileFixture, IsCustomTest)
{
	Profile profile(anId);
	CHECK(!profile.isCustom);
	profile.isCustom = true;
	CHECK(profile.isCustom);
}
