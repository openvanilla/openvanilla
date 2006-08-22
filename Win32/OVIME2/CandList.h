#pragma once

#include <windows.h>
#include <tchar.h>
#include <immdev.h>

class CandList
{
public:
	CandList(void);
	~CandList(void);
	void setTotalCount(int c){	cl.dwCount = c;	}
	int getTotalCount(){ return cl.dwCount; }
	void setPageSize(int s){ cl.dwPageSize = s; }
	int getPageSize(){ return cl.dwPageSize; }
	void setPageStart(int i){ cl.dwPageStart = i; }
	void setSelection( int i ) { cl.dwSelection = i; }
	int getSelection() { return cl.dwSelection; }
	int getPageStart(){ return cl.dwPageStart; }
	void setCand(int i, LPCWSTR cand);
	LPCWSTR getCand(int i){ return candStr[i]; }
protected:
    CANDIDATEINFO  ci;
    CANDIDATELIST  cl;
    DWORD offset[1024-1];
	wchar_t candStr[1024][8];	// unicode string
};
