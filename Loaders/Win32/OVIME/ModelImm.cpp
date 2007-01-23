#include "ModelImm.h"

ModelImm* ModelImm::m_modelImm = NULL;
ModelImm::ModelImm(HIMC h)
{
	m_hIMC = h;
	m_lpIMC = lockIMC();
	m_lpCompStr = lockCompStr();
	m_lpMyPrivate = lockMyPrivate();
}

ModelImm::~ModelImm(void)
{
	unlockMyPrivate();
	unlockCompStr();
	unlockIMC();
}

ModelImm* ModelImm::open(HIMC h)
{
	if(m_modelImm == NULL)
		m_modelImm = new ModelImm(h);

	return m_modelImm;
}

void ModelImm::close(void)
{
	if(m_modelImm) delete m_modelImm;
}

LPINPUTCONTEXT ModelImm::getIMC(void)
{
	return lockIMC();
}

LPCOMPOSITIONSTRING ModelImm::getCompStr(void)
{
	return lockCompStr();
}

LPMYPRIVATE ModelImm::getMyPrivate(void)
{
	return lockMyPrivate();
}

bool ModelImm::setIMC(LPINPUTCONTEXT inLpIMC)
{
	bool flag = false;
	if(getIMC()) {
		m_lpIMC = inLpIMC;
		if(m_lpIMC) flag = true;
	}
	return flag;
}

bool ModelImm::setCompStr(LPCOMPOSITIONSTRING inLpCompStr)
{
	bool flag = false;
	if(getCompStr()) {
		m_lpCompStr = inLpCompStr;
		if(m_lpCompStr) flag = true;
	}
	return flag;
}

bool ModelImm::setMyPrivate(LPMYPRIVATE inLpMyPrivate)
{
	bool flag = false;
	if(getMyPrivate()) {
		m_lpMyPrivate = inLpMyPrivate;
		if(m_lpMyPrivate) flag = true;
	}
	return flag;
}

LPINPUTCONTEXT ModelImm::lockIMC(void)
{
	if(!m_lpIMC && m_hIMC)
		m_lpIMC = ImmLockIMC(m_hIMC);
	return m_lpIMC;
}

LPCOMPOSITIONSTRING ModelImm::lockCompStr(void)
{
	if(!m_lpCompStr && getIMC())		
		m_lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(m_lpIMC->hCompStr);
	return m_lpCompStr;
}

LPMYPRIVATE ModelImm::lockMyPrivate(void)
{
	if(!m_lpMyPrivate && getIMC())
		m_lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(m_lpIMC->hPrivate);
	return m_lpMyPrivate;
}

void ModelImm::unlockIMC(void)
{
	unlockCompStr();
	unlockMyPrivate();

	if(m_hIMC) {
		ImmUnlockIMC(m_hIMC);
		m_lpIMC = NULL;
		m_hIMC = NULL;
	}
}

void ModelImm::unlockCompStr(void)
{
	if(m_lpCompStr && m_lpIMC)
	{
		ImmUnlockIMCC(m_lpIMC->hCompStr);
		m_lpCompStr=NULL;
	}
}

void ModelImm::unlockMyPrivate(void)
{
	if(m_lpMyPrivate && m_lpIMC)
	{
		ImmUnlockIMCC(m_lpIMC->hPrivate);
		m_lpMyPrivate=NULL;
	}
}
