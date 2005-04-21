// CVWrappers.mm

#include "CVWrappers.h"

@implementation CVModuleWrapper
- (id)initWithModule:(OVModule*)m loadedPath:(NSString*)p {
    self=[super init];
    if (self) {
        inited=NO;
        canuse=NO;
        mod=m;
        loadedpath=[[NSString alloc] initWithString:p];
    }
    return self;
}
- (void)dealloc {
    [loadedpath release];
    [super dealloc];
}
- (NSString*)description {
    if (!mod) return @"";
    return [NSString stringWithFormat: 
        @"(module id=%s, type=%s, module path=%@, initialized=%s, usable=%s)",
        mod->identifier(), mod->moduleType(), loadedpath,
        inited ? "YES" : "NO", canuse ? "YES" : "NO"];
}
- (NSString*)moduleType {
    return mod ? [NSString stringWithUTF8String:mod->moduleType()] : @"";
}
- (NSString*)identifier {
    return mod ? [NSString stringWithUTF8String:mod->identifier()] : @"";
}
- (OVModule*)module {
    return mod;
}
- (BOOL)usable {
    return canuse;
}
- (BOOL)initializeWithConfig:(OVDictionary*)cfg service:(OVService*)s {
    if (!mod) return NO;
    if (inited) return canuse;
    inited=YES;
    if (mod->initialize(cfg, s, [loadedpath UTF8String])) return canuse=YES;
    return canuse=NO;
}
@end

@implementation CVContextWrapper
-(id)initWithContext:(OVInputMethodContext*)c {
    cntx=c;
    return self;
}
-(void)dealloc {
    if (cntx) {
        fprintf (stderr, "context deleted!\n");
        delete cntx;
    }
}
-(OVInputMethodContext*)context {
    return cntx;
}
@end
