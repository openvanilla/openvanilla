#ifndef OVDISPLAYCOMPONENT_H
#define OVDISPLAYCOMPONENT_H

#include "OpenVanilla.h"
#include "OVUtility.h"

class OVDisplayServer;

class OVDisplayComponent : public OVModule
{
public:
	virtual void update()=0;
	virtual void regDisplayServer(OVDisplayServer *ds);
    virtual const char* moduleType() { return "OVDisplayComponent"; }
protected:
	OVDisplayServer *svr;
};

#endif //OVDISPLAYCOMPONENT_H