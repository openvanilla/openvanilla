// OVIME2Client.h: interface for the OVIME2Client class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVIME2Client_H__E54E6740_17FB_4E1D_B912_751FF053FFD1__INCLUDED_)
#define AFX_OVIME2Client_H__E54E6740_17FB_4E1D_B912_751FF053FFD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "OVIME2Server.h"

#include <vector>
#include <string>

using std::vector;
using std::wstring;

class OVIME2Client  
{
public:
	OVIME2Client();
	~OVIME2Client();

public:
	// Return the i-th selection wchar_t, i >= 0.
	char* Selection(int i);
	void SetFullShape(bool full);
	bool GetFullShape(void);

	void ConnectServer(void);
    unsigned int  EchoFromServer();
    bool CheckServer();

	bool keyEvent(UINT vKey, LPARAM lParam, const BYTE* keyState);
	int getData(wstring& comp_str, wstring& result_str, wstring& notify_str,
				bool& has_cand, int& cursor_pos, int& mark_from, int& mark_to );
	int getModList(vector<wstring>& list);

protected:
	HANDLE sharedMem;
	LPARAM clientID;
	HWND serverWnd;
    TCHAR filemapName[MAX_PATH];
};

#endif // !defined(AFX_OVIME2Client_H__E54E6740_17FB_4E1D_B912_751FF053FFD1__INCLUDED_)

