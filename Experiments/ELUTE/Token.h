#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

using namespace std;

class Token
{
public:
	Token();
	~Token();

	bool withPrefix;
	bool withSuffix;
	bool isFixed;
	bool isBoundary;

	int inputMethod;

	vector<string> characterStringVector;
	string word;
};

#endif