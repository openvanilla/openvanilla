#include "ImmModel.h"   

ImmModel* ImmModel::m_self = NULL;
HIMC ImmModel::m_hIMC = NULL;
LPINPUTCONTEXT ImmModel::m_lpIMC = NULL;
LPCOMPOSITIONSTRING ImmModel::m_lpCompStr = NULL;
LPMYPRIVATE ImmModel::m_lpMyPrivate = NULL;
LPCANDIDATEINFO ImmModel::m_lpCandInfo = NULL;

ImmModel::ImmModel(HIMC h)
{
	m_hIMC = h;

	m_lpIMC = lockIMC();

	m_lpCompStr = lockCompStr();

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

LPCANDIDATEINFO ImmModel::getCandInfo(void)
{
	return lockCandInfo();
}


LPMYPRIVATE ImmModel::getMyPrivate(void)
{
	return lockMyPrivate();
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
LPCANDIDATEINFO ImmModel::lockCandInfo(void)
{
	if(!m_lpCandInfo && getIMC())
		m_lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(m_lpIMC->hCandInfo);
	return m_lpCandInfo;
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

void ImmModel::unlockCandInfo(void)
{
	if(m_lpCandInfo && m_lpIMC)
	{
		ImmUnlockIMCC(m_lpIMC->hCandInfo);
		m_lpCandInfo=NULL;
	}
}
