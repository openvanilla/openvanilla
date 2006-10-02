#include "OVIME.h"
#include <cstdio>

void MakeCompStr(LPMYPRIVATE lpMyPrivate, LPCOMPOSITIONSTRING lpCompStr)
{
	wcscpy(GETLPCOMPSTR(lpCompStr), L"");
	swprintf(GETLPCOMPSTR(lpCompStr), lpMyPrivate->PreEditStr);
	lpCompStr->dwCompStrLen = (DWORD)wcslen(GETLPCOMPSTR(lpCompStr));
}

void InitCompStr(LPCOMPOSITIONSTRING lpCompStr)
{
	lpCompStr->dwSize = sizeof(MYCOMPSTR);
	lpCompStr->dwCompStrOffset =
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szCompStr - (DWORD)lpCompStr;
	lpCompStr->dwCompStrLen = 0;
	*GETLPCOMPSTR(lpCompStr) = L'\0';

	lpCompStr->dwResultStrOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szResultStr - (DWORD)lpCompStr;
	lpCompStr->dwResultStrLen = 0;
	*GETLPRESULTSTR(lpCompStr) = L'\0';
	
	lpCompStr->dwCursorPos = 0;
}

void InitCandInfo(LPCANDIDATEINFO lpCandInfo)
{
	LPCANDIDATELIST lpCandList;
	
	int i;

	lpCandInfo->dwSize = sizeof(MYCAND);
	lpCandInfo->dwCount = 1;
	lpCandInfo->dwOffset[0] = 
			(DWORD)(&(((LPMYCAND)lpCandInfo)->cl)) - (DWORD)lpCandInfo;

	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	for(i = 0; i < MAXCANDSTRNUM; i++)
	{
		lpCandList->dwOffset[i] = 
                   (DWORD)((LPMYCAND)lpCandInfo)->szCandStr[i] - (DWORD)lpCandList;
	}
	lpCandList->dwSize = sizeof(MYCAND) - sizeof(CANDIDATEINFO); 
	lpCandList->dwCount = 0L;
	lpCandList->dwSelection = 0L;
	lpCandList->dwPageStart = 0L;
	lpCandList->dwPageSize = 0L;
}

void ClearCandidate(LPCANDIDATEINFO lpCandInfo)
{
    ((LPMYCAND)lpCandInfo)->cl.dwCount =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwSelection =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageStart =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageSize =0L;
}

void UpdateCandidate(LPINPUTCONTEXT lpIMC, const wchar_t* candis)
{
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo))
	{
		wchar_t* lpStr;
		lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo
			+ lpCandInfo->dwOffset[0]);
		int rlen = (int)wcslen(candis);
		int n = 0;
		int ln = 0;
		for( int i = 0; i < rlen; i++ )
		{
			if(candis[i] == L' ' || i == rlen - 1)
			{
				murmur("UPDATE\n", NULL);
				int dst = i - 1;
				if( i == rlen - 1)
					dst = i + 1;
				lpStr = GETLPCANDSTR(lpCandList, n);
				memset(lpStr, 0, 100*sizeof(wchar_t));
				wcsncpy(lpStr, candis+ln, dst - ln + 1);
				ln = i + 1;
				n++;
			}
		}
		lpCandList->dwCount = n - 2;
		lpCandList->dwPageSize = 11;
		lpCandList->dwPageStart = 0;

		ImmUnlockIMCC(lpIMC->hCandInfo);
	}
}
