#include "UnitTest++.h"
#include "Profile.h"
#include "Token.h"

using namespace std;

struct TestProfileFixture
{
	TestProfileFixture()
	{
		anId = "foo";
		theTokens.push_back(aToken);
	}

	string anId;
	Token aToken;
	vector<Token> theTokens;
};

TEST_FIXTURE(TestProfileFixture, IdTest)
{	
	Profile profile(anId, theTokens);
	CHECK_EQUAL("foo", profile.id().c_str());
}

TEST_FIXTURE(TestProfileFixture, TokensTest)
{
}

TEST_FIXTURE(TestProfileFixture, PositionTest)
{
	Profile profile(anId, theTokens);
	CHECK_EQUAL(-1, profile.position);
	profile.position = 1;
	CHECK_EQUAL(1, profile.position);
}

TEST_FIXTURE(TestProfileFixture, HitRateTest)
{
	Profile profile(anId, theTokens);
	CHECK_EQUAL(0, profile.hitRate);
	++profile.hitRate;
	CHECK_EQUAL(1, profile.hitRate);
	--profile.hitRate;
	CHECK_EQUAL(0, profile.hitRate);
}

TEST_FIXTURE(TestProfileFixture, IsCustomTest)
{
	Profile profile(anId, theTokens);
	CHECK_EQUAL(false, profile.isCustom);
	profile.isCustom = true;
	CHECK(profile.isCustom);
}
