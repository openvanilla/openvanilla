// CIMDynamicLoader.cpp

#include "CIM.h"

int CIMCustomInitialize(MenuRef mnu)
{
    if (IMInitialize) return IMInitialize(mnu);
    return 1;
}

void CIMCustomTerminate()
{
    if (IMTerminate) IMTerminate();
}

void *CIMCustomOpen()
{
    if (IMOpenSession) return IMOpenSession();
    return NULL;
}

int CIMCustomClose(void *data)
{
    if (IMCloseSession) return IMCloseSession(data);
    return 1;
}

int CIMCustomActivate(void *data, CIMInputBuffer *buf)
{
    if (IMActivateSession) return IMActivateSession(data, buf);
    return 1;
}

int CIMCustomDeactivate(void *data, CIMInputBuffer *buf)
{
    if (IMDeactivateSession) return IMDeactivateSession(data, buf);
    return 1;
}

int CIMCustomHandleInput(void *data, CIMInputBuffer *buf, unsigned char charcode,
		UInt32 keycode, UInt32 modifiers,  
		Point *pnt)
{
    if (IMHandleInput) return IMHandleInput(data, buf, charcode, keycode, modifiers,
        pnt);
    return 0;
}


int CIMCustomMenuHandler(void *data, UInt32 command, MenuRef mnu, 
    CIMInputBuffer *buf)
{
    if (IMMenuHandler) return IMMenuHandler(data, mnu, buf);
    return 0;
}
