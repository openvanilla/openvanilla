// VXWrappers.mm

#include "VXUtility.h"
#include "VXWrappers.h"

@implementation VXModuleWrapper
-(id)initWithModule:(OVModule*)m libraryPath:(NSString*)p
{
    self=[super init];
    if (self)
    {
        inited=FALSE;
        canUse=FALSE;
        mod=m;
        if (loadedPath) [loadedPath release];
        loadedPath=p;
        [loadedPath retain];
    }
    return self;
}
-(void)dealloc
{
    if (loadedPath) [loadedPath release];
    [super dealloc];
}
-(NSString*)description
{
    if (!mod) return @"";
    return [NSString stringWithFormat: 
        @"(module id=%@, type=%s, module path=%@, initialized=%s)",
        VXSTR(mod->identifier()), mod->moduleType(), 
        loadedPath, inited ? "TRUE" : "FALSE"];
}
-(NSString*)moduleType
{
    return mod ? VXSTR(mod->moduleType()) : @"";
}
-(NSString*)identifier
{
    return mod ? VXSTR(mod->identifier()) : @"";
}
-(OVModule*)module
{
    return mod;
}
-(BOOL)usable
{
    return canUse;
}
-(BOOL)initialized
{
    return inited;
}
-(BOOL)initialize:(OVDictionary*)globalPref localPref:(OVDictionary*)pref
 service:(OVService*)s userSpace:(NSString*)u
{
    if (inited) return canUse;
    inited=TRUE;

    if (mod->initialize(globalPref, pref, s, [loadedPath UTF8String], 
        [u UTF8String], "/"))
        return canUse=TRUE;

    return canUse=FALSE;
}
@end

@implementation VXContextWrapper
-(id)initWithContext:(OVInputMethodContext*)c
{
    cntx=c;
    return self;
}
-(OVInputMethodContext*)context
{
    return cntx;
}
@end
