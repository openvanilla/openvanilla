#include "UnitTest++.h"
#include "Profile.h"
#include "Token.h"

using namespace std;

struct TestProfileFixture
{
	TestProfileFixture()
	{
		aToken.characterStringVector.push_back("syllable string 1");
		aToken.characterStringVector.push_back("syllable string 2");
		aToken.keystrokes = "keystroke string";
		aToken.word = "word string";
		tokens.push_back(aToken);

		anId = make_pair(aToken.keystrokes, aToken.word);
	}

	pair<string, string> anId;
	Token aToken;
	vector<Token> tokens;
};

TEST_FIXTURE(TestProfileFixture, IdTest)
{	
	Profile profile(anId, tokens);
	CHECK_EQUAL("keystroke string", profile.id().first.c_str());
	CHECK_EQUAL("word string", profile.id().second.c_str());
}

TEST_FIXTURE(TestProfileFixture, TokensTest)
{
	Profile profile(anId, tokens);
	CHECK_EQUAL(
		"syllable string 1",
		tokens[0].characterStringVector[0].c_str());
	CHECK_EQUAL(
		"syllable string 2",
		tokens[0].characterStringVector[1].c_str());
	CHECK_EQUAL("keystroke string", tokens[0].keystrokes.c_str());
	CHECK_EQUAL("word string", tokens[0].word.c_str());

	CHECK(!tokens[0].isBoundary);
	CHECK(!tokens[0].isFixed);
	CHECK(!tokens[0].withPrefix);
	CHECK(!tokens[0].withSuffix);
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
