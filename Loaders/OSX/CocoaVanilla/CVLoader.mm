// CVLoader.mm: The CocoaVanilla Loader (OVLoader for OS X)
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// #define OV_DEBUG
#include <ctype.h>
#include <unistd.h>
#include <sys/time.h>
#include "CVLoader.h"
#include "TSBundle.h"
#include <OpenVanilla/OVUtility.h>
#include "CVLoaderUtility.h"
#include "CVLoaderConfig.h"
#include "CVDictionary.h"
#include "CVKeyCode.h"
#include "NSDictionaryExtension.h"
#include "OVDisplayServer.h"

// A experiment to make CocoaVanilla safer
class CVAutorelease {
public:
    CVAutorelease() {
        // NSLog(@"CVAutorelease()");
        ap=[NSAutoreleasePool new];
    }
    ~CVAutorelease() {
        // NSLog(@"~CVAutorelease()");
        [ap release];
    }
    
protected:
    NSAutoreleasePool *ap;
};

#define CVSAFETY CVAutorelease _cvar;

#define CV_INTERNAL_VERSION @"0.7.3 beta (r1904)"

enum {      // CVLMI = CVLoader Menu Item
    CVLMI_KPGROUPSTART=1000,
    CVLMI_IMGROUPSTART=2000,
    CVLMI_OFGROUPSTART=3000,
	CVLMI_FASTIMSWITCH=4000,
    CVLMI_ABOUT=4001,
    CVLMI_PREFERENCES=4002,
    CVLMI_HELP=4003,
};

CVLoader::CVLoader() {
    // actual initialization code in CVLoadver::init()
}

CVLoader::~CVLoader() {
    // although this is a VERY BAD practice, but we DO NOT DELETE anything here
    // the reason: text component is never released once it's loaded,
    // everything goes away as the application is closed by OS X
}

int CVLoader::init(MenuRef m) {
    CVSAFETY;
    NSLog(@"An instance of CocoaVanilla version %@ created", CV_INTERNAL_VERSION);

    // UInt32 beginTime;
    // beginTime = TickCount();
    // murmur("CVLoader::init begin");
	// get CVLoader.bundle
    loaderbundle=[NSBundle bundleWithIdentifier:@ TSBUNDLEID];
    if (!loaderbundle) {
        NSLog (@"CVLoader: fatal error, bundle %s not found", TSBUNDLEID);
        return 0;
    }

	// connect to the display server
	if (!connectDisplayServer()) {
		NSLog ([NSString stringWithFormat: @"CVLoader: init failed, cannot start up OVDisplayServer at %@", CVLC_DISPLAYSERVER]);
		return 0;
	}
	
    // gets factory (default) settings
    CVConfig *factorydef=[[CVConfig alloc] initWithFile:CVLC_FACTORY_DEFAULTS defaultData:nil];
    [factorydef autorelease];
    // NSLog([[factorydef dictionary] description]);
    
	// member variables initialization
    activecontext=NULL;
	immenu=m;
    srv=new CVService(CVGetUserSpacePath(), dspsrvr);
    candi=new CVCandidate(dspsrvr);
    cfg=[[CVConfig alloc] initWithFile:CVGetUserConfigFilename() defaultData:[factorydef dictionary]];
    modarray=[NSMutableArray new];
    imarray=[NSMutableArray new];
    ofarray=[NSMutableArray new];

    // load configuration
    loaderdict=[[cfg dictionary] valueForKey:@"OVLoader" default:[[NSMutableDictionary new] autorelease]];
    
    NSString *timeout=[loaderdict valueForKey:@"atomicInitLockTimeout" default:@"500"];
    // check if our last atomic init failed
    if (checkIfLastAtomicInitFailed([timeout intValue])) return 0;
    
    // get library-exclude list and module-exclude list
    NSArray *libexclude=[loaderdict valueForKey:@"excludeLibraryList" default:[[NSArray new] autorelease]];
    NSArray *modexclude=[loaderdict valueForKey:@"excludeModuleList" default:[[NSArray new] autorelease]];
    NSMutableDictionary *loadhistory=[[NSMutableDictionary new] autorelease];
    
    // load everything!
    [modarray addObjectsFromArray: CVLoadEverything(CVGetModuleLoadPath(), srv, libexclude, modexclude, loadhistory, CVGetAtomicInitLockFilename())];
    // NSLog([loadhistory description]);
	
    // create menu groups and check all menu items, then sync config
	menudict=nil;
	kpmenugroup=immenugroup=ofmenugroup=NULL;
	refreshMenu();
    syncMenuAndConfig();
    // murmur("CVLoader::init ticks: %d", TickCount() - beginTime );
    
    return 1;
}

void CVLoader::refreshMenu() {
	NSDictionary *cfgmenukey=[[cfg dictionary] valueForKey:@"OVMenuManager" default:[[NSMutableDictionary new] autorelease]];
	if (!menudict) {
		menudict=[NSMutableDictionary new];
		[menudict addEntriesFromDictionary:cfgmenukey];
		createMenuGroups();
	}
	else {
		if (![menudict isEqualToDictionary:cfgmenukey]) {
			// NSLog(@"OVMenuManager config node changed, rebuilding the menu");
			[menudict removeAllObjects];
			[menudict addEntriesFromDictionary:cfgmenukey];
			createMenuGroups();
		}
	}
	
	checkMenuItems();
}

CVContext *CVLoader::newContext() {
    CVSAFETY;
    return new CVContext(this);
}

void CVLoader::setActiveContext(CVContext *c) {
    activecontext=c;
    if (!c) return;
    
	// check if the config has been changed and thus need reload!
    if (![cfg needSync]) return;
    [cfg sync];
    
    // we have to reload loaderdict as it might have been gone!
    loaderdict=[[cfg dictionary] valueForKey:@"OVLoader" default:[[NSMutableDictionary new] autorelease]];
    
    // now we tell every usable module to update config
    NSMutableDictionary *cfgdict=[cfg dictionary];
    NSEnumerator *e=[modarray objectEnumerator];
    CVModuleWrapper *m;
    while (m=[e nextObject]) {
        if (![m usable]) continue;
        NSString *mid=[m identifier];
        NSMutableDictionary *mdict=[cfgdict valueForKey:mid default:[[NSMutableDictionary new] autorelease]];
        CVDictionary cvd(mdict);
        OVModule *om=[m module];
        om->update(&cvd, srv);
    }
    
	// now we refresh our menu
	refreshMenu();
    syncMenuAndConfig();
}

void CVLoader::menuHandler(unsigned int cmd) {
    CVSAFETY;
    if (immenugroup->clickItem(cmd)) {
		NSString *pIM=[NSString stringWithString: [loaderdict valueForKey:@"primaryInputMethod" default:@""]];
        syncMenuAndConfig();
        if (activecontext) {
            activecontext->syncConfig();
        }
		
		// overwrite lastPrimaryInputMethod key
        NSString *npIM=[NSString stringWithString: [loaderdict valueForKey:@"primaryInputMethod" default:@""]];
        if (![npIM isEqualToString:pIM])
            [loaderdict setValue:pIM forKey:@"lastPrimaryInputMethod"];		
        return;
    }

    // if the item falls in key preprocessor group
    if (kpmenugroup->clickItem(cmd)) {
        syncMenuAndConfig();
		if (activecontext) {
            activecontext->syncConfig();
			CVSmartMenuItem *i=kpmenugroup->getMenuItem(cmd);
            // we re-use the function to display the enable/disable status
			showOutputFilterStatus([i idTag], [i checked]);
		}
        return;        
    }

    // if the item falls in output filter group
    if (ofmenugroup->clickItem(cmd)) {
        syncMenuAndConfig();
		if (activecontext) {
			CVSmartMenuItem *i=ofmenugroup->getMenuItem(cmd);
			showOutputFilterStatus([i idTag], [i checked]);
		}
        return;
    }

    switch (cmd) {
		case CVLMI_FASTIMSWITCH:
			switchToLastPrimaryIM();
			return;
        case CVLMI_ABOUT:
            // the effect wasn't very satisfactory; we should launch an About application for this
            [dspsrvr aboutDialog];
            return;
        case CVLMI_PREFERENCES:
            system([[NSString stringWithFormat:@"open %@", CVLC_PREFERENCEUTIL] UTF8String]);
            return;
        case CVLMI_HELP:
			if (!strcasecmp(srv->locale(), "zh_TW")) {
				system(CVLC_DOCUMENT_ZH_TW);
			}
			else if (!strcasecmp(srv->locale(), "zh_CN")) {
				system(CVLC_DOCUMENT_ZH_CN);
			}
			else {
				system(CVLC_DOCUMENT_EN);
			}
            return;
    }
    
    murmur("unknown menu item %d clicked", cmd);
}

id CVLoader::connectDisplayServer() {
	dspsrvr=nil;
	
	dspsrvr=[[NSConnection rootProxyForConnectionWithRegisteredName:OVDSPSRVR_NAME host:nil] retain];
	if (!dspsrvr) {
		system([[NSString stringWithFormat:@"open %@", CVLC_DISPLAYSERVER] UTF8String]);
		
		// a total timeout of 1 sec
		for (int retry=0; retry<20; retry++) {
			NSLog([NSString stringWithFormat:@"connecting to OVDisplayServer, retry %d", retry]);
			usleep(50000);		// wait for the server to be brought up (0.05 sec)
			dspsrvr=[[NSConnection rootProxyForConnectionWithRegisteredName:OVDSPSRVR_NAME host:nil] retain];
			if (dspsrvr) break;
		}
	}
	
	if (dspsrvr) [dspsrvr setProtocolForProxy:@protocol(OVDisplayServer)];
	return dspsrvr;
}

void CVLoader::createMenuGroups() {
	NSDictionary *cfgmenukey=[[cfg dictionary] valueForKey:@"OVMenuManager" default:[[NSMutableDictionary new] autorelease]];

    if (kpmenugroup) delete kpmenugroup;
	if (immenugroup) delete immenugroup;
	if (ofmenugroup) delete ofmenugroup;
	CVDeleteMenu(immenu);
	kpmenugroup=new CVSmartMenuGroup(immenu, CVLMI_KPGROUPSTART, loaderbundle, CVSM_MULTIPLE);
    immenugroup=new CVSmartMenuGroup(immenu, CVLMI_IMGROUPSTART, loaderbundle, CVSM_EXCLUSIVE);
    ofmenugroup=new CVSmartMenuGroup(immenu, CVLMI_OFGROUPSTART, loaderbundle, CVSM_MULTIPLE);
    
    NSArray *kpa=CVGetModulesByType(modarray, @"OVKeyPreprocessor");
    if ([kpa count]) {
        kpmenugroup->insertTitle(MSG(@"Key Preprocessors"));
        
        // get the "sorted-by" key to make KP submenu ordered by user settings
        NSArray *kporderedby=[cfgmenukey valueForKey:@"keyPreprocessorOrder" default:[[NSArray new] autorelease]];
        
        pourModuleArrayIntoMenu(kpa, kpmenugroup, kporderedby);
        kpmenugroup->insertSeparator();
    }
    
    NSArray *ima=CVGetModulesByType(modarray, @"OVInputMethod");
    
    if ([ima count]) {
        immenugroup->insertTitle(MSG(@"Input Methods"));
    }
    else {
        immenugroup->insertTitle(MSG(@"No Input Method Available"));
    }
    
    pourModuleArrayIntoMenu(ima, immenugroup);
    immenugroup->insertSeparator();

    // we get the "sorted-by" key to make our menu ordered by user's settings
    // we read our own cfgmenukey because we want to force sync the config file
    // if the key hasn't existed yet
    NSArray *oforderedby=[cfgmenukey valueForKey:@"outputFilterOrder" default:[[NSArray new] autorelease]];
    
    NSArray *ofa=CVGetModulesByType(modarray, @"OVOutputFilter");
    if ([ofa count]) {
        ofmenugroup->insertTitle(MSG(@"Output Filters"));
    }

    pourModuleArrayIntoMenu(ofa, ofmenugroup, oforderedby);
    if ([ofa count]) ofmenugroup->insertSeparator();
	
	// the fastIMSwitch has a menudict key called "fastIMSwitch"
	
	if ([ima count]) {
        CVInsertMenuItem(immenu, CVLMI_FASTIMSWITCH, MSG(@"Input method toggle"), 0, [menudict valueForKey:@"fastIMSwitch"]);
    }

    CVInsertMenuItem(immenu, CVLMI_PREFERENCES, MSG(@"Preferences"), 0);
    CVInsertMenuItem(immenu, CVLMI_HELP, MSG(@"Help"), 0);
	CVInsertMenuItem(immenu, CVLMI_ABOUT, MSG(@"About"), 0);
}

void CVLoader::checkMenuItems() {
    // uncheck everything
    kpmenugroup->uncheckAll();
    immenugroup->uncheckAll();
    ofmenugroup->uncheckAll();
    
    // check keyPreprocessors
    NSArray *kpa=[loaderdict valueForKey:@"keyPreprocessorArray" default:[[NSMutableArray new] autorelease]];
    kpmenugroup->checkItemArray(kpa);
    
    // check primaryInputMethod
    NSString *pIM=[loaderdict valueForKey:@"primaryInputMethod" default:@""];
    immenugroup->checkItem(pIM);
    
    // fall back when there is no input method selected
    NSArray *imchk=immenugroup->getCheckedItems();
    if (![imchk count]) {
        murmur ("no IM checked, using the first item on the menu");
        immenugroup->checkFirstItem();
    }
    
    // check outputFilters
    NSArray *ofa=[loaderdict valueForKey:@"outputFilterArray" default:[[NSMutableArray new] autorelease]];
    ofmenugroup->checkItemArray(ofa);
}

void CVLoader::syncMenuAndConfig() {
    // Carbon-app-friendly addition
    NSAutoreleasePool *pool=[NSAutoreleasePool new];

    // anyway, are both groups are created according to module array,
    // we can be very sure that every checked item is in the module array
    // (they are autoreleased, so there's no memory management problem)
    NSArray *kpmgitems=kpmenugroup->getCheckedItems();
    NSArray *immgitems=immenugroup->getCheckedItems();
    NSArray *ofmgitems=ofmenugroup->getCheckedItems();

    // remember the result object from CVFindModules is autoreleased
    NSArray *kpsrc=CVFindModules(modarray, kpmgitems, @"OVKeyPreprocessor");
    NSArray *imsrc=CVFindModules(modarray, immgitems, @"OVInputMethod");
    NSArray *ofsrc=CVFindModules(modarray, ofmgitems, @"OVOutputFilter");

    // NOTA BENE: there is no "kparray" in CVLoader. Instead all loaded
    // key preprocessors are put into "imarray"--since key preprocessors
    // are themselves (derived) input methods. By doing this we also
    // put everything in order, i.e. all modules are put according to
    // the menu order into the imarray
    NSMutableArray *tmp=[[NSMutableArray new] autorelease];
    [imarray removeAllObjects];
    
    // since initializeModules() deletes everything in dst, we need this
    initializeModules(kpsrc, tmp, kpmenugroup);    
    [imarray addObjectsFromArray:tmp];
    
    initializeModules(imsrc, tmp, immenugroup);
    [imarray addObjectsFromArray:tmp];

    initializeModules(ofsrc, ofarray, ofmenugroup);
    
    // after so much work, we write these back to config

    kpmgitems=kpmenugroup->getCheckedItems();
    immgitems=immenugroup->getCheckedItems();        
    ofmgitems=ofmenugroup->getCheckedItems();
    [loaderdict setValue:kpmgitems forKey:@"keyPreprocessorArray"];
    [loaderdict setValue:ofmgitems forKey:@"outputFilterArray"];
    if ([immgitems count]) {
        [loaderdict setValue:[immgitems objectAtIndex:0] forKey:@"primaryInputMethod"];
		// write if lastPrimaryInputMethod doesn't exist yet
		[loaderdict valueForKey:@"lastPrimaryInputMethod" default:[immgitems objectAtIndex:0]];
	}
    else {
        [loaderdict setValue:@"" forKey:@"primaryInputMethod"];
	}

	// and get OVDisplayServer settings too
 	NSDictionary *dsrvdict=[[cfg dictionary] valueForKey:@"OVDisplayServer" default:CVGetDisplayServerConfig()];
	if (dspsrvr) {
        // NSLog(@"syncing config with display server");
		[dspsrvr setConfig:dsrvdict];
	}
	
    // sound and notification style settings
    NSString *shouldbeep=[loaderdict valueForKey:@"shouldBeep" default:@"1"];
    NSString *beepsound=[loaderdict valueForKey:@"beepSound" default:@""];
    srv->setShouldBeep([shouldbeep intValue]);
    srv->setBeepSound(beepsound);
    
    [cfg sync];
    [pool release];
}

void CVLoader::initializeModules(NSArray *src, NSMutableArray* dst, CVSmartMenuGroup *fallout) {
    // clean up dst array
    [dst removeAllObjects];
    
    NSMutableDictionary *cfgdict=[cfg dictionary];
    
    NSEnumerator *e=[src objectEnumerator];
    CVModuleWrapper *m;
    while (m=[e nextObject]) {
        NSString *mid=[m identifier];
        NSMutableDictionary *mdict=[cfgdict valueForKey:mid default:[[NSMutableDictionary new] autorelease]];
        CVDictionary cvd(mdict);
        
        NSString *atomic=CVGetAtomicInitLockFilename();
        CVAtomicInitStart(atomic, [m fromLibrary]);
        if ([m initializeWithConfig:&cvd service:srv]) {
            [dst addObject:m];      // added to our target array
        }
        else {
            // module init failed! cross out from our menu!
            fallout->uncheckItem(mid);
            fallout->disableItem(mid);
        }
        CVAtomicInitEnd(atomic);
    }
}

void CVLoader::pourModuleArrayIntoMenu(NSArray *ma, CVSmartMenuGroup *g, NSArray *orderedby) {
    NSMutableArray *a=[NSMutableArray arrayWithArray:ma];
    NSEnumerator *e;
    const char *lc=srv->locale();

    if (orderedby) {
        e=[orderedby objectEnumerator];
        NSString *mid;
        while (mid=[e nextObject]) {
            CVModuleWrapper *w=CVFindModule(a, mid);
            if (!w) continue;
            OVModule *m=[w module];        
            g->insertItem([w identifier], [NSString stringWithUTF8String: m->localizedName(lc)], [menudict valueForKey:[w identifier]]);
            [a removeObject:w];
        }
    }

    // pour in the remaining items
    e=[a objectEnumerator];
    CVModuleWrapper *w;
    
    while (w=[e nextObject]) {
        OVModule *m=[w module];        
        g->insertItem([w identifier], [NSString stringWithUTF8String: m->localizedName(lc)], [menudict valueForKey:[w identifier]]);
    }
}

void CVLoader::showOutputFilterStatus(NSString *modid, BOOL s) {
	if (!activecontext) return;
	
	CVModuleWrapper *w=CVFindModule(modarray, modid);
	if (!w) return;
	OVModule *m=[w module];
	NSString *display=[NSString stringWithFormat:@"%@ %@", [NSString stringWithUTF8String:m->localizedName(srv->locale())], s ? MSG(@"enabled") : MSG(@"disabled")];
	srv->notify([display UTF8String]);
	srv->fadeNotification();
}

void CVLoader::switchToLastPrimaryIM() {
    NSString *pIM=[NSString stringWithString: [loaderdict valueForKey:@"primaryInputMethod" default:@""]];
	NSString *lIM=[NSString stringWithString: [loaderdict valueForKey:@"lastPrimaryInputMethod" default:@""]];
	if ([pIM isEqualToString:lIM]) {
		// NSLog(@"lastPIM=PIM, return");
		return;
	}
	// NSLog(@"lastPIM!=PIM, switching");
	[loaderdict setValue:lIM forKey:@"primaryInputMethod"];
	[loaderdict setValue:pIM forKey:@"lastPrimaryInputMethod"];
	checkMenuItems();
	syncMenuAndConfig();
	if (activecontext) {
		activecontext->syncConfig();
	}
}

BOOL CVLoader::checkIfLastAtomicInitFailed(int timeout) {
    NSString *atomic=CVGetAtomicInitLockFilename();
    if (!CVIfPathExists(atomic)) return NO;    
    
    NSLog(@"CVLoader: found atomic lock file, wait to see if it's a failure or a lock in another process.");
    int tick=0;
    while (tick < timeout) {
        if (!CVIfPathExists(atomic)) return NO;
        usleep(10000);      // sleep for 1/100 second
        tick++;
        if (!(tick % 10)) NSLog(@"CVLoader: still waiting for the unlock (tick=%d, timeout=%d)", tick, timeout);
    }
    
    NSError *err;
    NSString *libname=[NSString stringWithContentsOfFile:atomic encoding:NSUTF8StringEncoding error:&err];
    NSString *datestr=[[NSDate date] descriptionWithCalendarFormat:nil timeZone:nil locale:nil];
    NSString *msg=[NSString stringWithFormat:MSG(@"AtomicInitFailed"), libname, atomic, datestr];

    char *username=getenv("USER");
    NSString *msgfile=[NSString stringWithFormat:@"%@-%s",
        CVGetAtomicInitErrorMessageFilename(),
        username ? username : "unknown-user"];

    // this won't work in 10.3
    // [msg writeToFile:msgfile atomically:YES encoding:NSUTF8StringEncoding error:&err];
    [msg writeToFile:msgfile atomically:NO];

    // unlink([atomic UTF8String]);
    system([[NSString stringWithFormat:@"open %@", msgfile] UTF8String]);
    return YES;
}

NSString *CVLoader::MSG(NSString *m) {
    return [loaderbundle localizedStringForKey:m value:nil table:nil];
}

CVContext::CVContext(CVLoader *p) {
    loader=p;
    buf=new CVBuffer(NULL, loader->ofarray, loader->srv);
    candistate=NULL;
    contexts=nil;
    // NSLog(@"dumping cfg");
    // NSLog([loader->cfg description]);
    stamp=[loader->cfg timeStamp];
    syncConfig(1);
}

CVContext::~CVContext() {
    CVSAFETY;
    delete buf;
    [contexts release];
}

void CVContext::activate(TSComposingBuffer *b) {
    CVSAFETY;
    buf->setComposingBuffer(b);
    loader->setActiveContext(this);
//  repositionInfoBoxes();
    loader->srv->closeNotification();
    if (!contexts) {
        contexts=[NSMutableArray new];
        syncConfig(1);
    }
    else {
        syncConfig();
    }
    loader->candi->hide()->clear()->update();

    // if we find a saved candistate exists
    if (candistate) {
        if (!buf->isEmpty()) {
            // buffer not empty, we restore the state
            loader->candi->restoreState(candistate);
        }
        delete candistate;
        candistate=NULL;
    }
    else {
        // the context is actually activated anew
        NSEnumerator *e=[contexts objectEnumerator];
        CVContextWrapper *w;
        while (w=[e nextObject]) {
            OVInputMethodContext *c=[w context];
            if (c) c->start(buf, loader->candi, loader->srv);
        }
    }
    
    // showPrimaryIM();
}

void CVContext::deactivate() {
    CVSAFETY;
    // if the buf is not empty, it means the user just switches away from the
    // current IM temporarily
    if (!buf->isEmpty())  {
        candistate=loader->candi->saveState();
    }
    else {
        // context is not only deactivated but is also switched  off
        NSEnumerator *e=[contexts objectEnumerator];
        CVContextWrapper *w;
        while (w=[e nextObject]) {
            OVInputMethodContext *c=[w context];
            if (c) c->end();
        }        
    }
    
    buf->setComposingBuffer(NULL);
    loader->setActiveContext(NULL);
    loader->candi->hide()->clear()->update();
    loader->srv->closeNotification();
}

void CVContext::fix() {
    CVSAFETY;
    // we can be 100% sure this is only called when an IM is actually
    // being deactivated, so we send out the remaining characters in the buffer
    if (!buf->isEmpty()) buf->send();
    clearAll();
}

int CVContext::event(char charcode, int modifiers) {
    CVSAFETY;
    CVKeyCode key(charcode, modifiers);
	
    loader->srv->closeNotification();
    repositionInfoBoxes();

    NSEnumerator *e=[contexts objectEnumerator];
    CVContextWrapper *w;
    while (w=[e nextObject]) {
        OVInputMethodContext *c=[w context];
        if (c) {
            if (c->keyEvent(&key, buf, loader->candi, loader->srv)) {
                // NSLog(@"key handled");
                return 1;
            }
        }
    }

    return 0;
}

void CVContext::repositionInfoBoxes() {
    // reposition our two boxes
    Point p=buf->getAppCursorPosition();
	p.v += 5;	// to make the infoboxes a bit lower, makes 'em look better
    loader->candi->setPosition(p);
//  p.v += loader->candi->height();				// obsolete
    loader->srv->setNotificationPosition(p);
}

void CVContext::clearAll() {
    if (!buf->isEmpty()) buf->clear()->update();
    if (loader->candi->onScreen()) loader->candi->hide();
    loader->candi->clear()->update();
    loader->srv->closeNotification();

    NSEnumerator *e=[contexts objectEnumerator];
    CVContextWrapper *w;
    while (w=[e nextObject]) {
        OVInputMethodContext *c=[w context];
        if (c) c->clear();
    }
}

void CVContext::syncConfig(int forced) {
    // if (forced) murmur("CVContext: forced update");
    CVTimeTag loaderst=[loader->cfg timeStamp];
    // if (!(stamp==loaderst)) murmur("config change: update");
    
    if (forced || !(stamp==loaderst)) {
        // we have to sync config and switch IM
        stamp=loaderst;
        clearAll();
        NSEnumerator *e=[contexts objectEnumerator];
        CVContextWrapper *w;
        while (w=[e nextObject]) {
            OVInputMethodContext *c=[w context];
            if (c) c->end();
        }        
        
        // murmur ("reloading contexts array, delete all context objects");
        // NSLog([loader->imarray description]);
        
        [contexts removeAllObjects];
        
        e=[loader->imarray objectEnumerator];
        CVModuleWrapper *mw;
        while (mw=[e nextObject]) {
            OVInputMethod *m=(OVInputMethod*)[mw module];
            // murmur ("creating context for IM %s", m->identifier());
            OVInputMethodContext *c=m->newContext();
            c->start(buf, loader->candi, loader->srv);
            CVContextWrapper *cw=[[CVContextWrapper alloc] initWithContext:c];
            [contexts addObject:cw];
            [cw release];
        }
        
        if (!forced) showPrimaryIM();
    }
}

void CVContext::showPrimaryIM() {
    NSString *pim=[loader->loaderdict valueForKey:@"primaryInputMethod"];
    int c=[loader->imarray count];
    if (!c) return;
    for (int i=0; i<c; i++) { 
        CVModuleWrapper *mw=[loader->imarray objectAtIndex:i];
        if (!mw) continue;
        if ([[mw identifier] isEqualToString:pim]) {
            OVModule *om=[mw module];
            loader->srv->notify(om->localizedName(loader->srv->locale()));
            loader->srv->fadeNotification();
        }
    }
}
