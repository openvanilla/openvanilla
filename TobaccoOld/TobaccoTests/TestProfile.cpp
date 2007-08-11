#include "UnitTest++.h"
#include "Profile.h"
#include "Token.h"
#include <vector>

using namespace std;

struct TestProfileFixture
{
	TestProfileFixture()
	{
		tokens.push_back(token);
	}

	Token token;
	vector<Token> tokens;
};

TEST_FIXTURE(TestProfileFixture, IdTest)
{
	Profile profile("foo", tokens);
	CHECK("foo", profile.id.c_str());
}

TEST_FIXTURE(TestProfileFixture, TokensTest)
{
}

TEST_FIXTURE(TestProfileFixture, PositionTest)
{
}

TEST_FIXTURE(TestProfileFixture, HitRateTest)
{
}

TEST_FIXTURE(TestProfileFixture, IsCustomTest)
{
}
