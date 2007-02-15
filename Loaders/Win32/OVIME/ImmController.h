#ifndef IMMCONTROLLER_H
#define IMMCONTROLLER_H

#include <windows.h>
#include <immdev.h>
#include "ExtraStructs.h"

#include "OVIME.h"

#include "AVKeyCode.h"

#include "ImmModel.h"

struct KeyInfo
{
	UINT repeatCount:16;
	UINT scanCode:8;
	UINT isExtended:1;
	UINT reserved:4;
	UINT contextCode:1;
	UINT prevKeyState:1;
	UINT isKeyUp:1;	// transition state
};

class ImmController
{
public:
	static ImmController* open();
	static void close(void);

	inline KeyInfo getKeyInfo(LPARAM lparam) { return *(KeyInfo*)&lparam; }
	inline bool isKeyDown(BYTE keyState) { return !!(keyState & 0xF0); }

	inline bool isShiftPressed(CONST LPBYTE lpbKeyState)
	{ return (lpbKeyState[VK_SHIFT] & 0x80) ? true: false; }

	inline bool isCtrlPressed(CONST LPBYTE lpbKeyState)
	{ return (lpbKeyState[VK_CONTROL] & 0x80) ? true: false; }

	inline bool isAltPressed(CONST LPBYTE lpbKeyState)
	{ return (lpbKeyState[VK_MENU] & 0x80) ? true: false; }

	int onKeyShiftOnly(HIMC hIMC, LPARAM lKeyData);
	int onKeyShift(HIMC hIMC, UINT uVKey, LPARAM lKeyData);
	int onKeyCtrl(HIMC hIMC, UINT uVKey);
	int onKeyCtrlAlt(HIMC hIMC, UINT uVKey);

	int onControlEvent(
		HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState);
	BOOL onTypingEvent(
		HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState);

protected:
	ImmController();
	~ImmController(void);

private:
	static ImmController* m_self;

	DWORD m_shiftPressedTime;
	bool m_isCompStarted;
};

#endif