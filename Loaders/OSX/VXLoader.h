// VXLoader.h: OpenVanilla Loader for OS X interfaces

#ifndef __VXLoader_h
#define __VXLoader_h

#include <Cocoa/Cocoa.h>
#define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

#include "VXConstants.h"
#include "VXUtility.h"
#include "VXPreferenceFile.h"
#include "VXLoaderUtility.h"
#include "VXWrappers.h"
#include "VXOpenVanilla.h"
#import "VXInfoBoxController.h"

class VXContext;

class VXLoader
{
public:
    int init(MenuRef mnu)
    {
        pencilmenu=mnu;
        srv=new VXService;
        pref=new VXPreferenceFile(VXPREFPATH);
        
        murmur ("loaded pref content: %s", [[pref->getDictionary(@VXLOADERPREFKEY)
            description] UTF8String]);                    
        
        NSMutableArray *loadlist=[[NSMutableArray new] autorelease];
        [loadlist addObject: @VXMODLIBPATH];
        [loadlist addObject: @VXUSRMODLIBPATH];
        [loadlist addObject: @"/Users/lukhnos/logiciels/openvanilla/Modules/IMExample"];
        
        murmur ("loading IBC!");
        ibc=[[VXInfoBoxController alloc] initWithWindowNibName: @"VXLoader"];
        if (!ibc) return 0;
        ibw=[ibc window];
        
        VXDictionary gp(pref->getDictionary(@VXLOADERPREFKEY));
        
        modulelist=VXLoadEverything(loadlist, &gp, srv);
        murmur ("no module!");
        if (![modulelist count]) return 0;
        [modulelist retain];
        
        murmur("%d modules loaded!", [modulelist count]);
        return 1;
    }
    
    VXContext *newContext();
    
    void menuHandler(unsigned int cmd)
    {
    }
    
    void setActiveContext(VXContext *c)
    {
        activecontext=c;
    }
    
    void syncConfig(int forced=0)
    {
        if (!forced && (timestamp==pref->timeStamp())) return;
        timestamp=pref->read();
        ofarray=assembleModuleArray(modulelist, 
            [pref->getDictionary(@VXLOADERPREFKEY) objectForKey:
                @VXUSINGOFARRAYKEY]);
        imarray=assembleModuleArray(modulelist, 
            [pref->getDictionary(@VXLOADERPREFKEY) objectForKey:
                @VXUSINGIMARRAYKEY]);
        
        murmur("IMArray content: %s\n", [[[pref->getDictionary(@VXLOADERPREFKEY) objectForKey:
                @VXUSINGIMARRAYKEY] description] UTF8String]);
        murmur("imarray count=%d\n", [imarray count]);
        
        [ofarray retain];
        [imarray retain];
            
        // and tell every module loaded that we have updated...
        // updatePref(ofarray);
        // updatePref(imarray);
    }
    
    NSArray *assembleModuleArray(NSArray *modlist, NSArray *requirement)
    {
        int writeFlag=0;
        
        NSMutableArray *a=[[NSMutableArray new] autorelease];
        NSEnumerator *e=[requirement objectEnumerator];
        while (NSString *s=[e nextObject])
        {
            VXModuleWrapper *m=VXFindModule(modlist, s);
            if (!m) continue;
            
            VXDictionary gp(pref->getDictionary(@VXLOADERPREFKEY));
            VXDictionary lp(pref->getDictionary([m identifier]));
        
            // at least one moudle is unintialized, we'll have to write pref
            if (![m initialized]) writeFlag=1;
            
            // we have to initialize it first (re-init is always ok as won't happen)
            if ([m initialize:&gp localPref:&lp service:srv userSpace:
                VXGetUserDataPath(m, @VXLIBUSRSPACEPREFIX)]) [a addObject:m];
        }
        
        if (writeFlag) pref->write();
        return a;
    }
    
protected:
    MenuRef pencilmenu;
    VXContext *activecontext;
    VXPreferenceFile *pref;
    VXTimeStamp timestamp;
    NSArray *modulelist;
    NSArray *ofarray;
    NSArray *imarray;
    VXService *srv;
     
    VXInfoBoxController *ibc;
    NSWindow *ibw;
    
    friend class VXContext;
};

class VXContext
{
public:
    VXContext(VXLoader *p)
    {
        parent=p;
        imarray=[NSMutableArray new];
        syncConfig(1);
    }
    ~VXContext()
    {
        [imarray release];
    }

    void activate(TSComposingBuffer *buf)
    {

        VXBuffer b(buf);
        VXInfoBox ib(parent->ibc, parent->ibw);   
        NSEnumerator *e=[imarray objectEnumerator];
        while (VXContextWrapper *c=[e nextObject])
            [c context]->start(&b, &ib, parent->srv);
    }
    
    void deactivate(TSComposingBuffer *buf)
    {
        fix(buf);
    }
    
    void fix(TSComposingBuffer *buf)
    {
        NSEnumerator *e=[imarray objectEnumerator];
        while (VXContextWrapper *c=[e nextObject])
            [c context]->clear();
        buf->clear()->update();
    }
    
    int event(TSComposingBuffer *buf, char charcode, int modifiers)
    {
            Point p=buf->getAppCursorPosition();
        NSPoint pp;
        pp.x=p.h;
        NSArray* screens = [NSScreen screens];

        if ([screens count])
        {
            NSRect mainScreenRect = [[screens objectAtIndex:0] frame];
            float newYOrigin = NSMaxY(mainScreenRect);
            pp.y=newYOrigin-p.v;
        }
        
        [parent->ibw setFrameTopLeftPoint: pp];
            murmur("event! imarray=%d", [imarray count]);
        VXBuffer b(buf);
        VXInfoBox ib(parent->ibc, parent->ibw);   
        VXKeyCode k(charcode, modifiers);
        NSEnumerator *e=[imarray objectEnumerator];
        while (VXContextWrapper *c=[e nextObject])
        {
            murmur("context! key=%d", charcode);
            if ([c context]->keyEvent(&k, &b, &ib, parent->srv)) return 1;
        }
        return 0;
    }
    
protected:
    void syncConfig(int forced=0)
    {
        parent->syncConfig();
        if (!forced && timestamp==parent->timestamp) return;
        timestamp=parent->timestamp;
        
        [imarray removeAllObjects];
        NSEnumerator *e=[parent->imarray objectEnumerator];
        while (VXModuleWrapper *m=[e nextObject])
        {
            // type check?
            
            OVInputMethod *im=(OVInputMethod*)[m module];
            murmur("enum: %s\n", im->identifier());
            OVInputMethodContext *c=im->newContext();
            VXContextWrapper *cw=[[VXContextWrapper alloc] initWithContext: c];
            [imarray addObject: cw];
        }
        murmur("context imarray=%d\n", [imarray count]);
    }
    
    NSMutableArray *imarray;
    VXLoader *parent;
    VXTimeStamp timestamp;
};

inline VXContext *VXLoader::newContext()
{
    VXContext *c=new VXContext(this);
    return c;
}


#endif
