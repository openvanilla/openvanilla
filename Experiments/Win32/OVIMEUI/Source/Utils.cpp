#include <stdio.h>
#include "OVIMEUI.h"

LPUIEXTRA lpUIExtra;

void SetLPUIExtra(LPUIEXTRA lp)
{
	lpUIExtra = lp;
}

LPTSTR UTF16toWCHAR(char *str)
{
	LPTSTR r = (LPTSTR)malloc(1024 * sizeof(TCHAR));
	char tmp[5];
	int out;

	int len = strlen(str);
	for(int i = 0; i*4 < len; i++)
	{
		if(i*4 < len)
			strncpy(tmp, str + i*4, 4);
		sscanf(tmp, "%x", &out);
		r[i] = out;
	}
	r[len/4] = NULL;
	return r;
}

BOOL MyIsIMEMessage(UINT msg)
{
	switch(msg)
	{
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_ENDCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_IME_NOTIFY:
		case WM_IME_SETCONTEXT:
		case WM_IME_CONTROL:
		case WM_IME_COMPOSITIONFULL:
		case WM_IME_SELECT:
		case WM_IME_CHAR:
			return TRUE;
	}
	return FALSE;
}
