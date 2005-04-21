// CVLoader.mm
#define OV_DEBUG
#include <ctype.h>
#include "CVLoader.h"
#include "TSBundle.h"
#include <OpenVanilla/OVUtility.h>
#include "CVLoaderUtility.h"
#include "CVLoaderConfig.h"
#include "CVDictionary.h"
#include "CVKeyCode.h"
#include "NSDictionaryExtension.h"

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

void CVLoader::setActiveContext(CVContext *c) {
    activecontext=c;
    
    if (![cfg needSync]) return;
    [cfg sync];
    
    // now we tell every usable module to update config
    NSMutableDictionary *cfgdict=[cfg dictionary];
    NSEnumerator *e=[modarray objectEnumerator];
    CVModuleWrapper *m;
    while (m=[e nextObject]) {
        if (![m usable]) continue;
        NSString *mid=[m identifier];
        murmur("trying to update module: %s", [mid UTF8String]);
        NSMutableDictionary *mdict=[cfgdict valueForKey:mid default:[[NSMutableDictionary new] autorelease]];
        CVDictionary cvd(mdict);
        OVModule *om=[m module];
        om->update(&cvd, srv);
    }
}

int CVLoader::init(MenuRef m) {
    murmur("OpenVanilla Loader initializing");
    
    activecontext=NULL;

    NSBundle *bundle=[NSBundle bundleWithIdentifier:@ TSBUNDLEID];
    if (!bundle) {
        murmur ("CVLoader: fatal error, bundle %s not found", TSBUNDLEID);
        return 0;
    }

    
    CVInfoBox *candiib, *ntfyib;        
    candiib=[[CVInfoBox alloc] initWithWindowNibName:@"CVInfoBox"];
    ntfyib=[[CVInfoBox alloc] initWithWindowNibName:@"CVInfoBox"];
    if (!candiib || !ntfyib) {
        murmur ("CVLoader: fatal error, failed CVInfoBox.nib loading");
        return 0;
    }
    
    // wakes the two controllers up from .nib file
    [candiib window];
    [ntfyib window];

    srv=new CVService(CVLC_USERSPACE_PATH, ntfyib);
    candi=new CVCandidate(candiib);
    cfg=[[CVConfig alloc] initWithFile:CVLC_USERCONFIG defaultData:nil];
    
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

void CVLoader::checkMenuItems() {
    // check primaryInputMethod first
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
        murmur("trying to init module: %s", [mid UTF8String]);
        NSMutableDictionary *mdict=[cfgdict valueForKey:mid default:[[NSMutableDictionary new] autorelease]];
        CVDictionary cvd(mdict);
        
        if ([m initializeWithConfig:&cvd service:srv]) {
            murmur("init successful");
            [dst addObject:m];      // added to our target array
        }
        else {
            // module init failed! cross out from our menu!
            murmur("init failed!");
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

void CVLoader::menuHandler(unsigned int cmd) {
    if (immenugroup->clickItem(cmd)) {
        murmur("im menu grop clicked, checked items=");
        NSArray *ia=immenugroup->getCheckedItems();
        murmur([[ia description] UTF8String]);
        syncMenuAndConfig();
        if (activecontext) {
            activecontext->syncConfig();
        }
        return;
    }

    if (ofmenugroup->clickItem(cmd)) {
        murmur("of menu grop clicked, checked items=");
        NSArray *ia=ofmenugroup->getCheckedItems();
        murmur([[ia description] UTF8String]);
        murmur("syncing menu and config");
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

CVContext *CVLoader::newContext() {
    return new CVContext(this);
}

CVContext::CVContext(CVLoader *p) {
    murmur ("context created");
    loader=p;
    buf=NULL;
    contexts=[NSMutableArray new];
    stamp=[loader->cfg timeStamp];
    syncConfig(1);
}

CVContext::~CVContext() {
    murmur ("context deleted");
    [contexts release];
}

void CVContext::activate(TSComposingBuffer *b) {
    murmur ("context activated");
    loader->setActiveContext(this);
    syncConfig();

    buf=new CVBuffer(b, loader->ofarray, loader->srv);
    loader->candi->clear();
    repositionInfoBoxes();

    NSEnumerator *e=[contexts objectEnumerator];
    CVContextWrapper *w;
    while (w=[e nextObject]) {
        OVInputMethodContext *c=[w context];
        if (c) c->start(buf, loader->candi, loader->srv);
    }
}

void CVContext::deactivate() {
    murmur("context deactivate");
    loader->setActiveContext(NULL);
    delete buf;
    buf=NULL;
}

void CVContext::fix() {
    murmur("context fix");
    if (buf->isEmpty()) {
        murmur("buffer not empty, sending");
        buf->send();
    }
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
    loader->candi->setPosition(p);
    p.v += loader->candi->height();
    loader->srv->setNotificationPosition(p);
}

void CVContext::clearAll() {
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
    if (forced) murmur("CVContext: forced update");
    CVTimeStamp loaderst=[loader->cfg timeStamp];
    if (!(stamp==loaderst)) murmur("config change: update");
    
    if (forced || !(stamp==loaderst)) {
        stamp=[loader->cfg timeStamp];
        clearAll();
        
        murmur ("reloading contexts array, delete all context objects");
        [contexts removeAllObjects];
        
        NSEnumerator *e=[loader->imarray objectEnumerator];
        CVModuleWrapper *w;
        while (w=[e nextObject]) {
            OVInputMethod *m=(OVInputMethod*)[w module];
            murmur ("creating context for IM %s", m->identifier());
            OVInputMethodContext *c=m->newContext();
            CVContextWrapper *cw=[[CVContextWrapper alloc] initWithContext:c];
            [contexts addObject:cw];
            [cw release];
        }
    }
}

