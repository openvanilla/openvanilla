#define OV_DEBUG
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
	//m_model->close();
	m_model = NULL;
	m_self = NULL;
}

ImmController* ImmController::open(ImmModel* model)
{
	if(m_self == NULL)
		m_self = new ImmController(model);
	else if(model != m_self->m_model)
		m_self->m_model = model;

	return m_self;
}

void ImmController::close(void)
{
	if(m_self) delete m_self;
}

bool ImmController::onKeyShift(HIMC hIMC, LPARAM lKeyData)
{
	bool isShiftEaten = false;
	if(m_shiftPressedTime == 0)
		m_shiftPressedTime = GetTickCount();
	else if(getKeyInfo(lKeyData).isKeyUp)
	{
		if(GetTickCount() - m_shiftPressedTime < 300)
		{
			//Toggle Chinese/English mode.
			//lParam == 2
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 2);
			isShiftEaten = true;
		}
		m_shiftPressedTime = 0;
    }

	return isShiftEaten;
}

int ImmController::onControlEvent
(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	int processState = 0;
	if(isCtrlPressed(lpbKeyState))
	{
		murmur("control state");
		if(isAltPressed(lpbKeyState)) {
			murmur("alt state");
			switch(LOWORD(uVKey)) {
				case VK_G:
					//Toggle Traditional / Simplified Chinese.
					//lParam == 4
					murmur("C_A_g: TW-CN");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 4);
					processState = 1;
					break;
				case VK_K:
					//Toggle Large Candidate window.
					//lParam == 6
					murmur("C_A_k: Expand Cand");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 6);
					processState = 1;
					break;
				case VK_L:
					// Test Notify window.
					murmur("C_A_l: Notify");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 7);
					processState = 1;
					break;
				default:
					murmur("C_A: passed");
					processState = 2;
			}
		}
		else
		{
			switch(LOWORD(uVKey)) {
				case VK_OEM_5:
					//Change the module by Ctrl+"\":
					//lParam == 8
					murmur("C_\: change module");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 8);
					processState = 1;
					break;
				case VK_OEM_PLUS:					
					//Change the BoPoMoFo keyboard layout by Ctrl+"=":
					//lParam == 5
					murmur("C_=: change Hsu's layout");
					MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 5);
					processState = 1;
					break;
				/* Seems not necessary
				case VK_SPACE:
					murmur("C_space: switch IME");
					processState = 1;
					break;
				*/
				default:
					murmur("C: passed");
					processState = 2;
			}
		}
	}

	if(processState == 0) {
		switch(uVKey) {
			case VK_SHIFT:
			{
				murmur("shift vkey");
				if(onKeyShift(hIMC, lKeyData)) {
					murmur("S: EN-ZH");
					processState = 1;
				}
				else {
					murmur("S: passed");
					processState = 2;
				}
				break;
				/*
				BOOL isKeyProcessed = FALSE;
				if(isShiftPressedOnly) {
					//<comment author='b6s'>Tell the module that Shift was pressed
						AVKeyCode keycode;
						keycode.setShift(1);
					//</comment>

					//<comment author='b6s'>Redundant module processing
					if(m_loader->keyEvent(UICurrentInputMethod(), keycode)) {
						//isKeyProcessed = TRUE;
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
				}

				return isKeyProcessed;
				*/
			}
			case VK_CONTROL:
				murmur("control vkey");
				murmur("C: passed");
				processState = 2;
				break;
			case VK_MENU:
				murmur("alt vkey");
				murmur("A: passed");
				processState = 2;
				break;
		}
	}

	return processState;
}

BOOL ImmController::onTypingEvent
(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	BOOL isProcessed = FALSE;

	if(getKeyInfo(lKeyData).isKeyUp) return isProcessed;

		if(!m_isCompStarted &&
		wcslen(GETLPCOMPSTR(m_model->getCompStr())) == 0)	
	{
		murmur("STARTCOMPOSITION");
		m_isCompStarted = true;//要先做!
		MyGenerateMessage(hIMC, WM_IME_STARTCOMPOSITION, 0, 0);
	}

	int k = LOWORD(uVKey);
	if( k >= 65 && k <= 90)
		k = k + 32;
	WORD out[2];
	int spec =
		ToAscii(uVKey, MapVirtualKey(uVKey, 0), lpbKeyState, (LPWORD)&out, 0);
	murmur("KEY: %c", out[0]);
	switch(LOWORD(uVKey)) {
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
		return isProcessed;
		//keycode.setShift(1);
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
	
	if(m_loader->keyEvent(UICurrentInputMethod(), keycode)) //如果目前模組處理此key
	{
		isProcessed = TRUE;
		if(LOWORD(uVKey) != VK_RETURN) {
			murmur("COMPOSITION GCS_COMPSTR");
			MyGenerateMessage(hIMC, WM_IME_COMPOSITION, 0, GCS_COMPSTR);
		}
		else {
			murmur("COMPOSITION GCS_RESULTSTR");
			MyGenerateMessage(hIMC, WM_IME_COMPOSITION, 0, GCS_RESULTSTR);

			m_isCompStarted = false; //要先做!
			murmur("ENDCOMPOSITION");
			MyGenerateMessage(hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);
			//InitCompStr(m_model->getCompStr());
		}
	} else {
		//James comment: 解決未組成字之前選字 comp window 會消失的問題(?待商榷)
		if(m_isCompStarted &&
			wcslen(GETLPCOMPSTR(m_model->getCompStr())) == 0)
		{
			murmur("ENDCOMPOSITION");
			m_isCompStarted = false; //要先做!
			MyGenerateMessage(hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);
		}
	}

	return isProcessed;
}
