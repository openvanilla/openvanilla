/* MyDelegate */

#import <Cocoa/Cocoa.h>
#import "OVDisplayServer.h"

@interface ServerDelegate : NSObject <OVDisplayServer> {
	NSWindow *candi;
	NSTextField *canditext;
	BOOL candion;
	NSWindow *noti;
	NSTextField *notitext;
	float notialpha;
	float fadealpha;
	NSTimer *fadetimer;
    NSColor *defaultbackground;
}
- (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w textField:(NSTextField*)t;
- (void)updateTextInWindow:(NSWindow*)w textField:(NSTextField*)t text:(NSString*)s position:(Point)p;
- (NSTextField*)createTextField:(NSRect)fr;
- (NSWindow*)createWindow:(NSRect)fr;
- (void)solveConflict;
- (void)stopTimer;
- (void)fadeStart;
- (void)fadeWork;
@end
