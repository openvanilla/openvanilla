//#define OV_DEBUG
//#define showCompWindow  //Uncomment this to show composition window.
#include "ImmController.h"
#ifdef WINCE
#include "OVIMEUI.h"
#endif

ImmController* ImmController::m_self = NULL;

ImmController::ImmController()
{ 
	m_shiftPressedTime = 0;
	m_isCompStarted = false;
}

ImmController::~ImmController(void)
{
	m_self = NULL; 
} 

ImmController* ImmController::open()
{
	if(m_self == NULL)
		m_self = new ImmController();

	return m_self;
}

void ImmController::close(void)
{
	if(m_self) delete m_self;
}

int ImmController::onKeyShiftOnly(HIMC hIMC, LPARAM lKeyData)
{
	int shiftState;
	if(!getKeyInfo(lKeyData).isKeyUp)  // Shift pressed
	{
		murmur("shift-only: down");
		m_shiftPressedTime = GetTickCount(); 
		shiftState = 1;
	}
	else if(GetTickCount() - m_shiftPressedTime < 200) // Shift Up
	{
		murmur("shift-only: up");
		//Toggle Chinese/English mode.
		//lParam == 2
#ifdef WINCE
		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);
		if(conv & IME_CMODE_NATIVE) conv &= !IME_CMODE_NATIVE;
		else conv |= IME_CMODE_NATIVE;
		ImmSetConversionStatus(hIMC, conv, sentence);
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
#else
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 2);
#endif
		shiftState = 2;
		m_shiftPressedTime = 0;
	}
	else   //Shift Up too late
	{ 
		murmur("shift-only: other");
		shiftState = 0;
		m_shiftPressedTime = 0;
	}

	return shiftState;
}

int ImmController::onKeyShift(HIMC hIMC, UINT uVKey, LPARAM lKeyData)
{
	murmur("S: vkey =%u", LOWORD(uVKey));
	int processState;
	if(LOWORD(uVKey) == VK_SPACE)   //shift + space
	{
		
		murmur("S_Space: Full-Half char");
#ifdef WINCE
		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);
		if(conv & IME_CMODE_FULLSHAPE) conv &= !IME_CMODE_FULLSHAPE;
		else conv |= IME_CMODE_FULLSHAPE;
		ImmSetConversionStatus(hIMC, conv, sentence);
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
#else
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 1);
#endif
		m_shiftPressedTime = 0;
		
		processState = 1;
	}
	else if(LOWORD(uVKey) == VK_SHIFT)  // only shift
	{
		switch(onKeyShiftOnly(hIMC, lKeyData)) {
			case 1:
				murmur("S: EN-ZH: waiting for key-up");
				processState = 1;
				break;
			case 2:
				murmur("S: EN-ZH: proceeded");
				processState = 2;
				break;
			case 0:
			default:
				murmur("S: passed");
				m_shiftPressedTime = 0;
				processState = 0;
				break;
		}
	}
	else  //shift + ? 
	{
		murmur("S_%u: assume normal", LOWORD(uVKey));
		m_shiftPressedTime = 0;
		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);

		if(!(conv & IME_CMODE_NATIVE) &&!(conv & IME_CMODE_FULLSHAPE))  //processState = 0;
			processState = 0;//Half-shape alphanumeric mode
		else
			processState = 2;
	}
	return processState;
}

int ImmController::onKeyCtrl(HIMC hIMC, UINT uVKey)
{
	int processState;
	switch(LOWORD(uVKey)) {
		case VK_CONTROL:
			murmur("C: passed");
			processState = 0;
			break;
		case VK_MENU:
			murmur("C_A: passed");
			processState = 0;
			break;
#ifdef WINCE
		case VK_BACKSLASH:
#else
		case VK_OEM_5:
#endif
			//Change the module by Ctrl+"\":
			//lParam == 8
			murmur("C_\\: change module");
#ifdef WINCE
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 8);
#endif
			processState = 1;
			break;
#ifdef WINCE
		case VK_EQUAL:
#else
		case VK_OEM_PLUS:					
#endif
			//Change the BoPoMoFo keyboard layout by Ctrl+"=":
			//lParam == 5
			murmur("C_=: change Hsu's layout");
#ifdef WINCE
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 5);
#endif
			processState = 1;
			break;
		case VK_SPACE:
			murmur("C_Space: switch IME");
#ifdef WINCE
			if(ImmGetOpenStatus(hIMC))
				ImmSetOpenStatus(NULL, FALSE);
			else
				ImmSetOpenStatus(NULL, TRUE);
#endif
			processState = 1;
			break;
		case VK_SHIFT:
			murmur("C_S: rotate IME");
			processState = 1;
			break;
		default:
			murmur("C_%u: assume normal", LOWORD(uVKey));
			processState = 2;
	}
	return processState;
}

int ImmController::onKeyCtrlAlt(HIMC hIMC, UINT uVKey)
{
	int processState;
	switch(LOWORD(uVKey)) {
		case VK_MENU:
		case VK_CONTROL:
			murmur("C_A: passed");
			processState = 0;
			break;
		case VK_G:
			//Toggle Traditional / Simplified Chinese.
			//lParam == 4
			murmur("C_A_g: TW-CN");
#ifdef WINCE
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 4);
#endif
			processState = 1;
			break;
		case VK_K:
			//Toggle Large Candidate window.
			//lParam == 6
#ifdef WINCE
#else
			murmur("C_A_k: Expand Cand");
#endif
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 6);
			processState = 1;
			break;
		case VK_L:
			// Test Notify window.
			murmur("C_A_l: Notify");
#ifdef WINCE
#else
			MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 7);
#endif
			processState = 1;
			break;
		default:
			murmur("C_A_%u: assume normal", LOWORD(uVKey));
			processState = 2;
			break;
	}
	return processState;
}

int ImmController::onControlEvent
(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	int processState;

	if(getKeyInfo(lKeyData).isKeyUp) 
	{
		if(LOWORD(uVKey) == VK_SHIFT) {
			processState = onKeyShift(hIMC, uVKey, lKeyData);
		} else {
			murmur("other key up");
			m_shiftPressedTime = 0;
			processState = 0;
		}
	}
	else if(isCtrlPressed(lpbKeyState))
	{
		if(isAltPressed(lpbKeyState)) {
			murmur("ctrl-alt state");
			processState = onKeyCtrlAlt(hIMC, uVKey);
		}
		else {
			murmur("ctrl state");
			processState = onKeyCtrl(hIMC, uVKey);
		}
	}
#ifdef WINCE
	else if(isShiftPressed(lpbKeyState)|| m_shiftPressedTime) {
#else
	else if(isShiftPressed(lpbKeyState)) {
#endif
		murmur("shift state");
		processState = onKeyShift(hIMC, uVKey, lKeyData);

		//else processState = onKeyShift(hIMC, uVKey, lKeyData);
	}
	else if(isFnPressed(uVKey)) {
		murmur("Fn pressed");
		processState = 0;
	}
	else {
		murmur("other state: assume normal");
		m_shiftPressedTime = 0;

		DWORD conv, sentence;
		ImmGetConversionStatus(hIMC, &conv, &sentence);
		
		if(!(conv & IME_CMODE_NATIVE) && !(conv & IME_CMODE_FULLSHAPE))
			processState = 0; //half-shape alphanumeric mode
		else processState = 2;
	}
	return processState;
}

BOOL ImmController::onTypingEvent
(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	BOOL isProcessed = FALSE;
	
	dsvr->hideNotify();  // hide notify window for any typing event.
	//if(getKeyInfo(lKeyData).isKeyUp) return isProcessed;
/*
	DWORD conv, sentence; 
	ImmGetConversionStatus(hIMC, &conv, &sentence); 
	//Alphanumeric mode
	if(!(conv & IME_CMODE_NATIVE)) return isProcessed;
*/	

	ImmModel* model = ImmModel::open(hIMC);
	if(!m_isCompStarted)// && wcslen(GETLPCOMPSTR(model->getCompStr())))
	{
		murmur("IMM:STARTCOMPOSITION"); 
		MyGenerateMessage(hIMC, WM_IME_STARTCOMPOSITION, 0, 0);
#ifdef showCompWindow
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, WM_IME_STARTCOMPOSITION);
#endif
		m_isCompStarted = true;//要先做!	
		

	}
	//if(wcslen(GETLPCOMPSTR(model->getCompStr())))
	//		m_isCompStarted = true;


	ImmModel::close();

	int k = LOWORD(uVKey);
	if( k >= 65 && k <= 90)
		k = k + 32;

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

#ifdef WINCE
	murmur("uVKey: %c, hex:%x", k, k);
	UINT spec=MapVirtualKeyW(uVKey, 2);
	if(spec) k= (char)spec;
	
	
	if(isShiftPressed(lpbKeyState)) {
		switch(LOWORD(uVKey)) {
			case VK_1:
				k='!';
				break;
			case VK_2:
				k='@';
				break;
			case VK_3:
				k='#';
				break;
			case VK_4:
				k='$';
				break;
			case VK_5:
				k='%';
				break;
			case VK_6:
				k='^';
				break;
			case VK_7:
				k='&';
				break;
			case VK_8:
				k='*';
				break;
			case VK_9:
				k='(';
				break;
			case VK_0:
				k=')';
				break;
			case VK_SEMICOLON:
				k=';';
				break;
			case VK_COMMA:
				k='<';
			case VK_HYPHEN:
				k='_';
				break;
			case VK_EQUAL:
				k='+';
			case VK_PERIOD:
				k='>';
			case VK_SLASH:
				k='?';
				break;
				
			default:
				break;
		}	
	}
	murmur("Mapped Keycode: %c, hex:%x", k, k);

#else
	WORD out[2];
	int spec =
		ToAscii(uVKey, MapVirtualKey(uVKey, 0), lpbKeyState, (LPWORD)&out, 0);
	if(spec == 1) k = (char)out[0];
	murmur("KEY: %c", out[0]);
#endif

	DWORD conv, sentence;	
	AVKeyCode keycode(k);
	ImmGetConversionStatus(hIMC, &conv, &sentence);
	//Force Capslock on Fullshape mode
	if(LOWORD(lpbKeyState[VK_CAPITAL])  || (!(conv & IME_CMODE_NATIVE)&& (conv & IME_CMODE_FULLSHAPE)))
		keycode.setCapslock(1);
	if(!(conv & IME_CMODE_FULLSHAPE)) {
		if(isShiftPressed(lpbKeyState) || LOWORD(uVKey) == VK_SHIFT)
			keycode.setShift(1);
		}
	else if(isShiftPressed(lpbKeyState) !=  !!(LOWORD(lpbKeyState[VK_CAPITAL])))
			keycode.setShift(1);
		
	if(isCtrlPressed(lpbKeyState))
		keycode.setCtrl(1);
	if(isAltPressed(lpbKeyState))
		keycode.setAlt(1);
	if((lpbKeyState[VK_NUMLOCK])
		&& (uVKey >= VK_NUMPAD0)
		&& (uVKey <= VK_DIVIDE))
		keycode.setNum(1);
	
	AVLoader* loader = AVLoader::open();
	murmur("Send to loader->keyevent, currentIC,%d", UICurrentInputMethod());
	if(loader->keyEvent(UICurrentInputMethod(), keycode)) //如果目前模組處理此key
	{
		isProcessed = TRUE;
		//<comment author='b6s'>
		// Moves this block back to DisplayServer.
		/*
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
		*/
		//</comment>
	
	} else {
		
		model = ImmModel::open(hIMC);
		//James comment: 解決未組成字之前選字 comp window 會消失的問題(?待商榷)
		// Force left right keys moving inside comp buffer.  Avoid problems when applications handle composition buffer (office 2007).
		if(wcslen(GETLPCOMPSTR(model->getCompStr())) &&( LOWORD(uVKey) == VK_LEFT ||  LOWORD(uVKey) ==VK_RIGHT) )
			isProcessed = true;
		

		if(m_isCompStarted &&
			wcslen(GETLPCOMPSTR(model->getCompStr())) == 0)
		{
			murmur("IMM:ENDCOMPOSITION");
			m_isCompStarted = false; //要先做!
			MyGenerateMessage(hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);
#ifdef showCompWindow
			MyGenerateMessage(hIMC,	WM_IME_NOTIFY, IMN_PRIVATE, WM_IME_ENDCOMPOSITION);
#endif
			
		}
		ImmModel::close(); 		
		

	}
	
	

	return isProcessed; 
}
