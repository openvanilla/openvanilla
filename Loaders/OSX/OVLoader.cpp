// OVXLoader.cpp

#define OVDEBUG

#include <Carbon/Carbon.h>
#include <dlfcn.h>
#include <sys/syslimits.h>
#include "CIM.h"
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include "CIMConst.h"
#include "VXTextBar.h"
#include "VXBuffer.h"
#include "VXKeyCode.h"
#include "VXConfig.h"
#include "VXLoadableIM.h"
#include "OVAbout.h"

class CIMContext
{
public:
    CIMContext() : ovcontext(NULL), onScreen(0) { add(); }
	~CIMContext() { remove(); }
    VXTextBar bar;
    OVIMContext *ovcontext;
    int onScreen;
	
	void add();
	void remove();
};

int listloaded=0;

const int vxMaxContext=256;
CIMContext* pool[vxMaxContext];

void CIMContext::add()
	{ for (int c=0; c<vxMaxContext; c++) if (!pool[c]) { pool[c]=this; break; } }
void CIMContext::remove()
	{ for (int c=0; c<vxMaxContext; c++) if (pool[c]==this) pool[c]=NULL; }


int floatingwindowlock=0, defposx, defposy, textsize=24;
OVService srv;
VXConfig *sysconfig=NULL;
VXLibraryList list;
UInt32 usermenu='USRM';
const char *plistfile="/Library/OpenVanilla/Development/OVXLoader.plist";
const char *loaddir="/Library/OpenVanilla/Development/";
OVInputMethod *inputmethod;

OVDictionary *GetGlobalConfig()
{
	OVDictionary *d=sysconfig->getDictionaryRoot();
	
	if (!d->keyExist("OVLoader")) d->newDictionary("OVLoader");
	return d->getDictionary("OVLoader");
}

OVDictionary *GetLocalConfig(char *id)
{
	OVDictionary *d=sysconfig->getDictionaryRoot();

	char buf[256];
	strcpy (buf, "IM-");
	strcat (buf, id);

	if (!d->keyExist(buf)) d->newDictionary(buf);
	return d->getDictionary(buf);
}


int CIMCustomInitialize(MenuRef mnu)
{
	murmur ("OVLoader: initializing");
	
	for (int c=0; c<vxMaxContext; c++) pool[c]=NULL;
	
	sysconfig=new VXConfig(plistfile);
	OVDictionary *global=GetGlobalConfig();


	// load every .dylib; this is to prevent a strange "load twice" bug in iTerm
	if (!listloaded)
	{
		list.addglob(loaddir, ".dylib");
		listloaded=1;
	}
	
	
	char locale[256];
	VXGetCurrentLocale(CFBundleGetBundleWithIdentifier(CFSTR(cimBundleName)), locale);
	
	// for the time being, we use id as menu names
	// (and we initialize everything)
	int i=0;
	for (i=0; i<list.imcntr; i++)
	{
		OVEncoding enc;
		char imn[256];
		list.impair[i].im->name(locale, imn, &enc);
		CFStringRef imname=VXCreateCFString(imn, enc);
		InsertMenuItemTextWithCFString(mnu, imname, i, 0, usermenu+i);
		
		SetMenuItemCommandKey(mnu, i+1, FALSE, '1'+i);
		SetMenuItemModifiers(mnu, i+1, kMenuControlModifier);
		
		CFRelease(imname);

		char id[256];
		list.impair[i].im->identifier(id);
		OVDictionary *local=GetLocalConfig(id);
		list.impair[i].im->initialize(global, local, &srv, loaddir);
		delete local;
	}
	
	InsertMenuItemTextWithCFString(mnu, CFSTR("-"), i++, 0, 0);

	CFBundleRef bundle=CFBundleGetBundleWithIdentifier(CFSTR(cimBundleName));
    CFStringRef mstr;
	mstr=CFBundleCopyLocalizedString(bundle, CFSTR("Preferences"), NULL, NULL);
	InsertMenuItemTextWithCFString(mnu, mstr, i++, 0, 'PREF');
	CFRelease(mstr);
	mstr=CFBundleCopyLocalizedString(bundle, CFSTR("About"), NULL, NULL);
	InsertMenuItemTextWithCFString(mnu, mstr, i++, 0, 'ABUT');
	CFRelease(mstr);
	
	
	// get floating window settings
	if (!global->keyExist("floatingWindowLock")) 
	{
		global->setInt("floatingWindowLock", 0);
		global->setInt("floatingWindowLockPosX", 20);
		global->setInt("floatingWindowLockPosY", 760);
	}
	
	if (!global->keyExist("textSize")) global->setInt("textSize", 24);
	textsize=global->getInt("textSize");
	
	floatingwindowlock=global->getInt("floatingWindowLock");
	defposx=global->getInt("floatingWindowLockPosX");
	defposy=global->getInt("floatingWindowLockPosY");	
	
	// look for the input method we want to use
	char currentim[256];
	if (global->keyExist("currentIM"))
	{
		global->getString("currentIM", currentim);
	}
	
	inputmethod=list.find(currentim);
	if (!inputmethod) inputmethod=list.getFirst();
	
	if (inputmethod)
	{
		inputmethod->identifier(currentim);
		global->setString("currentIM", currentim);
		
		int pos=list.findPos(currentim);
		SetItemMark(mnu, pos+1, checkMark);
	}
	
	sysconfig->write();	
	delete global;
    return 1;
}

void CIMCustomTerminate()
{    
    fprintf (stderr, "OVXSimpleLoader: Terminating\n");
    if (inputmethod)
	{
/*		OVDictionary *global=GetGlobalConfig();
		OVDictionary *local=GetLocalConfig(imid);
		inputmethod->terminate(global, local, &srv); 
		sysconfig->write();
		delete global;
		delete local;		 */
	}
	
	delete sysconfig;
}

void *CIMCustomOpen()
{
    CIMContext *c=new CIMContext;

	c->bar.setFontSize(textsize);
	if (floatingwindowlock) 
	{
		c->bar.unlock();
		c->bar.setPosition(defposx, defposy);
		c->bar.lock(); 
	}

    if (inputmethod) c->ovcontext=inputmethod->newContext();
    return c;
}

int CIMCustomClose(void *data)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;
    if (inputmethod) inputmethod->deleteContext(c->ovcontext);
	delete c;
    return 1;
}

int CIMCustomActivate(void *data, CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;

	if (sysconfig->changed())
	{
		sysconfig->read();
	
		char buf[256];
		inputmethod->identifier(buf);
		OVDictionary *global=GetGlobalConfig();
	
		c->bar.setFontSize(global->getInt("textSize"));
		
		floatingwindowlock=global->getInt("floatingWindowLock");
		defposx=global->getInt("floatingWindowLockPosX");
		defposy=global->getInt("floatingWindowLockPosY");

		if (floatingwindowlock) 
		{
			c->bar.unlock();
			c->bar.setPosition(defposx, defposy);
			c->bar.lock(); 
		}
		else c->bar.unlock();
		
		OVDictionary *local=GetLocalConfig(buf);
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

	if (floatingwindowlock) 
	{
		c->bar.clear();
		c->bar.show();
		c->bar.lock();
		c->bar.append("(選字窗)");
		c->bar.update();
	}
	
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
		if (floatingwindowlock)
		{
			c->bar.unlock();
			c->bar.hide();		
		}
    }

	if (floatingwindowlock)
	{
		int newx, newy;
		c->bar.getPosition(&newx, &newy);
		if ((newx != defposx) || (newy != defposy))
		{
			murmur("old: (%d,%d), new: (%d,%d)", defposx, defposy, newx, newy);
			OVDictionary *global=GetGlobalConfig();
			defposx=newx;
			defposy=newy;
			global->setInt("floatingWindowLock", floatingwindowlock);
			global->setInt("floatingWindowLockPosX", defposx);
			global->setInt("floatingWindowLockPosY", defposy);
			sysconfig->write();
		}
	}


	c->ovcontext->deactivate(&srv);

    return 1;
}

int CIMCustomHandleInput(void *data, CIMInputBuffer *buf, unsigned char charcode,
		UInt32 keycode, UInt32 modifiers, Point *pnt)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;

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
			
		case 'ABUT':
			ShowAbout();
			return 1;
	}
    
    if (command >= usermenu && command <= usermenu+list.imcntr)
    {
    	int newpos=command-usermenu;
    	
    	char buf[256];
    	inputmethod->identifier(buf);
    	int oldpos=list.findPos(buf);
    	SetItemMark(mnu, oldpos+1, 0);
    	SetItemMark(mnu, newpos+1, checkMark);
    	
		OVInputMethod *newim=list.impair[newpos].im;
    	newim->identifier(buf);
    	murmur ("user wants to switch IM, newimpos=%d, new im id=%s", newpos, buf);
    	
		// kill all existing context
		for (int i=0; i<vxMaxContext; i++)
		{
			if (pool[i])
			{
				inputmethod->deleteContext(pool[i]->ovcontext);
				pool[i]->ovcontext=newim->newContext();
			}
		}
		
		inputmethod=newim;
		
    	OVDictionary *global=GetGlobalConfig();
    	global->setString("currentIM", buf);
		delete global;
		sysconfig->write();
    }
    
    
    return 0;
}
