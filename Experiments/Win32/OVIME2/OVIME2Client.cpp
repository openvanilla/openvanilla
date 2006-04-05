// OVIME2Client.cpp: implementation of the OVIME2Client class.
//
//////////////////////////////////////////////////////////////////////

#include "OVIME2.h"
#include "OVIME2Client.h"
#include <string.h>
#include <tchar.h>
#include ".\ovime2client.h"
#include "OpenVanilla.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OVIME2Client::OVIME2Client()
	: serverWnd(NULL), clientID(0), sharedMem(INVALID_HANDLE_VALUE)
{
	ConnectServer();
}

OVIME2Client::~OVIME2Client()
{
	SendMessage( serverWnd, OVIME2Server::cmdRemoveClient, 0, clientID );
    if ( sharedMem==INVALID_HANDLE_VALUE )
    {
        CloseHandle(sharedMem);
    }
}

void OVIME2Client::ConnectServer(void)
{
	serverWnd = FindWindow( OVIME2ServerClassName, NULL );
	if( ! serverWnd )
	{
		LPCTSTR evtname = _T("Local\\ChewingServerEvent");
		DWORD osVersion = GetVersion();
 		DWORD major = (DWORD)(LOBYTE(LOWORD(osVersion)));
		DWORD minor =  (DWORD)(HIBYTE(LOWORD(osVersion)));
		if( osVersion >= 0x80000000 || major <= 4 )	// Windows 9x or Windows NT 4
			evtname += 6;	// remove prfix "Local\\"

		HANDLE evt = CreateEvent( NULL, TRUE, FALSE, evtname );
		TCHAR server_path[MAX_PATH];
		GetWindowsDirectory( server_path, MAX_PATH );
		_tcscat( server_path, _T("\\OpenVanilla\\OVIME2Server.exe") );
		ShellExecute( NULL, _T("open"), server_path, NULL, NULL, SW_HIDE );
		WaitForSingleObject( evt, 10000 );
		CloseHandle(evt);
		serverWnd = FindWindow( OVIME2ServerClassName, NULL );
	}
	clientID = SendMessage( serverWnd, OVIME2Server::cmdAddClient, 0, 0 );
	GetWindowText( serverWnd, filemapName, sizeof(filemapName) );
}

void OVIME2Client::SetFullShape(bool full)
{
	// SendMessage( serverWnd, OVIME2Server::cmdSetFullShape, full, clientID);
}

bool OVIME2Client::GetFullShape(void)
{
	// return !!SendMessage( serverWnd, OVIME2Server::cmdGetFullShape, 0, clientID);
	return false;
}

unsigned int OVIME2Client::EchoFromServer()
{	return (unsigned int)SendMessage( serverWnd, OVIME2Server::cmdEcho, 0, clientID);	}

bool OVIME2Client::CheckServer()
{
    if ( OVIME2Client::EchoFromServer()==(~clientID) )
    {
        return  true;
    }
    ConnectServer();
    return  false;
}


bool OVIME2Client::keyEvent(UINT vKey, LPARAM lParam, const BYTE* keyState)
{
	KeyInfo ki = GetKeyInfo(lParam);
	WORD key = 0;
	switch( vKey )
	{
	case VK_ESCAPE:
		key = ovkEsc;
		break;
	case VK_SPACE:
		key = ovkSpace;
		break;
	case VK_RETURN:
		key = ovkReturn;
		break;
	case VK_DELETE:
		key = ovkDelete;
		break;
	case VK_BACK:
		key = ovkBackspace;
		break;
	case VK_UP:
		key = ovkUp;
		break;
	case VK_DOWN:
		key = ovkDown;
		break;
	case VK_LEFT:
		key = ovkLeft;
		break;
	case VK_RIGHT:
		key = ovkRight;
		break;
	case VK_HOME:
		key = ovkHome;
		break;
	case VK_END:
		key = ovkEnd;
		break;
	case VK_PRIOR:
		key = ovkPageUp;
		break;
	case VK_NEXT:
		key = ovkPageDown;
		break;
	case VK_TAB:
		key = ovkTab;
		break;
	default:
		char ascii[2];
		int ret = ToAscii( vKey, ki.scanCode, (BYTE*)keyState, (LPWORD)ascii, 0);
		if( ret )
		{
//			if( key >= VK_NUMPAD0 && key <= VK_DIVIDE )
//			{
//				g_client->NumPad( ascii[0] );
//				return true;
//			}
			key = ascii[0];
		}
		else
			return false;
	}

	WORD state = 0;

	if( IsKeyDown( keyState[VK_CONTROL] ) )
		state |= OVIME2Server::isCtrl;
	if( IsKeyDown( keyState[VK_SHIFT] ) )
		state |= OVIME2Server::isShift;
	if( IsKeyDown( keyState[VK_MENU] ) )
		state |= OVIME2Server::isAlt;
	if( IsKeyToggled( keyState[VK_CAPITAL] ) )
		state |= OVIME2Server::isCapsLock;
	if( IsKeyToggled( keyState[VK_NUMLOCK] ) )
		state |= OVIME2Server::isNum;

	WPARAM wp = MAKEWPARAM( key, state );
	return ::SendMessage( serverWnd, OVIME2Server::cmdKeyEvent, wp, clientID );
}

int OVIME2Client::getData(wstring& comp_str, wstring& result_str, wstring& notify_str,
						  bool& has_cand, int& cursor_pos, int& mark_from, int& mark_to )
{
	DWORD total_size = ::SendMessage( serverWnd, 
								OVIME2Server::cmdGetData, 0, clientID );
	if( !total_size )
		return 0;

	sharedMem = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, filemapName);
    if ( sharedMem==NULL ) {
        sharedMem = INVALID_HANDLE_VALUE; 
        return  0;
    }

	char* buf = (char*)MapViewOfFile( sharedMem, FILE_MAP_READ, 0, 0, total_size );
	if( buf ) {
		char* p = buf;
		comp_str = reinterpret_cast<wchar_t*>(p);
		p += (comp_str.length()+1) * sizeof(wchar_t);
		result_str = reinterpret_cast<wchar_t*>(p);
		p += (result_str.length()+1) * sizeof(wchar_t);
		notify_str = reinterpret_cast<wchar_t*>(p);
		p += (notify_str.length()+1) * sizeof(wchar_t);

		cursor_pos = *reinterpret_cast<int*>(p);
		p += sizeof(int);

		mark_from = *reinterpret_cast<int*>(p);
		p += sizeof(int);

		mark_to = *reinterpret_cast<int*>(p);
		p += sizeof(int);

		has_cand = (bool)*reinterpret_cast<int*>(p);

		UnmapViewOfFile(buf);
	}
    CloseHandle(sharedMem);
    sharedMem = INVALID_HANDLE_VALUE;
	return total_size;
}

int OVIME2Client::getModList(vector<wstring>& list)
{
	DWORD total_size = ::SendMessage( serverWnd, 
								OVIME2Server::cmdGetModList, 0, clientID );
	if( !total_size )
		return 0;

	sharedMem = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, filemapName);
    if ( sharedMem==NULL ) {
        sharedMem = INVALID_HANDLE_VALUE; 
        return  0;
    }

	int n = 0;
	char* buf = (char*)MapViewOfFile( sharedMem, FILE_MAP_READ, 0, 0, total_size );
	if( buf ) {
		char* p = buf;
		n = *reinterpret_cast<int*>(p);
		p += sizeof(int);

		for( int i = 0; i < n; ++i ) {
			int len = wcslen( reinterpret_cast<wchar_t*>(p) );
			list.push_back( reinterpret_cast<wchar_t*>(p) );
			p += ( len * sizeof(wchar_t) );
		}
		UnmapViewOfFile(buf);
	}
    CloseHandle(sharedMem);
    sharedMem = INVALID_HANDLE_VALUE;
	return n;
}
