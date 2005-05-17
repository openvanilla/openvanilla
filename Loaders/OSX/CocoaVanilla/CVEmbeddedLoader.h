// CVEmbeddedLoader.h

#ifndef __CVEmbeddedLoader_h
#define __CVEmbeddedLoader_h

#import <Cocoa/Cocoa.h>
#import "CVService.h"
#import "CVConfig.h"

@interface CVEmbeddedLoader : NSObject {
    id dpsrv;       // dump display server;
    CVService *srv;
    CVConfig *cfg;
    NSMutableArray *modlist;
    NSMutableDictionary *loadhistory;
}
- (id)init;
- (void)dealloc;
- (CVConfig*)config;
- (CVService*)service;
- (NSArray*)moduleList;
- (NSDictionary*)loadHistory;
@end




#endif
