// OVIME2Server.cpp: implementation of the OVIME2Server class.
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <tchar.h>
#include <shlobj.h>

#include "OVIME2Server.h"
#include "OVIME2Instance.h"
#include ".\ovime2server.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static OVIME2Server* g_OVIME2ServerInst = NULL;

OVIME2Server::OVIME2Server()
	: hwnd(NULL), sharedMem(INVALID_HANDLE_VALUE)
{
	g_OVIME2ServerInst = this;

	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= (WNDPROC)OVIME2Server::wndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE)GetModuleHandle(NULL);
	wc.hCursor			= NULL;
	wc.hIcon			= NULL;
	wc.lpszMenuName		= (LPTSTR)NULL;
	wc.lpszClassName	= OVIME2ServerClassName;
	wc.hbrBackground	= NULL;
	wc.hIconSm			= NULL;
	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return;
	hwnd = CreateWindowEx(0, OVIME2ServerClassName, NULL, 0, 
					0, 0, 0, 0, HWND_DESKTOP, NULL, wc.hInstance, NULL);
}

OVIME2Server::~OVIME2Server()
{
	CloseHandle(sharedMem);
    OutputDebugString("OVIME server down.");
}

bool OVIME2Server::run()
{
	if( !hwnd )
		return false;
	if( !startServer() )
		return false;

	MSG msg;
	while( GetMessage(&msg, NULL, 0, 0 ) )
		DispatchMessage( &msg );

	return true;
}

LRESULT CALLBACK OVIME2Server::wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if( !g_OVIME2ServerInst->hwnd )
		g_OVIME2ServerInst->hwnd = hwnd;
	return g_OVIME2ServerInst->wndProc( msg, wp, lp );
}

LRESULT OVIME2Server::wndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
	case cmdKeyEvent:
		return keyEvent( wp, reinterpret_cast<OVIME2Instance*>(lp) );
		break;
	case cmdGetData:	// Retrive data in IMContext
		return getData( wp, reinterpret_cast<OVIME2Instance*>(lp) );
		break;
	case cmdGetCand:	// Get candidate strings
		return getCand( wp, reinterpret_cast<OVIME2Instance*>(lp) );
		break;
	case cmdEcho:
        {
            map<unsigned int, OVIME2Instance*>::iterator it;
            it = OVIME2Instances.find((unsigned int)lp);
            if ( it != OVIME2Instances.end() )
            {
                return  ~((LRESULT)it->second);
            }
        }
        return  -1; /* return -1 here, so clinet side can identify 
                       its response from server or API failed */
	case cmdAddClient:
		{
			OVIME2Instance* client = new OVIME2Instance( this );
			OVIME2Instances[(unsigned int)client] = client;
			return (LRESULT)client;
			break;
		}
	case cmdRemoveClient:
		{
			if ( OVIME2Instances.erase((unsigned int)lp)>0 ) {
                delete (OVIME2Instance*) lp;
				return TRUE;
            }
			return FALSE;
			break;
		}
	case cmdGetModList:
		{
			return getModList(  reinterpret_cast<OVIME2Instance*>(lp) );
			break;
		}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		{
			OVIME2Instances.clear();
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc( hwnd, msg, wp, lp );
}

bool OVIME2Server::startServer()
{
	HANDLE hprocess = GetCurrentProcess();

	LPCTSTR name = _T("Local\\OVIME2Server");
	LPCTSTR evtname = _T("Local\\OVIME2ServerEvent");
	DWORD osVersion = GetVersion();
 	DWORD major = (DWORD)(LOBYTE(LOWORD(osVersion)));
	DWORD minor =  (DWORD)(HIBYTE(LOWORD(osVersion)));
	if( osVersion >= 0x80000000 || major <= 4 )	// Windows 9x or Windows NT 4
	{
		name += 6;	// remove prfix "Local\\"
		evtname += 6;	// remove prfix "Local\\"
	}
	HANDLE evt = OpenEvent( EVENT_ALL_ACCESS, FALSE, evtname );
	SetWindowText( hwnd, name );

	sharedMem = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
						0, OVIME2_SERVER_BUF_SIZE, name );

	// Initialize AVEmbeddedLoader
	vector<OVModule*> tmpmod_vector;
	vector<OVModule*>::iterator m;
	for(m = modlist().begin(); m != modlist().end(); m++) {
		dict()->setDict((*m)->identifier());
		if(!strcmp((*m)->moduleType(), "OVInputMethod")) {
			if(!dict()->keyExist("enable")) {
				dict()->setInteger("enable", 1);
				tmpmod_vector.push_back(*m);
			} else {
				if(dict()->getInteger("enable")) {
					tmpmod_vector.push_back(*m);
				}
			}
		} else if(!strcmp((*m)->moduleType(), "OVOutputFilter")) {
			if(dict()->getInteger("enable")) {
				OVOutputFilter *of = dynamic_cast<OVOutputFilter*>(*m);
				murmur("load output filter %s", of->identifier());
				of->initialize(dict(), srv(), cfg()->getModuleDir());
				ovof_vector.push_back(of);
			} else {
				dict()->setInteger("enable", 0);
			}
		}
	}
	modlist() = tmpmod_vector; // Orz
//	dsvr = new DisplayServer();
//	loader->connectDisplayServer(dsvr);

	char str[1024];
	wchar_t wstr[1024];
	for(int i = 0;; ++i) {
		if(moduleName(i, str)) {
			MultiByteToWideChar(CP_UTF8, 0, str, strlen(str)+1, wstr, 1024);
//			UIPushInputMethod(wstr);
		} else {
			break;
		}
	}

	if( evt != INVALID_HANDLE_VALUE )
	{
		SetEvent(evt);
		CloseHandle(evt);
	}
    OutputDebugString("OVIME2 server up.");
	return true;
}


bool OVIME2Server::moduleName(int i, char *str)
{
	string s;
	int modVectorNum = static_cast<int>(modlist().size()) - 1;
	if(i > modVectorNum) {
		strcpy(str, "");
		return false;
	}
	s = modlist().at(i)->localizedName("zh_TW");
	strcpy(str, s.c_str());
	return true;
}


LRESULT OVIME2Server::keyEvent( WPARAM wp, OVIME2Instance* client )
{
	WORD key = LOWORD(wp);
	WORD state = HIWORD(wp);
	AVKeyCode code( key );
	if( state & isCtrl )
		code.setCtrl(1);
	if( state & isShift )
		code.setShift(1);
	if( state & isAlt )
		code.setAlt(1);
	if( state & isCapsLock )
		code.setCapslock(1);
	if( (state & isNum) && (key > 45 && key < 58))	//<comment author='b6s'>46 is "./Del" and 48~57 are [0-9] of NumPad, but what is 47?</comment>
		code.setNum(1);

	return LRESULT( client->keyEvent( code ) );
}

LRESULT OVIME2Server::getData( WPARAM wp, OVIME2Instance* client )
{
	char* pbuf = (char*)MapViewOfFile( sharedMem, FILE_MAP_WRITE, 
								0, 0, OVIME2_SERVER_BUF_SIZE );
	wstring comp_str = client->getCompStr();
	wstring result_str = client->getResultStr();
	wstring notify_str = client->getNotifyStr();

	char* p = pbuf;
	size_t size = (comp_str.length() + 1) * sizeof(wchar_t);
	memcpy( p, comp_str.c_str(), size );
	p += size;
	size = (result_str.length() + 1) * sizeof(wchar_t);
	memcpy( p, result_str.c_str(), size );
	p += size;
	size = (notify_str.length() + 1) * sizeof(wchar_t);
	memcpy( p, notify_str.c_str(), size );
	p += size;

	int cursor_pos = (BYTE)client->getCursorPos();
	*reinterpret_cast<int*>(p) = cursor_pos;
	p += sizeof(int);

	int mark_from = (BYTE)client->getMarkFrom();
	*reinterpret_cast<int*>(p) = mark_from;
	p += sizeof(int);

	int mark_to = (BYTE)client->getMarkTo();
	*reinterpret_cast<int*>(p) = mark_to;
	p += sizeof(int);

	int has_cand = !client->getCandidates().empty();
	*reinterpret_cast<int*>(p) = has_cand;
	p += sizeof(int);

	UnmapViewOfFile( pbuf );

	return (LONG(p) - LONG(pbuf));	// total length of data
}

LRESULT OVIME2Server::getCand( WPARAM wp, OVIME2Instance* client )
{
	char* pbuf = (char*)MapViewOfFile( sharedMem, FILE_MAP_WRITE, 
								0, 0, OVIME2_SERVER_BUF_SIZE );
	UnmapViewOfFile( pbuf );
	return 0;
}


LRESULT OVIME2Server::getModList(OVIME2Instance* client)
{
	char* pbuf = (char*)MapViewOfFile( sharedMem, FILE_MAP_WRITE, 
								0, 0, OVIME2_SERVER_BUF_SIZE );

	char* p = pbuf;
	*reinterpret_cast<int*>(p) = modlist().size();
	vector<OVModule*>::iterator m;
	string s;
	for(m = modlist().begin(); m != modlist().end(); m++) {
		s = (*m)->localizedName("zh_TW");
		wchar_t* wname;
		int len = srv()->UTF8ToUTF16( s.c_str(), &wname );
		memcpy( p, wname, len * sizeof(wchar_t) );
		reinterpret_cast<wchar_t*>(p)[len] = 0;
		p += (len + 1);
	}

	UnmapViewOfFile( pbuf );
	return (long(p) - long(pbuf));
}
