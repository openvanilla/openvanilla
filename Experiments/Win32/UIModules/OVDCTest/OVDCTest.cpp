#include "OpenVanilla.h"
#include "OVUtility.h"
#include "OVDisplayServer.h"
#include "OVLibrary.h"
#include "Utility.h"

#include <windows.h>

#define OV_DEBUG

class OVDCTest : public OVDisplayComponent
{
public:
	virtual void update() {
		if(!svr->isBufShow() && svr->getBufString() == "") return;

		wchar_t* dec = UTF16toWCHAR(svr->getBufString().c_str());
		if(dec[0])
			MessageBoxW(NULL, dec, L"Update", MB_OK);
		free(dec);
	}
	virtual const char *identifier() { return "OVDCTest"; }
};

// the module wrapper
OV_SINGLE_MODULE_WRAPPER(OVDCTest);