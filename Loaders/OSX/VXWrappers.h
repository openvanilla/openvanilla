// VXWrappers.h

#ifndef __VXWrapper_h
#define __VXWrapper_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>

@interface VXModuleWrapper : NSObject
{
    OVModule *mod;
    NSString *loadedPath;
    BOOL inited;
    BOOL canUse;
}
-(void)dealloc;
-(id)initWithModule:(OVModule*)m libraryPath:(NSString*)p;
-(NSString*)description;
-(NSString*)moduleType;
-(NSString*)identifier;
-(OVModule*)module;
-(BOOL)usable;
-(BOOL)initialized;
-(BOOL)initialize:(OVDictionary*)globalPref localPref:(OVDictionary*)pref
 service:(OVService*)s userSpace:(NSString*)u;
@end

@interface VXContextWrapper : NSObject
{
    OVInputMethodContext *cntx;
}
-(id)initWithContext:(OVInputMethodContext*)c;
-(OVInputMethodContext*)context;
@end

#endif
