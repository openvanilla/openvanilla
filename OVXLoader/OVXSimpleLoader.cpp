// OVXSimpleLoader.cpp

#include "CIM.h"
#include "VXTextBar.h"
#include "VXBuffer.h"
#include "VXKeyCode.h"
#include <dlfcn.h>

struct OVXLoadableLibrary
{
    OVXLoadableLibrary()
    {
        strcpy (path, "");
        libhandle=NULL;
        availableim=1;
        unloadable=0;
        imnew=0;
        imdelete=0;
    }
    
    int load(char *libpath, char *workpath)
    {
        fprintf (stderr, "loading loadable library %s\n", p);
        strcpy(path, p);
        if (!libhandle=(p, RTLD_LAZY)) 
        {
            fprintf (stderr, "loading failed\n");
            return 0;
        }

        imnew=(OVLNewType*)dlsym(libh, "OVLoadableNewIM");
        imdelete=(OVLDeleteType*)dlsym(libh, "OVLoadbleDeleteIM");
        OVLVersionType ver=(OVLVersionType*)dlsym(libh, "OVLoadableVersion");
        OVLAvailableType avl=(OVLAvailableType*)dlsym(libh, 
            "OVLoadableAvailableIMCount");
        OVLUnloadType unl=(OVLUnloadType*)dysym(libh, "OVLoadableCanUnload");
        
        if (!imnew || !imdelete || !ver) 
        {
            fprintf (stderr, "Three required interface functions missing\n");
            return 0;
        }
        
        version=ver();
        if (unl) unloadable=unl();
        if (avl) availableim=avl(workpath);
        return 1;
    }
    
    int unload()
    {
        fprintf (stderr, "unloading loadable library %s\n", path);
        if (!unloadable) return 1;
        fprintf (stderr, "system function dlclose returns=%d, errmsg=%s\n", 
            dlclose(libhandle), dlerror());
    }
    
    char path[MAXPATH];
    void *libhandle;
    int availableim;
    int unloadable;
    unsigned int version;
    OVLNewType imnew;
    OVLDeleteType imdelete;
};


class CIMContext
{
public:
    CIMContext() : ovcontext(NULL), onscreen(0) {}
    VXTextBar bar;
    OVIMContext *ovcontext;
    int onscreen;
};

OVXLoadableLibrary *lib=NULL;
OVInputMethod *inputmethod=NULL;
const char *configfile="/Library/OpenVanilla/Development/OVXSimpleLoader.config";

int CIMCustomInitialize(MenuRef mnu)
{
    fprintf (stderr, "OVXSimpleLoader: initializing\n");

    // open OVXSimpleLoader's config file

    FILE *config=fopen(configfile, "r");
    if (!config)
    {
        fprintf (stderr, "OVXSimpleLoader: can't find config %s\n", configfile);
        return 0;
    }
    
    char buf[256];
    while (!feof(config))
    {
        fgets(buf, 255, config);
        
        // strips the trailing 0xa
        if (strlen(buf)) buf[strlen(buf)-1]=0;
        
        // ignores comment mark '#'
        if (buf[0]=='#') continue;
        
        // we have the first line now, break
        if (strlen(buf)) break;
    }
    
    char workpath[256];
    strcpy (workpath, buf);
    for (int i=strlen(workpath)-1; i>0; i--) if (workpath[i]=='/') break;
    workpath[i+1]=0;
    
    lib=new OVXLoadableLibrary;
    if (!lib->load(buf, workpath))
    {
        delete lib;
        lib=NULL;
        return 0;
    }
    
    if (lib)
    {
        try
        {
            inputmethod=lib->imnew(0);
        }
        catch (OVException e)
        {
            fprintf (stderr, "exception caught\n");
            lib->imdelete(inputmethod);
            inputmethod=NULL;
            return 0;
        }  
    }

    char buf[256];
    inputmethod->identifier(buf);
    fprintf ("loaded input method id=%s\n", buf);
        
    // TODO: put OS X locale information here
    OVEncoding e;
    int l=inputmethod->name("zh_TW", buf, &e);
    CFString imname=VXCreateCFString(buf, e, l);
    InsertMenuItemTextWithCFString(mnu, imname, 0, 0, 'OVXL');
    
    return 1;
}

void CIMCustomTerminate()
{    
    fprintf (stderr, "OVXSimpleLoader: Terminating\n");
    if (inputmethod) if (lib) lib->imdelete(inputmethod);
    lib->unload();
}

void *CIMCustomOpen()
{
    CIMContext *c=new CIMContext;
    if (inputmethod) c->ovcontext=inputmethod->newContext();
    return c;
}

int CIMCustomClose(void *data)
{
    CIMContext *c=(CIMContext*c)data;    
    if (!c->ovcontext) return 0;
    if (inputmethod) inputmethod->deleteContext(c);
    return 1;
}

int CIMCustomActivate(void *data, CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*c)data;    
    if (!c->ovcontext) return 0;

    if (c->onscreen)
    {
        onscreen=0;
        
        if (buf->length()) 
        {
            c->bar.show();
            onscreen=0;
        }
        else
        {
            c->bar.hide()->clear();
            c->ovcontext->clear();    
        }
    }       
    return 1;
}

int CIMCustomDeactivate(void *data, CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*c)data;    
    if (!c->ovcontext) return 0;

    if (bar.onscreen())
    {
        onscreen=1;
        bar.hide();
    }
    return 1;
}

int CIMCustomHandleInput(void *data, CIMInputBuffer *buf, unsigned char charcode,
		UInt32 keycode, UInt32 modifiers, Point *pnt)
{
    CIMContext *c=(CIMContext*c)data;    
    if (!c->ovcontext) return 0;

	OVService srv;
	VXBuffer vxb;
	VXKeyCode key;

	vxb.bind(buf);
	key.set(charcode, keycode, modifiers);

    fprintf (stderr, "received keycode=%d, buffer position=(%d,%d)\n",
        charcode, pnt->h, pnt->v);
        
    c->bar.setPosition(pnt->h, pnt->v);
	return c->ovcontext->keyEvent(&key, &vxb, &cntr->bar, &srv);
}

int CIMCustomMenuHandler(void *data, UInt32 command, MenuRef mnu, 
    CIMInputBuffer *buf)
{
    CustomCounter *cntr=(CustomCounter*)data;
    switch (command)
    {
        case 'OVXL':
            fprintf (stderr, "Menu item selected\n");
            return 1;
    }
    
    return 0;
}
