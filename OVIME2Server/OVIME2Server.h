// OVIME2Server.h: interface for the OVIME2Server class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVIME2Server_H__BA0A82BD_DE77_4E22_9B6C_443D7E3D22EB__INCLUDED_)
#define AFX_OVIME2Server_H__BA0A82BD_DE77_4E22_9B6C_443D7E3D22EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786) 

#include <windows.h>
#include <map>
#include <vector>

#include "AVEmbeddedLoader.h"

using namespace std;

#define		OVIME2ServerClassName		_T("OVIME2Server")
#define		OVIME2_SERVER_BUF_SIZE		4096

class OVIME2Instance;

class OVIME2Server : public AVEmbeddedLoader
{
public:
	enum OVIME2KeyState{
		isCtrl		= 1,
		isShift		= 1 << 1,
		isAlt		= 1 << 2,
		isCapsLock	= 1 << 3,
		isNum		= 1 << 4
	};

	enum OVIME2InstanceCmd
	{
		cmdFirst= (WM_APP + 1),
		cmdAddClient = cmdFirst,
		cmdRemoveClient,
        cmdEcho,
		cmdKeyEvent,
		cmdGetData,
		cmdGetCand,
		cmdGetModList
	};

public:
	bool run();
	OVIME2Server();
	virtual ~OVIME2Server();
	bool moduleName(int i, char *str);
	vector<OVOutputFilter*>& filters() { return ovof_vector; }

protected:
	vector<OVOutputFilter*> ovof_vector;

	bool startServer();
	HWND hwnd;
	LRESULT wndProc( UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	map<unsigned int, OVIME2Instance*> OVIME2Instances;

	HANDLE sharedMem;
	LRESULT keyEvent( WPARAM wp, OVIME2Instance* client );
	LRESULT getData( WPARAM wp, OVIME2Instance* client );
	LRESULT getCand( WPARAM wp, OVIME2Instance* client );
	LRESULT getModList(OVIME2Instance* client);
};

#endif // !defined(AFX_OVIME2Server_H__BA0A82BD_DE77_4E22_9B6C_443D7E3D22EB__INCLUDED_)
