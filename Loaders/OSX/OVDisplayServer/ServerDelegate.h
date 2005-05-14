/* MyDelegate */

#import <Cocoa/Cocoa.h>
#import "OVDisplayServer.h"

@interface ServerDelegate : NSObject <OVDisplayServer> {
	NSWindow *candi;
	NSTextField *canditext;
	// NSWindow *noti;
	// NSTextField *notitext;
}
- (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w textField:(NSTextField*)t;
- (NSTextField*)createTextField:(NSRect)fr;
- (NSWindow*)createWindow:(NSRect)fr;
@end
