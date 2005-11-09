#include "Token.h"

using namespace std;

Token::Token()
{	
	Token::isBoundary = false;
	Token::isFixed = false;
	Token::withPrefix = false;
	Token::withSuffix = false;
}

Token::~Token()
{
    Token::characterStringVector.clear();
    Token::word.erase();
}