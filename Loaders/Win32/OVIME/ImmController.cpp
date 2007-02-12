#include "ImmController.h"

ImmController* ImmController::m_self = NULL;

ImmController::ImmController(ImmModel* model)
{
	m_model = model;
	m_loader = AVLoader::getLoader();

	m_shiftPressedTime = 0;
	m_isCompStarted = false;
}

ImmController::~ImmController(void)
{
	m_model->close();
	m_model = NULL;

	m_self = NULL;
}

ImmController* ImmController::open(ImmModel* model)
{
	if(m_self == NULL)
		m_self = new ImmController(model);

	return m_self;
}

void ImmController::close(void)
{
	if(m_self) delete m_self;
}

BOOL ImmController::onKeyShift(HIMC hIMC, LPARAM lKeyData)
{
	BOOL isShiftEaten = FALSE;

	if(m_shiftPressedTime == 0)
		m_shiftPressedTime = GetTickCount();
	else if(getKeyInfo(lKeyData).isKeyUp)
	{
		if(GetTickCount() - m_shiftPressedTime <= 20000)
		{
			//Toggle Chinese/English mode.
			//lParam == 2
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 2);
			isShiftEaten = TRUE;
		}

		m_shiftPressedTime = 0;        
    }

	return isShiftEaten;
}

BOOL ImmController::onControlEvent
(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	if(isCtrlPressed(lpbKeyState))
	{
		if(isAltPressed(lpbKeyState)) {
			switch(LOWORD(uVKey)) {
				case VK_G:
					//Toggle Traditional / Simplified Chinese.
					//lParam == 4
					murmur("IME.cpp: ctrl+alt+g");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 4);
					return TRUE;  // ctrl+ alt +g
				case VK_K:
					//Toggle Large Candidate window.
					//lParam == 6
					murmur("IME.cpp: ctrl+alt+K");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 6);
					return TRUE;
				case VK_L:
					// Test Notify window.
					murmur("IME.cpp: ctrl+alt+L");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 7);
					return TRUE;
				default:
					return FALSE;
			}
		}
		else
		{
			switch(LOWORD(uVKey)) {
				case VK_OEM_5:
					//Change the module by Ctrl+"\":
					//lParam == 8
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 8);
					return TRUE;
				case VK_OEM_PLUS:					
					//Change the BoPoMoFo keyboard layout by Ctrl+"=":
					//lParam == 5
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 5);
					return TRUE;
				case VK_SPACE: //is this necessary?
					return TRUE;
				default:
					return FALSE;
			}
		}
	}

	switch(uVKey) {
		case VK_CONTROL:
		case VK_MENU:
			return FALSE;
		case VK_SHIFT:
		{
			BOOL isShiftPressedOnly = onKeyShift(hIMC, lKeyData);
			BOOL isKeyProcessed = FALSE;
			if(isShiftPressedOnly) {
				//<comment author='b6s'>Tell the module that Shift was pressed
					AVKeyCode keycode;
					keycode.setShift(1);
				//</comment>

				//<comment author='b6s'>Redundant module processing
				if(m_loader->keyEvent(UICurrentInputMethod(), keycode)) {
					isKeyProcessed = TRUE;
					MyGenerateMessage(hIMC,
							WM_IME_COMPOSITION, 0, GCS_COMPSTR);
				} else {
					if(m_isCompStarted &&
						wcslen(GETLPCOMPSTR(m_model->getCompStr())) == 0)
					{
						m_isCompStarted = false; //要先做!
						MyGenerateMessage(hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);						
					}
				}
				//</comment>

				/*暫時治標拿掉，應該還是要unlock，然後在 dsvr 裡面所有地方要 lock/unlock
				dsvr->releaseIMC();  
				*/
				//return retVal;
			}

			return isKeyProcessed;
		}
	}

	return TRUE;
}

BOOL ImmController::onTypingEvent
(HIMC hIMC, UINT uVKey, CONST LPBYTE lpbKeyState)
{
	if(!m_isCompStarted &&
		wcslen(GETLPCOMPSTR(m_model->getCompStr())) == 0)	
	{
		m_isCompStarted = true;//要先做!
		MyGenerateMessage(hIMC, WM_IME_STARTCOMPOSITION, 0, 0);		
	}

	int k = LOWORD(uVKey);
	if( k >= 65 && k <= 90)
		k = k + 32;
	WORD out[2];
	int spec =
		ToAscii(uVKey, MapVirtualKey(uVKey, 0), lpbKeyState, (LPWORD)&out, 0);
	murmur("KEY: %c\n", out[0]);
	switch(LOWORD(uVKey))	{
	case VK_PRIOR: // pageup
		k = 11;
		break;
	case VK_NEXT: // pagedown
		k = 12;
		break;
	case VK_END:
		k = 4;
		break;
	case VK_HOME:
		k = 1;
		break;
	case VK_LEFT:
		k = 28;
		break;
	case VK_UP:
		k = 30;
		break;
	case VK_RIGHT:
		k = 29;
		break;
	case VK_DOWN:
		k = 31;
		break;
	case VK_DELETE:
		k = 127;
		break;
	default:
		//DebugLog("uVKey: %x, %c\n", LOWORD(uVKey), LOWORD(uVKey));
		break;
	}
	
	if(spec == 1)
		k = (char)out[0];

	AVKeyCode keycode(k);
	DWORD conv, sentence;
	ImmGetConversionStatus( hIMC, &conv, &sentence);

	if( !(conv & IME_CMODE_NATIVE) )	//Alphanumeric mode
		keycode.setShift(1);
		//<comment author='b6s'>Unbind CapsLock and Alphanumeric mode
		//keycode.setCapslock(1);
		//</comment>		
	if(LOWORD(lpbKeyState[VK_CAPITAL]))
		keycode.setCapslock(1);
	if(isShiftPressed(lpbKeyState))
		keycode.setShift(1);
	if(isCtrlPressed(lpbKeyState))
		keycode.setCtrl(1);
	if(isAltPressed(lpbKeyState))
		keycode.setAlt(1);
	if((lpbKeyState[VK_NUMLOCK])
		&& (uVKey >= VK_NUMPAD0)
		&& (uVKey <= VK_DIVIDE))
		keycode.setNum(1);
	
	BOOL retVal = FALSE;
	if(m_loader->keyEvent(UICurrentInputMethod(), keycode)) //如果目前模組處理此key
	{
		retVal = TRUE;

		if(LOWORD(uVKey) != VK_RETURN)
			MyGenerateMessage(hIMC, WM_IME_COMPOSITION, 0, GCS_COMPSTR);
		else {
			MyGenerateMessage(hIMC, WM_IME_COMPOSITION, 0, GCS_RESULTSTR);
			m_isCompStarted = false; //要先做!
			MyGenerateMessage(hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);
			//InitCompStr(m_model->getCompStr());
		}
	} else {
		retVal = FALSE;
		//James comment: 解決未組成字之前選字 comp window 會消失的問題(?待商榷)

		if(m_isCompStarted &&
			wcslen(GETLPCOMPSTR(m_model->getCompStr())) == 0)
		{
			m_isCompStarted = false; //要先做!
			MyGenerateMessage(hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);
		}
	}

	return retVal;
}
