// OVXLoader.cpp

#define OVDEBUG

#include <Carbon/Carbon.h>
#include <dlfcn.h>
#include <sys/syslimits.h>
#include "CIM.h"
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <OpenVanilla/OVUtility.h>
#include "CIMConst.h"
#include "VXTextBar.h"
#include "VXBuffer.h"
#include "VXKeyCode.h"
#include "VXConfig.h"
#include "VXLoadableIM.h"
#include "VXUtility.h"
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

const int vxMaxContext = 256;
const char *plistfile  = "/Library/OpenVanilla/Development/OVXLoader.plist";
const char *loaddir    = "/Library/OpenVanilla/Development/";

int floatingwindowlock=0, defposx, defposy, textsize=24;
int listloaded=0;
UInt32 usermenu='USRM';

VXConfig *sysconfig=NULL;
VXLibraryList list;

OVService srv;
OVInputMethod *inputmethod;

CIMContext* pool[vxMaxContext];

void CIMContext::add() {
    for (int c=0; c<vxMaxContext; c++) if (!pool[c]) { pool[c]=this; break; }
}

void CIMContext::remove() {
    for (int c=0; c<vxMaxContext; c++) if (pool[c]==this) pool[c]=NULL;
}

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

    strcpy (buf, "IM-"); strcat (buf, id);

    if (!d->keyExist(buf)) d->newDictionary(buf);
    return d->getDictionary(buf);
}

void LoadEveryDylib() {
    if (!listloaded) {
        list.addglob((char*)loaddir,(char*)".dylib");
        listloaded=1;
    }
}

void SetupMenuString(MenuRef mnu, int pos) {
    VXCFAutoreleasePool p;
    CFBundleRef bdl = CFBundleGetBundleWithIdentifier(CFSTR(cimBundleName));
    CFStringRef str = (CFStringRef)p.add
        (CFBundleCopyLocalizedString(bdl,CFSTR("Preferences"),NULL,NULL));
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'PREF');
    str = (CFStringRef)p.add
        (CFBundleCopyLocalizedString(bdl,CFSTR("About"),NULL,NULL));
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'ABUT');
}

int SetupMenuList(MenuRef mnu) {
    char locale[256];
    VXGetCurrentLocale(CFBundleGetBundleWithIdentifier(CFSTR(cimBundleName)),
                       locale);
    // for the time being, we use id as menu names
    // (and we initialize everything)
    int i;
    for (i=0; i<list.imcntr; i++) {
        OVEncoding enc;
        char imn[256];
        list.impair[i].im->name(locale, imn, &enc);
        CFStringRef imname=VXCreateCFString(imn, enc);
        InsertMenuItemTextWithCFString(mnu, imname, i, 0, usermenu+i);
        SetMenuItemCommandKey(mnu, i+1, FALSE, '1'+i);
        SetMenuItemModifiers(mnu, i+1, kMenuControlModifier);
        CFRelease(imname);
    }

    return i;
}

void ClearContextPool() {
    for (int c=0; c<vxMaxContext; c++) pool[c]=NULL;
}

void SetupGlobalConfig(OVDictionary *global) {
    if (!global->keyExist("floatingWindowLock")) {
        global->setInt("floatingWindowLock", 0);
        global->setInt("floatingWindowLockPosX", 20);
        global->setInt("floatingWindowLockPosY", 760);
    }
    global->keyExist("textSize") || global->setInt("textSize", 24);
    floatingwindowlock = global->getInt("floatingWindowLock");
    textsize = global->getInt("textSize");
    defposx  = global->getInt("floatingWindowLockPosX");
    defposy  = global->getInt("floatingWindowLockPosY");	
}

void SwitchToCurrentInputMethod(MenuRef mnu,OVDictionary *global) {
    OVAutoDeletePool p;
    char currentim[256];
    if (global->keyExist("currentIM"))
        global->getString("currentIM", currentim);

    int pos=list.findPos(currentim);
    if (pos==-1) pos=0;
    inputmethod=list.impair[pos].im;
    if (inputmethod) {
        OVDictionary *local= (OVDictionary*)p.add(GetLocalConfig(currentim));
        inputmethod->initialize(global, local, &srv, (char*)loaddir);
        list.impair[pos].inited=1;
        inputmethod->identifier(currentim);
        global->setString("currentIM", currentim);
        int pos=list.findPos(currentim);
        SetItemMark(mnu, pos+1, checkMark);
    }
}

int CIMCustomInitialize(MenuRef mnu)
{
    murmur ("OVLoader: initializing");
    OVAutoDeletePool p;
    sysconfig=new VXConfig(plistfile);

    ClearContextPool();
    LoadEveryDylib();
    int pos = SetupMenuList(mnu);
    InsertMenuItemTextWithCFString(mnu, CFSTR("-"), pos++, 0, 0);
    SetupMenuString(mnu,pos);
    OVDictionary *global=(OVDictionary*)p.add(GetGlobalConfig());
    SetupGlobalConfig(global);
    SwitchToCurrentInputMethod(mnu, global);
    sysconfig->write();	
    return 1;
}

void CIMCustomTerminate()
{    
    murmur("OVXSimpleLoader: Terminating");
    if (inputmethod) {
        // XXX: Not implemented: Free each element in XVLibraryList List
    }
    
    delete sysconfig;
}

void *CIMCustomOpen()
{
    CIMContext *c=new CIMContext;

    c->bar.setFontSize(textsize);
    if (floatingwindowlock) {
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

int RefreshConfig() {
    OVAutoDeletePool p;
    if (sysconfig->changed()) {
        sysconfig->read();
	
        char buf[256];
        inputmethod->identifier(buf);

        OVDictionary *global=(OVDictionary*)p.add(GetGlobalConfig());
        OVDictionary *local =(OVDictionary*)p.add(GetLocalConfig(buf));

        floatingwindowlock=global->getInt("floatingWindowLock");
        defposx=global->getInt("floatingWindowLockPosX");
        defposy=global->getInt("floatingWindowLockPosY");
        
        inputmethod->update(global, local);

        return 1;
    }
    return 0;
}

void RefreshDisplay(CIMContext *c,CIMInputBuffer *buf) {
    OVDictionary *global=GetGlobalConfig();
    c->bar.setFontSize(global->getInt("textSize"));
    delete global;
    if (c->onScreen) {
        c->onScreen=0;
        if (buf->length()) {
            c->bar.show();
            c->onScreen=0;
        } else {
            c->bar.hide()->clear();
            c->ovcontext->clear();    
        }
    }    

    if (floatingwindowlock) {
        c->bar.clear();
        c->bar.show();
        c->bar.lock();
        c->bar.append((char*)"(選字窗)");
        c->bar.update();
    }
}

int CIMCustomActivate(void *data, CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*)data;    
    if(!c->ovcontext) return 0;
    if(RefreshConfig()) {
        c->onScreen = 0;
        c->ovcontext->clear();
    }

    if (floatingwindowlock) {
        c->bar.unlock();
        c->bar.setPosition(defposx, defposy);
        c->bar.lock(); 
    } else {
        c->bar.unlock();
    }

    RefreshDisplay(c,buf);
    c->ovcontext->activate(&srv);
    return 1;
}


void SetFloatingWindowPosition(CIMContext *c) {
    if (floatingwindowlock) {
        int newx, newy;
        c->bar.getPosition(&newx, &newy);
        if ((newx != defposx) || (newy != defposy)) {
            murmur("old: (%d,%d), new: (%d,%d)", defposx, defposy, newx, newy);
            OVDictionary *global=GetGlobalConfig();
            defposx=newx;
            defposy=newy;
            global->setInt("floatingWindowLock", floatingwindowlock);
            global->setInt("floatingWindowLockPosX", defposx);
            global->setInt("floatingWindowLockPosY", defposy);
            sysconfig->write();
            delete global;
        }
    }
}

int CIMCustomDeactivate(void *data, CIMInputBuffer *buf)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;

    if (c->bar.onScreen()) {
        c->onScreen=1;
        if (floatingwindowlock) c->bar.unlock();
        c->bar.hide();		
    }
    SetFloatingWindowPosition(c);

    c->ovcontext->deactivate(&srv);
    return 1;
}

int CIMCustomHandleInput(void *data, CIMInputBuffer *buf,
                         unsigned char charcode,
                         UInt32 keycode, UInt32 modifiers, Point *pnt)
{
    CIMContext *c=(CIMContext*)data;    
    if (!c->ovcontext) return 0;

    VXBuffer vxb;
    VXKeyCode key;

    vxb.bind(buf);
    key.set(charcode, keycode, modifiers);

    murmur("received keycode=%d, buffer position=(%d,%d)\n",
           charcode, pnt->h, pnt->v);
        
    c->bar.setPosition(pnt->h, pnt->v);
    return c->ovcontext->keyEvent(&key, &vxb, &c->bar, &srv);
}

OVInputMethod* InitInputMethodAtMenuPos(int pos) {
    OVAutoDeletePool p;
    OVInputMethod *im=list.impair[pos].im;
    if (!list.impair[pos].inited) {
        char imid[256];
        im->identifier(imid);
        OVDictionary *global= (OVDictionary*)p.add(GetGlobalConfig());
        OVDictionary *local = (OVDictionary*)p.add(GetLocalConfig(imid));
        im->initialize(global, local, &srv, (char*)loaddir);
        list.impair[pos].inited=1;
    }
    return im;
}

void SetCurrentInputMethod(char *imid) {
    OVDictionary *global=GetGlobalConfig();
    global->setString("currentIM", imid);
    delete global;
}

void KillAllExistingContext(OVInputMethod *newim) {
    for (int i=0; i<vxMaxContext; i++) {
        if (pool[i]) {
            inputmethod->deleteContext(pool[i]->ovcontext);
            pool[i]->ovcontext=newim->newContext();
        }
    }
}

void SwitchMenuItemMark(MenuRef mnu,int oldp, int newp) 
{
    SetItemMark(mnu, oldp, 0);
    SetItemMark(mnu, newp, checkMark);
}

void StartupPreferenceEditor() 
{
    char sbuf[512];
    murmur("launching application to edit %s", plistfile);
    sprintf (sbuf, "open %s", plistfile);
    system(sbuf);   
}


int CIMCustomMenuHandler(void *data, UInt32 command, MenuRef mnu, 
                         CIMInputBuffer *buf)
{
    switch (command) {
    case 'PREF':
        StartupPreferenceEditor();
        return 1;
    case 'ABUT':
        ShowAbout();
        return 1;
    }

    if (command >= usermenu && command <= usermenu+list.imcntr) {
    	char imid[256];
    	inputmethod->identifier(imid);
        SwitchMenuItemMark(mnu,list.findPos(imid)+1,command-usermenu+1);
        OVInputMethod *newim = InitInputMethodAtMenuPos(command-usermenu);
    	newim->identifier(imid);
    	murmur ("user wants to switch IM, newimpos=%d, new im id=%s",
                command-usermenu, buf);
        KillAllExistingContext(newim);
        inputmethod=newim;
        SetCurrentInputMethod(imid);
        sysconfig->write();
    }
    return 0;
}
