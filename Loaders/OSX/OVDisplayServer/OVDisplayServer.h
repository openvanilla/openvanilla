// OVDisplayServer.h

#ifndef __OVDisplayServer_h
#define __OVDisplayServer_h

@protocol OVDisplayServer
- (void)setConfig:(NSDictionary*)cfg;
- (void)candidateShow;
- (void)candidateHide;
- (void)candidateUpdate:(NSString*)s position:(Point)p;
@end

#endif
