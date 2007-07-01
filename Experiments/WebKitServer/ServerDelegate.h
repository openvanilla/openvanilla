/* ServerDelegate */

#import <Cocoa/Cocoa.h>
#import <Growl/GrowlApplicationBridge.h>
#import "CandiWindow.h"

#define OVDSPSRVR_NAME		@"OVWebKitServer-0.7.2"

@protocol OVDisplayServer
- (void)setConfig:(bycopy NSDictionary*)cfg;
- (void)candidateShow;
- (void)candidateHide;
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p;
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p;
- (void)notifyClose;
- (void)aboutDialog;
- (BOOL)ping;
@end

@interface NotifyDelegate : NSObject <GrowlApplicationBridgeDelegate>
{
}
@end

@interface ServerDelegate : NSObject <OVDisplayServer>
{
    //IBOutlet id candiWindow;
	NotifyDelegate * notifyDelegate;	
	CandiWindow * candiWindow;
    IBOutlet id candiWeb;
	IBOutlet id aboutWindow;

	BOOL candion;
    int notistyle;
	int candialwaysshow;
	int candifix;
	NSURL *urlbase;
}
- (IBAction)sendKeyTest:(id)sender;
@end
