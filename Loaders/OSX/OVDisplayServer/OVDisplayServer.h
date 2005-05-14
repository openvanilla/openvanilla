// OVDisplayServer.h

#ifndef __OVDisplayServer_h
#define __OVDisplayServer_h

@protocol OVDisplayServer
- (void)setConfig:(NSDictionary*)cfg;
- (void)candidateShow;
- (void)candidateHide;
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p;
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p;
- (void)notifyClose;
- (void)notifyFade;
- (void)aboutDialog;
@end

#endif
