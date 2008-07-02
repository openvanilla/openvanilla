#ifndef MODELIMM_H
#define MODELIMM_H

#include <windows.h>
#ifdef WINCE
#include <imm.h>
#else
#include <immdev.h>
#endif
#include "ExtraStructs.h"

class ModelImm
{
public:
	static ModelImm* open(HIMC h);
	static void close(void);

	LPINPUTCONTEXT getIMC(void);
	LPCOMPOSITIONSTRING getCompStr(void);
	LPMYPRIVATE getMyPrivate(void);

	bool setIMC(LPINPUTCONTEXT inLpIMC);
	bool setCompStr(LPCOMPOSITIONSTRING inLpCompStr);
	bool setMyPrivate(LPMYPRIVATE inLpMyPrivate);

protected:
	ModelImm(HIMC h);
	~ModelImm(void);

private:
	static ModelImm* m_modelImm;

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
