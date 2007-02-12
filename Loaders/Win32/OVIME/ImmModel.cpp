#include "ImmModel.h"

ImmModel* ImmModel::m_self = NULL;

ImmModel::ImmModel(HIMC h)
{
	m_hIMC = h;

	m_lpIMC = NULL;
	m_lpIMC = lockIMC();

	m_lpCompStr = NULL;
	m_lpCompStr = lockCompStr();

	m_lpMyPrivate = NULL;
	m_lpMyPrivate = lockMyPrivate();
}

ImmModel::~ImmModel(void)
{
	unlockIMC();

	m_self = NULL;
}

ImmModel* ImmModel::open(HIMC h)
{
	if(m_self == NULL)
		m_self = new ImmModel(h);

	return m_self;
}

void ImmModel::close(void)
{
	if(m_self) delete m_self;
}

LPINPUTCONTEXT ImmModel::getIMC(void)
{
	return lockIMC();
}

LPCOMPOSITIONSTRING ImmModel::getCompStr(void)
{
	return lockCompStr();
}

LPMYPRIVATE ImmModel::getMyPrivate(void)
{
	return lockMyPrivate();
}

bool ImmModel::setIMC(LPINPUTCONTEXT inLpIMC)
{
	bool flag = false;
	if(getIMC()) {
		m_lpIMC = inLpIMC;
		if(m_lpIMC) flag = true;
	}
	return flag;
}

bool ImmModel::setCompStr(LPCOMPOSITIONSTRING inLpCompStr)
{
	bool flag = false;
	if(getCompStr()) {
		m_lpCompStr = inLpCompStr;
		if(m_lpCompStr) flag = true;
	}
	return flag;
}

bool ImmModel::setMyPrivate(LPMYPRIVATE inLpMyPrivate)
{
	bool flag = false;
	if(getMyPrivate()) {
		m_lpMyPrivate = inLpMyPrivate;
		if(m_lpMyPrivate) flag = true;
	}
	return flag;
}

LPINPUTCONTEXT ImmModel::lockIMC(void)
{
	if(!m_lpIMC && m_hIMC)
		m_lpIMC = ImmLockIMC(m_hIMC);
	return m_lpIMC;
}

LPCOMPOSITIONSTRING ImmModel::lockCompStr(void)
{
	if(!m_lpCompStr && getIMC())		
		m_lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(m_lpIMC->hCompStr);
	return m_lpCompStr;
}

LPMYPRIVATE ImmModel::lockMyPrivate(void)
{
	if(!m_lpMyPrivate && getIMC())
		m_lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(m_lpIMC->hPrivate);
	return m_lpMyPrivate;
}

void ImmModel::unlockIMC(void)
{
	unlockCompStr();
	unlockMyPrivate();

	if(m_hIMC) {
		ImmUnlockIMC(m_hIMC);
		m_lpIMC = NULL;
		m_hIMC = NULL;
	}
}

void ImmModel::unlockCompStr(void)
{
	if(m_lpCompStr && m_lpIMC)
	{
		ImmUnlockIMCC(m_lpIMC->hCompStr);
		m_lpCompStr=NULL;
	}
}

void ImmModel::unlockMyPrivate(void)
{
	if(m_lpMyPrivate && m_lpIMC)
	{
		ImmUnlockIMCC(m_lpIMC->hPrivate);
		m_lpMyPrivate=NULL;
	}
}
