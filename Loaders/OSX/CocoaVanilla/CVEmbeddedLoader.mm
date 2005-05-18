// CVEmbeddedLoader.m

#import "CVEmbeddedLoader.h"
#import "CVLoaderConfig.h"
#import "CVLoaderUtility.h"
#import "OVDisplayServer.h"

@interface CVDumpDisplayServer : NSObject <OVDisplayServer> {
    NSMutableString *msg;
}
- (NSString*)notifyMessage;
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
    srv->setNotificationPosition((Point){1,1});     // to prevent "hide ghost notification" feature
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
- (NSString*)notifyMessage {
    return [dpsrv notifyMessage];
}
- (void)clearNotification {
    [dpsrv notifyClose];
}
@end

@implementation CVDumpDisplayServer
- (id)init {
    if (self=[super init]) {
        msg=[NSMutableString new];
    }
    return self;
}
- (void)dealloc {
    [msg release];
    [super dealloc];
}
- (NSString*)notifyMessage {
    return msg;
}
- (void)setConfig:(NSDictionary*)cfg {}
- (void)candidateShow {}
- (void)candidateHide {}
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p {}
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p {
    [msg setString:s];
}
- (void)notifyClose {
    [msg setString:@""];
}
- (void)notifyFade {}
- (void)aboutDialog {}
@end
