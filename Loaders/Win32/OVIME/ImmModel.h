#ifndef IMMMODEL_H
#define IMMMODEL_H

#include <windows.h>
#include <immdev.h>
#include "ExtraStructs.h"

class ImmModel
{
public:
	static ImmModel* open(HIMC h);
	static void close(void);

	LPINPUTCONTEXT getIMC(void);
	LPCOMPOSITIONSTRING getCompStr(void);
	LPMYPRIVATE getMyPrivate(void);

	bool setIMC(LPINPUTCONTEXT inLpIMC);
	bool setCompStr(LPCOMPOSITIONSTRING inLpCompStr);
	bool setMyPrivate(LPMYPRIVATE inLpMyPrivate);

protected:
	ImmModel(HIMC h);
	~ImmModel(void);

private:
	static ImmModel* m_self;

	HIMC m_hIMC;

	LPINPUTCONTEXT m_lpIMC;
	LPCOMPOSITIONSTRING m_lpCompStr;
	LPMYPRIVATE m_lpMyPrivate;

	LPINPUTCONTEXT lockIMC(void);
	LPCOMPOSITIONSTRING lockCompStr(void);
	LPMYPRIVATE lockMyPrivate(void);

	void unlockIMC(void);
	void unlockCompStr(void);
	void unlockMyPrivate(void);
};

#endif
