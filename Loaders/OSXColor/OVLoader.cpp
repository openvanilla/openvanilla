// OVLoader.cpp

#define OVDEBUG	
#include <Carbon/Carbon.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
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
#include "VXFullWidth.h"
#include "VXService.h"
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
const char *defaultplistfile  = "/Library/OpenVanilla/0.6.2/OVLoader.plist";
char userplistfile[PATH_MAX];
const char *loaddir    = "/Library/OpenVanilla/0.6.2/";

int floatingwindowlock=0, defposx, defposy, textsize=20, opacity=80, conversionfilter=0, fullwidthfilter=0;
int fore[3] = {0,0,0};
int back[3] = {255 * 256 , 255 * 256, 255 * 256};
int listloaded=0;
int menuPosFullWidthStart=0, menuPosHanConvertStart=0;
UInt32 usermenu='USRM';

VXConfig *sysconfig=NULL;
VXLibraryList list;

VXService srv;
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
	VXCFAUTORELEASE;
    CFBundleRef bdl = CFBundleGetBundleWithIdentifier(CFSTR(cimBundleName));
	CFStringRef str;
	
    InsertMenuItemTextWithCFString(mnu, CFSTR("-"), pos++, 0, 0);
	menuPosFullWidthStart=pos+1;
		
    str = (CFStringRef)VXSafe
        (CFBundleCopyLocalizedString(bdl,CFSTR("Half-width ASCII characters"),NULL,NULL));	
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'FHWC');
    str = (CFStringRef)VXSafe
        (CFBundleCopyLocalizedString(bdl,CFSTR("Full-width ASCII characters"),NULL,NULL));	
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'FFWC');

    InsertMenuItemTextWithCFString(mnu, CFSTR("-"), pos++, 0, 0);
	menuPosHanConvertStart=pos+1;
	
    str = (CFStringRef)VXSafe
        (CFBundleCopyLocalizedString(bdl,CFSTR("Normal Output"),NULL,NULL));	
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'FHCN');
    str = (CFStringRef)VXSafe
        (CFBundleCopyLocalizedString(bdl,CFSTR("Traditional to Simplified Chinese"),NULL,NULL));	
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'FHCS');
    str = (CFStringRef)VXSafe
        (CFBundleCopyLocalizedString(bdl,CFSTR("Simplified to Traditional Chinese"),NULL,NULL));	
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'FHCT');


    InsertMenuItemTextWithCFString(mnu, CFSTR("-"), pos++, 0, 0);
    str = (CFStringRef)VXSafe
        (CFBundleCopyLocalizedString(bdl,CFSTR("Preferences"),NULL,NULL));	
    InsertMenuItemTextWithCFString(mnu, str, pos++, 0, 'PREF');
    str = (CFStringRef)VXSafe
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
		if (!imname) imname=VXCreateCFString(list.impair[i].id);		
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
    floatingwindowlock = global->getIntDefault("floatingWindowLock", 0);
    textsize = global->getIntDefault("textSize", 20);
    opacity = global->getIntDefault("opacity", 80);
    fore[0] = 0;
    fore[1] = 0;
    fore[2] = 0;
    back[0] = 255 * 256;
    back[1] = 255 * 256;
    back[2] = 255 * 256;
    defposx  = global->getIntDefault("floatingWindowLockPosX", 20);
    defposy  = global->getIntDefault("floatingWindowLockPosY", 760);	
    conversionfilter=global->getIntDefault("useHanConversionFilter", 0);
    fullwidthfilter=global->getIntDefault("fullWidthCharFilter", 0);
}

void SwitchToCurrentInputMethod(MenuRef mnu,OVDictionary *global) {
    OVAUTODELETE;
    char currentim[256];
	int pos=-1;    
	
    if (global->keyExist("currentIM"))
    {
        global->getString("currentIM", currentim);
        pos=list.findPos(currentim);
    }
       
    if (pos==-1) pos=0;
    inputmethod=list.impair[pos].im;
    if (inputmethod) {
        inputmethod->identifier(currentim);
        OVDictionary *local= (OVDictionary*)OVSafe(GetLocalConfig(currentim));
        inputmethod->initialize(global, local, &srv, (char*)loaddir);
        list.impair[pos].inited=1;
        global->setString("currentIM", currentim);
        pos=list.findPos(currentim);
        SetItemMark(mnu, pos+1, checkMark);
    }
}

void CreateUserConfig()
{	
	// first we determine if ~/Library/OpenVanilla/0.6.2/ already exists
	
	char *ptr;
    static char userpref[PATH_MAX];
    
    if (!(ptr=getenv("HOME")))
    {
        struct passwd *pw = getpwuid(getuid());
        if (pw == NULL) strcpy(userpref, "/tmp");
        else strcpy(userpref, pw->pw_dir);
    }
    else strcpy(userpref, ptr);

    strcat(userpref, "/Library/OpenVanilla") ;
    mkdir(userpref, S_IRWXU) ;
    strcat(userpref, "/0.6.2");
    mkdir(userpref, S_IRWXU) ;

    strcat(userpref, "/OVLoader.plist");
    sysconfig=new VXConfig(userpref);
	
	struct stat filestat;
	
	// if ~/Lib/OV/0.6.2/OVLoader.plist does not exist, copy from default
	if (stat(userpref, &filestat))         // any error
	{
		VXConfig def(defaultplistfile);
		def.read();
		VXDictionary *userdict=new VXDictionary(def.getDictionaryRoot()->getDictRef(), 1);
		sysconfig->changeDictionary(userdict);
		sysconfig->write();
	}

	strcpy(userplistfile, userpref);
	murmur ("OVLoader: user preference file at %s", userplistfile);
}


int CIMCustomInitialize(MenuRef mnu)
{
    murmur ("OVLoader: initializing");
    OVAUTODELETE;
	
    CreateUserConfig();
    ClearContextPool();
    LoadEveryDylib();
    int pos = SetupMenuList(mnu);
    SetupMenuString(mnu,pos);
    OVDictionary *global=(OVDictionary*)OVSafe(GetGlobalConfig());
    SetupGlobalConfig(global);
    SwitchToCurrentInputMethod(mnu, global);
	
    SetItemMark(mnu, menuPosFullWidthStart+fullwidthfilter, checkMark);
    SetItemMark(mnu, menuPosHanConvertStart+conversionfilter, checkMark);
	
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

    c->bar.setFontSize(textsize, opacity, fore, back);
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
    OVAUTODELETE;
    if (sysconfig->changed()) {
        sysconfig->read();
	
        char buf[256];
        inputmethod->identifier(buf);

        OVDictionary *global=(OVDictionary*)OVSafe(GetGlobalConfig());
        OVDictionary *local =(OVDictionary*)OVSafe(GetLocalConfig(buf));

        floatingwindowlock=global->getInt("floatingWindowLock");
        defposx=global->getInt("floatingWindowLockPosX");
        defposy=global->getInt("floatingWindowLockPosY");
		conversionfilter=global->getInt("useHanConversionFilter");
		fullwidthfilter=global->getInt("fullWidthCharFilter");
        
        inputmethod->update(global, local);

        return 1;
    }
    return 0;
}

void RefreshDisplay(CIMContext *c,CIMInputBuffer *buf) {
    OVDictionary *global=GetGlobalConfig();
    int forec[3], backc[3];
    char buffer[80];
    {
    global->getString("forer", buffer);
    forec[0] = (unsigned short)(atof(buffer) * 255 * 256);
    global->getString("foreg", buffer);
    forec[1] = (unsigned short)(atof(buffer) * 255 * 256);
    global->getString("foreb", buffer);
    forec[2] = (unsigned short)(atof(buffer) * 255 * 256);
    global->getString("backr", buffer);
    backc[0] = (unsigned short)(atof(buffer) * 255 * 256);
    if(!backc[0]) backc[0] = 255 * 256;
    global->getString("backg", buffer);
    backc[1] = (unsigned short)(atof(buffer) * 255 * 256);
    if(!backc[1]) backc[1] = 255 * 256;
    global->getString("backb", buffer);
    backc[2] = (unsigned short)(atof(buffer) * 255 * 256);
    if(!backc[2]) backc[2] = 255 * 256;
    }

    c->bar.setFontSize(global->getInt("textSize"),
		    global->getInt("opacity"), forec, backc);
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
    c->ovcontext->clear();
    
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

	vxb.setConversionFilter(conversionfilter);
	vxb.setFullWidthFilter(fullwidthfilter);
    vxb.bind(buf);
    key.set(charcode, keycode, modifiers);

    murmur("received keycode=%d, buffer position=(%d,%d)",
           charcode, pnt->h, pnt->v);
        
    // textbar pos.y down 5 pt from cursur pos (pnt->v+5) to prevent
    // masking the "composing underline area"
//    c->otsbar.setPosition(pnt->h, pnt->v+5);
    c->bar.setPosition(pnt->h, pnt->v+5);     
    
    if (c->ovcontext->keyEvent(&key, &vxb, &c->bar, &srv)) return 1;
	 
	// full-width post filter
	if (key.isCommand() || key.isOpt() || key.isCtrl()) return 0;
	if (!key.isPrintable()) return 0;
	if (!fullwidthfilter) return 0;
	
	unsigned short fullwidthc;
	if ((fullwidthc=VXHalfToFullWidthChar(key.code()))==key.code()) return 0;
	vxb.append(&fullwidthc, ovEncodingUTF16Auto, 1)->send();
	return 1;
}

OVInputMethod* InitInputMethodAtMenuPos(int pos) {
    OVAUTODELETE;
    OVInputMethod *im=list.impair[pos].im;
    if (!list.impair[pos].inited) {
        char imid[256];
        im->identifier(imid);
        OVDictionary *global= (OVDictionary*)OVSafe(GetGlobalConfig());
        OVDictionary *local = (OVDictionary*)OVSafe(GetLocalConfig(imid));
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
    murmur("launching application to edit %s", userplistfile);
//    sprintf (sbuf, "open %s", userplistfile);
//    system(sbuf);   
    system("open /Library/PreferencePanes/OVPrefPane.prefPane");
}

void UpdateFilterConfig() {
    OVDictionary *global=GetGlobalConfig();
	global->setInt("useHanConversionFilter", conversionfilter);
	global->setInt("fullWidthCharFilter", fullwidthfilter);
    delete global;
	sysconfig->write();
}

int CIMCustomMenuHandler(void *data, UInt32 command, MenuRef mnu, 
                         CIMInputBuffer *buf)
{
    switch (command) {
	case 'FHWC':
		fullwidthfilter=0;
		SwitchMenuItemMark(mnu, menuPosFullWidthStart+1, menuPosFullWidthStart);
		UpdateFilterConfig();
		return 1;
	case 'FFWC':
		fullwidthfilter=1;
		SwitchMenuItemMark(mnu, menuPosFullWidthStart, menuPosFullWidthStart+1);
		UpdateFilterConfig();
		return 1;
	case 'FHCN':
		conversionfilter=0;
		SwitchMenuItemMark(mnu, menuPosHanConvertStart+1, menuPosHanConvertStart);		
		SwitchMenuItemMark(mnu, menuPosHanConvertStart+2, menuPosHanConvertStart);
		UpdateFilterConfig();
		return 1;
	case 'FHCS':
		conversionfilter=1;
		SwitchMenuItemMark(mnu, menuPosHanConvertStart, menuPosHanConvertStart+1);		
		SwitchMenuItemMark(mnu, menuPosHanConvertStart+2, menuPosHanConvertStart+1);
		UpdateFilterConfig();
		return 1;
	case 'FHCT':
		conversionfilter=2;
		SwitchMenuItemMark(mnu, menuPosHanConvertStart, menuPosHanConvertStart+2);		
		SwitchMenuItemMark(mnu, menuPosHanConvertStart+1, menuPosHanConvertStart+2);
		UpdateFilterConfig();
		return 1;		
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
