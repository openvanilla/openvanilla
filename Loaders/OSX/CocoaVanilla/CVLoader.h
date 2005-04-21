// CVLoader.h

#ifndef __CVLoader_h
#define __CVLoader_h

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include "TSComposingBuffer.h"
#include "CVConfig.h"
#include "CVService.h"
#include "CVInfoBox.h"
#include "CVSmartMenu.h"
#include "CVCandidate.h"
#include "CVBuffer.h"

class CVContext;

class CVLoader {
public:
    CVLoader();
    virtual ~CVLoader();
    int init(MenuRef m);
    CVContext *newContext();
    void setActiveContext(CVContext *c);
    void menuHandler(unsigned int cmd);
protected:
    void checkMenuItems();
    void initializeModules(NSArray *src, NSMutableArray *dst, CVSmartMenuGroup *fallout);
    void syncMenuAndConfig();       // sync menu items and config, re-assemble IM and OF arrays
    void pourModuleArrayIntoMenu(NSArray *a, CVSmartMenuGroup *g);

    CVService *srv;
    CVCandidate *candi;
    CVContext *activecontext;
    CVConfig *cfg;
    NSMutableDictionary *loaderdict;        // dictionary for loader config

    NSMutableArray *modarray;              // all loaded moudles
    NSMutableArray *ofarray;        // current in-use OF modules
    NSMutableArray *imarray;        // current in-use IM modules
    CVSmartMenuGroup *immenugroup;
    CVSmartMenuGroup *ofmenugroup;
    friend class CVContext;
};

class CVContext {
public:
    CVContext(CVLoader *p);
    virtual ~CVContext();
    void activate(TSComposingBuffer *b);
    void deactivate();
    void fix();
    int event(char charcode, int modifiers);
    
    void repositionInfoBoxes();
    void clearAll();
    void syncConfig(int forced=0);
    
protected:
    CVLoader *loader;
    CVBuffer *buf;
    NSMutableArray *contexts;
    CVTimeStamp stamp;
};

#endif
