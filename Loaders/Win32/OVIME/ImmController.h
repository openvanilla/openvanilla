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
	static ImmController* open(ImmModel* model);
	static void close(void);

	inline KeyInfo getKeyInfo(LPARAM lparam) { return *(KeyInfo*)&lparam; }
	inline bool isKeyDown(BYTE keystate) { return !!(keystate & 0xF0); }

	inline bool isShiftPressed(CONST LPBYTE lpbKeyState)
	{ return (lpbKeyState[VK_SHIFT] & 0x80) ? true: false; }

	inline bool isCtrlPressed(CONST LPBYTE lpbKeyState)
	{ return (lpbKeyState[VK_CONTROL] & 0x80) ? true: false; }

	inline bool isAltPressed(CONST LPBYTE lpbKeyState)
	{ return (lpbKeyState[VK_MENU] & 0x80) ? true: false; }

	BOOL onKeyShift(HIMC hIMC, LPARAM lKeyData);
	BOOL onControlEvent(
		HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState);
	BOOL onTypingEvent(
		HIMC hIMC, UINT uVKey, CONST LPBYTE lpbKeyState);

protected:
	ImmController(ImmModel* model);
	~ImmController(void);

private:
	static ImmController* m_self;
	ImmModel* m_model;

	AVLoader* m_loader;

	DWORD m_shiftPressedTime;
	bool m_isCompStarted;
};

#endif