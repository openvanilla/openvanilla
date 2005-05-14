// CVLoader.mm
#define OV_DEBUG
#include <ctype.h>
#include <unistd.h>
#include "CVLoader.h"
#include "TSBundle.h"
#include <OpenVanilla/OVUtility.h>
#include "CVLoaderUtility.h"
#include "CVLoaderConfig.h"
#include "CVDictionary.h"
#include "CVKeyCode.h"
#include "NSDictionaryExtension.h"
#include "OVDisplayServer.h"

// localization facilitator: gather all MSG occuranges into Localizable.strings
#define MSG(x)      x

enum {      // CVLMI = CVLoader Menu Item
    CVLMI_IMGROUPSTART=1000,
    CVLMI_OFGROUPSTART=2000,
    CVLMI_ABOUT=3000,
    CVLMI_HELP=3001
};

CVLoader::CVLoader() {
    // actual initialization code in CVLoadver::init()
}

CVLoader::~CVLoader() {
    // although this is a VERY BAD habit, but we DO NOT DELETE anything here
    // the reason: text component is never released once it's loaded,
    // everything goes away as the application is closed by OS X
}

int CVLoader::init(MenuRef m) {
    activecontext=NULL;

    NSBundle *bundle=[NSBundle bundleWithIdentifier:@ TSBUNDLEID];
    if (!bundle) {
        murmur ("CVLoader: fatal error, bundle %s not found", TSBUNDLEID);
        return 0;
    }
    
	connectDisplayServer();
	
    CVInfoBox *candiib, *ntfyib;        
    candiib=[[CVInfoBox alloc] initWithWindowNibName:@"CVInfoBox"];
    ntfyib=[[CVInfoBox alloc] initWithWindowNibName:@"CVInfoBox"];
    if (!candiib || !ntfyib) {
        murmur ("CVLoader: fatal error, failed CVInfoBox.nib loading");
        return 0;
    }
    
    // wakes the two controllers up from .nib file
    NSPanel *cp=(NSPanel*)[candiib window];
    NSPanel *np=(NSPanel*)[ntfyib window];
	[candiib setName:@"candi"];
	[ntfyib setName:@"notify"];
    [cp setFloatingPanel:YES];
    [np setFloatingPanel:YES];
    [cp setBecomesKeyOnlyIfNeeded:YES];
    [np setBecomesKeyOnlyIfNeeded:YES];
    srv=new CVService(CVGetUserSpacePath(), ntfyib);
    candi=new CVCandidate(dspsrvr);
    cfg=[[CVConfig alloc] initWithFile:CVGetUserConfigFilename() defaultData:nil];
    modarray=[NSMutableArray new];
    imarray=[NSMutableArray new];
    ofarray=[NSMutableArray new];
    
    // load everything!
    [modarray addObjectsFromArray: CVLoadEverything(CVGetModuleLoadPath(), srv)];

    // set up the menus
    immenugroup=new CVSmartMenuGroup(m, CVLMI_IMGROUPSTART, bundle, CVSM_EXCLUSIVE);
    ofmenugroup=new CVSmartMenuGroup(m, CVLMI_OFGROUPSTART, bundle, CVSM_MULTIPLE);
    immenugroup->insertTitle(MSG(@"input methods"));
    pourModuleArrayIntoMenu(CVGetModulesByType(modarray, @"OVInputMethod"), immenugroup);
    immenugroup->insertSeparator();
    ofmenugroup->insertTitle(MSG(@"output filters"));
    pourModuleArrayIntoMenu(CVGetModulesByType(modarray, @"OVOutputFilter"), ofmenugroup);    
    ofmenugroup->insertSeparator();
    CVInsertMenuItem(m, CVLMI_ABOUT, bundle, @"about");
    CVInsertMenuItem(m, CVLMI_HELP, bundle, @"help");    

    // now that we have menus loaded, we now do configuration stuff, whew!
    NSMutableDictionary *dict=[cfg dictionary];
    loaderdict=[dict valueForKey:@"OVLoader" default:[[NSMutableDictionary new] autorelease]];

    // and we check menu items according to the config
    checkMenuItems();

    // now we sync the menu items and config, plus we assemble the IM/OF arrays
    syncMenuAndConfig();

    return 1;
}

CVContext *CVLoader::newContext() {
    return new CVContext(this);
}

void CVLoader::setActiveContext(CVContext *c) {
    activecontext=c;
    if (!c) return;
    
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
    
    checkMenuItems();
    syncMenuAndConfig();
}

void CVLoader::menuHandler(unsigned int cmd) {
    if (immenugroup->clickItem(cmd)) {
        syncMenuAndConfig();
        if (activecontext) {
            activecontext->syncConfig();
        }
        return;
    }

    if (ofmenugroup->clickItem(cmd)) {
        syncMenuAndConfig();
        return;
    }

    switch (cmd) {
        case CVLMI_ABOUT:
            murmur ("about menu item clicked");
            return;
        case CVLMI_HELP:
            murmur ("help menu item clicked");
            return;
    }
    
    murmur("unknown menu item %d clicked", cmd);
}

void CVLoader::connectDisplayServer() {
	dspsrvr=nil;
	
	dspsrvr=[[NSConnection rootProxyForConnectionWithRegisteredName:@"OVDisplayServer" host:nil] retain];
	if (!dspsrvr) {
		system([[NSString stringWithFormat:@"open %@", CVLC_DISPLAYSERVER] UTF8String]);
		
		// a total timeout of 1 sec
		for (int retry=0; retry<20; retry++) {
			NSLog([NSString stringWithFormat:@"retry %d", retry]);
			usleep(50000);		// wait for the server to be brought up (0.05 sec)
			dspsrvr=[[NSConnection rootProxyForConnectionWithRegisteredName:@"OVDisplayServer" host:nil] retain];
			if (dspsrvr) break;
		}
	}
	
	if (dspsrvr) [dspsrvr setProtocolForProxy:@protocol(OVDisplayServer)];
}

void CVLoader::checkMenuItems() {
    // check primaryInputMethod first
    immenugroup->uncheckAll();
    ofmenugroup->uncheckAll();
    
    NSString *pIM=[loaderdict valueForKey:@"primaryInputMethod" default:@""];
    immenugroup->checkItem(pIM);
    
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
    // anyway, are both groups are created according to module array,
    // we can be very sure that every checked item is in the module array
    NSArray *immgitems=immenugroup->getCheckedItems();
    NSArray *ofmgitems=ofmenugroup->getCheckedItems();

    // remember the result object from CVFindModules is autoreleased
    NSArray *imsrc=CVFindModules(modarray, immgitems, @"OVInputMethod");
    NSArray *ofsrc=CVFindModules(modarray, ofmgitems, @"OVOutputFilter");
    
    initializeModules(ofsrc, ofarray, ofmenugroup);
    initializeModules(imsrc, imarray, immenugroup);
    
    // after so much work, we write these back to config
    ofmgitems=ofmenugroup->getCheckedItems();
    immgitems=immenugroup->getCheckedItems();    
    [loaderdict setValue:ofmgitems forKey:@"outputFilterArray"];
    if ([immgitems count])
        [loaderdict setValue:[immgitems objectAtIndex:0] forKey:@"primaryInputMethod"];
    else
        [loaderdict setValue:@"" forKey:@"primaryInputMethod"];

	// and get OVDisplayServer settings too
	NSDictionary *dsrvdict=[[cfg dictionary] valueForKey:@"OVDisplayServer" default:CVGetDisplayServerConfig()];
	if (dspsrvr) {
		NSLog(@"syncing config with display server");
		[dspsrvr setConfig:dsrvdict];
	}
	
    [cfg sync];
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
        
        if ([m initializeWithConfig:&cvd service:srv]) {
            [dst addObject:m];      // added to our target array
        }
        else {
            // module init failed! cross out from our menu!
            fallout->uncheckItem(mid);
            fallout->disableItem(mid);
        }
    }
}

void CVLoader::pourModuleArrayIntoMenu(NSArray *a, CVSmartMenuGroup *g) {
    NSEnumerator *e=[a objectEnumerator];
    CVModuleWrapper *w;
    const char *lc=srv->locale();
    
    while (w=[e nextObject]) {
        OVModule *m=[w module];
        
        g->insertItem(
            [NSString stringWithUTF8String: m->identifier()],
            [NSString stringWithUTF8String: m->localizedName(lc)]
        );
    }
}

CVContext::CVContext(CVLoader *p) {
    loader=p;
    buf=new CVBuffer(NULL, loader->ofarray, loader->srv);
    candistate=NULL;
    contexts=nil;
    stamp=[loader->cfg timeStamp];
    syncConfig(1);
}

CVContext::~CVContext() {
    delete buf;
    [contexts release];
}

void CVContext::activate(TSComposingBuffer *b) {
    buf->setComposingBuffer(b);
    loader->setActiveContext(this);
    repositionInfoBoxes();
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
    clearAll();
}

int CVContext::event(char charcode, int modifiers) {
    CVKeyCode key(charcode, modifiers);
    
    loader->srv->closeNotification();
    repositionInfoBoxes();
    NSEnumerator *e=[contexts objectEnumerator];
    CVContextWrapper *w;
    while (w=[e nextObject]) {
        OVInputMethodContext *c=[w context];
        if (c) if (c->keyEvent(&key, buf, loader->candi, loader->srv)) return 1;
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
        
//      murmur ("reloading contexts array, delete all context objects");
        [contexts removeAllObjects];
        
        e=[loader->imarray objectEnumerator];
        CVModuleWrapper *mw;
        while (mw=[e nextObject]) {
            OVInputMethod *m=(OVInputMethod*)[mw module];
//          murmur ("creating context for IM %s", m->identifier());
            OVInputMethodContext *c=m->newContext();
            c->start(buf, loader->candi, loader->srv);
            CVContextWrapper *cw=[[CVContextWrapper alloc] initWithContext:c];
            [contexts addObject:cw];
            [cw release];
        }
        
        showPrimaryIM();
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
