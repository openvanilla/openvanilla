// VXService.h

#ifndef __VXService_h
#define __VXService_h

#include <Carbon/Carbon.h>
#include <OpenVanilla/OpenVanilla.h>

class VXService : public OVService
{
public:
    virtual void beep()
	{
		SysBeep(30);
	}
};

#endif
