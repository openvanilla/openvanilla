#pragma once

#include <windows.h>
#include <tchar.h>
#include "imm.h"

class CompStr
{
public:
	CompStr(void);
	~CompStr(void);
protected:
	COMPOSITIONSTRING cs;
	wchar_t readStr[256];	// unicode string
	BYTE readAttr[ 256/sizeof(TCHAR) ];
	DWORD readClause[2];

	wchar_t compStr[256];	// unicode string
	BYTE compAttr[ 256/sizeof(TCHAR) ];
	DWORD compClause[257];

	wchar_t resultReadStr[256];	// unicode string
	DWORD resultReadClause[2];

	wchar_t resultStr[256];	// unicode string
	DWORD resultClause[257];

	short IntervalAry[512];

// private data;
	wchar_t showMsg[50];

public:
	wchar_t* getCompStr()	{	return &compStr[0];	}
	wchar_t* getResultStr()	{	return &resultStr[0];	}
	wchar_t* getShowMsg()	{	return &showMsg[0];	}
	DWORD getCursorPos()	{	return cs.dwCursorPos;	}
	void setCompStr(LPCWSTR comp_str);
	void setResultStr(LPCWSTR result_str);
	void setShowMsg(LPCWSTR show_msg);
	void setCursorPos(int pos);
	void setZuin(LPCWSTR zuin);
	void beforeGenerateMsg(void);
	void setInvervalAry( char* intervalStr );
	short* getIntervalAry() {	return IntervalAry; }
};
