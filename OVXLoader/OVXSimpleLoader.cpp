// OVXSimpleLoader.cpp

#include <Carbon/Carbon.h>
#include <dlfcn.h>
#include <sys/syslimits.h>
#include "CIM.h"
#include "OpenVanilla.h"
#include "OVLoadable.h"
#include "VXTextBar.h"
#include "VXBuffer.h"
#include "VXKeyCode.h"
#include "VXConfig.h"

struct OVXLoadableLibrary
{
    OVXLoadableLibrary()
    {
        strcpy (path, "");
        libhandle=NULL;
        availableim=1;
        unloadable=0;
        imnew=NULL;
        imdelete=NULL;
    }
    
    int load(char *libpath, char *workpath)
    {
        fprintf (stderr, "loading loadable library %s, working path=%s\n", libpath, workpath);
        strcpy(path, libpath);
        if (!(libhandle=dlopen(path, RTLD_LAZY)))
        {
            fprintf (stderr, "loading failed\n");
            return 0;
        }

        imnew=(OVLNewType*)dlsym(libhandle, "OVLoadableNewIM");
        imdelete=(OVLDeleteType*)dlsym(libhandle, "OVLoadableDeleteIM");
        OVLVersionType *ver=(OVLVersionType*)dlsym(libhandle, "OVLoadableVersion");
        OVLAvailableType *avl=(OVLAvailableType*)dlsym(libhandle, 
            "OVLoadableAvailableIMCount");
        OVLUnloadType *unl=(OVLUnloadType*)dlsym(libhandle, "OVLoadableCanUnload");
        
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
    
    char path[PATH_MAX];
    void *libhandle;
    int availableim;
    int unloadable;
    unsigned int version;
    OVLNewType *imnew;
    OVLDeleteType *imdelete;
};


class CIMContext
{
public:
    CIMContext() : ovcontext(NULL), onScreen(0) {}
    VXTextBar bar;
    OVIMContext *ovcontext;
    int onScreen;
};

OVXLoadableLibrary *lib=NULL;
OVInputMethod *inputmethod=NULL;
VXConfig *sysconfig=NULL;
const char *configfile="/Library/OpenVanilla/Development/OVXSimpleLoader.config";
const char *plistfile="/Library/OpenVanilla/Development/OVXSimpleLoader.plist";

char imid[256];

OVDictionary *GetGlobalConfig()
{
	OVDictionary *d=sysconfig->getDictionaryRoot();
	
	if (!d->keyExist("global_settings")) d->newDictionary("global_settings");
	return d->getDictionary("global_settings");
}

OVDictionary *GetLocalConfig(char *id)
{
	OVDictionary *d=sysconfig->getDictionaryRoot();

	char buf[256];
	strcpy (buf, "local_settings-");
	strcat (buf, id);

	if (!d->keyExist(buf)) d->newDictionary(buf);
	return d->getDictionary(buf);
}


int CIMCustomInitialize(MenuRef mnu)
{
	char buf[256];
    fprintf (stderr, "OVXSimpleLoader: initializing\n");

	sysconfig=new VXConfig(plistfile);

    // open OVXSimpleLoader's config file

    FILE *config=fopen(configfile, "r");
    if (!config)
    {
        fprintf (stderr, "OVXSimpleLoader: can't find config %s\n", configfile);
        return 0;
    }
    
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
	int i=0;
    for (i=strlen(workpath)-1; i>0; i--) if (workpath[i]=='/') break;
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

	if (!inputmethod) return 0;
	
	inputmethod->identifier(imid);
    fprintf (stderr, "loaded input method id=%s\n", imid);
        
	OVDictionary *global=GetGlobalConfig();
	OVDictionary *local=GetLocalConfig(imid);

    OVService srv;
    inputmethod->initialize(global, local, &srv, workpath);
    // TODO: put OS X locale information here
    OVEncoding e;
    int l=inputmethod->name("zh_TW", buf, &e);
    CFStringRef imname=VXCreateCFString(buf, e, l);
    InsertMenuItemTextWithCFString(mnu, imname, 0, 0, 'OVXL');
	CFRelease(imname);

	InsertMenuItemTextWithCFString(mnu, CFSTR("Preferences..."), 0, 0, 'PREF');
	
	sysconfig->write();
	
	delete global;
	delete local;
    
    return 1;
}

void CIMCustomTerminate()
{    
    fprintf (stderr, "OVXSimpleLoader: Terminating\n");
    if (inputmethod)
	{
		OVDictionary *global=GetGlobalConfig();
		OVDictionary *local=GetLocalConfig(imid);
		OVService srv;
		inputmethod->terminate(global, local, &srv);
		sysconfig->write();
		delete global;
		delete local;
		
		if (lib) lib->imdelete(inputmethod);
	}
	
    if (lib) lib->unload();
	
	delete sysconfig;
}

void *CIMCustomOpen()
{
    CIMContext *c=new CIMContext;
    if (inputmethod) c->ovcontext=inputmethod->newContext();
    return c;
}

int CIMCustomClose(void *data)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;
    if (inputmethod) inputmethod->deleteContext(c->ovcontext);
    return 1;
}

int CIMCustomActivate(void *data, CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;

	if (sysconfig->changed())
	{
		sysconfig->read();
	
		OVDictionary *global=GetGlobalConfig();
		OVDictionary *local=GetLocalConfig(imid);
		OVService srv;
		inputmethod->update(global, local);
		c->onScreen=0;
		c->ovcontext->clear();
		delete global;
		delete local;
	}


    if (c->onScreen)
    {
        c->onScreen=0;
        
        if (buf->length()) 
        {
            c->bar.show();
            c->onScreen=0;
        }
        else
        {
            c->bar.hide()->clear();
            c->ovcontext->clear();    
        }
    }    
	
	OVService srv;
	c->ovcontext->activate(&srv);
    return 1;
}

int CIMCustomDeactivate(void *data, CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;

    if (c->bar.onScreen())
    {
        c->onScreen=1;
        c->bar.hide();
    }

	OVService srv;
	c->ovcontext->deactivate(&srv);

    return 1;
}

int CIMCustomHandleInput(void *data, CIMInputBuffer *buf, unsigned char charcode,
		UInt32 keycode, UInt32 modifiers, Point *pnt)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;

	OVService srv;
	VXBuffer vxb;
	VXKeyCode key;

	vxb.bind(buf);
	key.set(charcode, keycode, modifiers);

    fprintf (stderr, "received keycode=%d, buffer position=(%d,%d)\n",
        charcode, pnt->h, pnt->v);
        
    c->bar.setPosition(pnt->h, pnt->v);
	return c->ovcontext->keyEvent(&key, &vxb, &c->bar, &srv);
}

int CIMCustomMenuHandler(void *data, UInt32 command, MenuRef mnu, 
    CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*)data;    
	char sbuf[512];
    switch (command)
    {
		case 'PREF':
			fprintf (stderr, "launching application to edit %s\n", plistfile);
			sprintf (sbuf, "open %s", plistfile);
			system(sbuf);
			return 1;
			
			
        case 'OVXL':
            fprintf (stderr, "Menu item selected\n");
            return 1;
    }
    
    return 0;
}
