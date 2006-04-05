#include ".\candlist.h"

CandList::CandList(void)
{
	ci.dwSize = sizeof(CandList);
	ci.dwCount = 1;
	ci.dwOffset[0] = (size_t(&cl) - size_t(&ci));
	ci.dwPrivateSize = 0;

	cl.dwSize = sizeof(cl)+sizeof(offset)+sizeof(candStr);
	cl.dwStyle = IME_CAND_READ;
	cl.dwCount = 0;
	cl.dwSelection = 0;
	cl.dwPageStart = 0;

	for( int i = 0; i <= (sizeof(offset)/sizeof(DWORD)); ++i )
	{
		cl.dwOffset[i] = ((DWORD)candStr[i] - (DWORD)&cl);
		*candStr[i] = 0;
	}

}

CandList::~CandList(void)
{
}

void CandList::setCand( int i, LPCWSTR cand )
{
	wcscpy( candStr[i], cand );
}

