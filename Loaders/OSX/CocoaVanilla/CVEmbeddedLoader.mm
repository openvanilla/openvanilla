// CVEmbeddedLoader.m

#import "CVEmbeddedLoader.h"
#import "CVLoaderConfig.h"
#import "CVLoaderUtility.h"
#import "OVDisplayServer.h"

@interface CVDumpDisplayServer : NSObject <OVDisplayServer> {
}
@end

@implementation CVEmbeddedLoader 
- (void)dealloc {
    delete srv;
    [dpsrv release];
    [cfg release];
    [modlist release];
    [loadhistory release];
    [super dealloc];
}
- (id)init {
    self=[super init];
    if (!self) return nil;

    dpsrv=[CVDumpDisplayServer new];
    srv=new CVService(CVGetUserSpacePath(), dpsrv);
    cfg=[[CVConfig alloc] initWithFile:CVGetUserConfigFilename() defaultData:nil];
    modlist=[NSMutableArray new];
    loadhistory=[NSMutableDictionary new];
    
    // load everything!
    [modlist addObjectsFromArray: CVLoadEverything(CVGetModuleLoadPath(), srv, nil, nil, loadhistory, nil)];
    return self;
}
- (CVConfig*)config {
    return cfg;
}
- (CVService*)service {
    return srv;
}
- (NSArray*)moduleList {
    return modlist;
}
- (NSDictionary*)loadHistory {
    return loadhistory;
}
@end

@implementation CVDumpDisplayServer
- (void)setConfig:(NSDictionary*)cfg {}
- (void)candidateShow {}
- (void)candidateHide {}
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p {}
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p {}
- (void)notifyClose {}
- (void)notifyFade {}
- (void)aboutDialog {}
@end
