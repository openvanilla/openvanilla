#include <stdio.h>
#include "OVIMEUI.h"

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
