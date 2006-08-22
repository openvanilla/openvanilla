#pragma once

#include <windows.h>
#include <immdev.h>

class CompStr;
class CandList;

class IMCLock
{
public:
	IMCLock(HIMC hIMC);
	~IMCLock(void);
protected:
	HIMC himc;
	INPUTCONTEXT* ic;
	CompStr* compStr;
	CandList* candList;
public:
	CompStr* getCompStr(void);
	CandList* getCandList(void);
	INPUTCONTEXT* getIC(){	return ic;	}
	HIMC getHIMC(){	return himc;	}
	bool lock(void);
	void unlock(void);
	bool isChinese(void);
	bool isFullShape(void);
	bool isVerticalComp(void);
};
