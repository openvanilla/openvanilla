// OVXSimpleLoader.cpp


#include "CIM.h"
#include "VXTextBar.h"
#include "VXBuffer.h"
#include "VXKeyCode.h"
#include <dlfcn.h>

class CustomCounter
{
public:
    CustomCounter() : cntr(0) {}
    
    void add() { cntr++; }
    int counter() { return cntr; }
    void print() { fprintf (stderr, "total chars typed=%d\n", cntr); }
    void activate() { fprintf (stderr, "activated\n"); }
    void deactivate() { fprintf (stderr, "deactivated\n"); }       
    
	VXTextBar bar;
	
protected:
    int cntr;
};

void *libh=NULL;
typedef OVInputMethod* create_t();
typedef void destroy_t(OVInputMethod*);
create_t *create_derived;
destroy_t *destroy_derived;
OVInputMethod *expim;
OVIMSession *expsession;

int CIMCustomInitialize(MenuRef mnu)
{
    InsertMenuItemTextWithCFString(mnu, 
        CFSTR("Print total chars typed to stderr"), 0, 0, 'TOTL');
    fprintf (stderr, "custom IM initialized\n");
	


	return 1;
}

void CIMCustomTerminate()
{
    fprintf (stderr, "custom IM terminated\n");
}

void *CIMCustomOpen()
{
	if (libh) return new CustomCounter;

	libh=dlopen("/tmp/OVIMExample.dylib", RTLD_LAZY);
	
    if (!libh)
	{
		fprintf (stderr, "dynamic lib load failed\n");
		return NULL;
	}
	else
	{
		create_derived=(create_t*)dlsym(libh, "create");
		destroy_derived=(destroy_t*)dlsym(libh, "destroy");
	}
    
    if (!create_derived || !destroy_derived)
	{
		fprintf (stderr, "load function failed\n");
		return NULL;
	}
    
	expim=create_derived();
	expsession=expim->newsession();

    return new CustomCounter;
}

int CIMCustomClose(void *data)
{
	if (expsession) if (expim) expim->deletesession(expsession);
	if (expim) destroy_derived(expim);
	if (libh) dlclose(libh);
	libh=NULL;

    delete (CustomCounter*)data;
    return 1;
}




int CIMCustomActivate(void *data)
{
    CustomCounter *cntr=(CustomCounter*)data;
    cntr->activate();
	cntr->bar.show();
    return 1;
}

int CIMCustomDeactivate(void *data)
{
    CustomCounter *cntr=(CustomCounter*)data;
    cntr->deactivate();
	cntr->bar.hide();
    return 1;
}

int CIMCustomHandleInput(CIMInputBuffer *buf, unsigned char charcode,
		UInt32 keycode, UInt32 modifiers,  
		Point *pnt, void *data)
{
	VXBuffer vxb;
	vxb.bind(buf);
    CustomCounter *cntr=(CustomCounter*)data;
 
	OVService srv;
	
	fprintf (stderr, "point=%d,%d\n", pnt->h, pnt->v);

	cntr->bar.setposition(pnt->h, pnt->v);
	
	VXKeyCode key;
	key.set(charcode, keycode, modifiers);
	
	return expsession->keyevent(&key, &vxb, &cntr->bar, &srv);
}


int CIMCustomMenuHandler(UInt32 command, MenuRef mnu, void *data)
{
    CustomCounter *cntr=(CustomCounter*)data;
    switch (command)
    {
        case 'TOTL':
            cntr->print();
            return 1;
    }
    
    return 0;
}
