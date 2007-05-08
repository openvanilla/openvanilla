/* ServerDelegate */

#import <Cocoa/Cocoa.h>

#define OVDSPSRVR_NAME		@"OVDisplayServer-0.7.2"

@protocol OVDisplayServer
- (void)setConfig:(bycopy NSDictionary*)cfg;
- (void)candidateShow;
- (void)candidateHide;
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p;
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p;
- (void)notifyClose;
- (void)notifyFade;
- (void)aboutDialog;
@end

@interface ServerDelegate : NSObject <OVDisplayServer>
{
    IBOutlet id candi;
    IBOutlet id candiweb;
    IBOutlet id noti;
    IBOutlet id notiweb;

	BOOL candion;
	float notialpha;
	float fadealpha;
    int notistyle;
	int candialwaysshow;
	int candifix;
	NSTimer *fadetimer;
    NSColor *defaultbackground;
	NSURL *urlbase;
}
- (IBAction)sendKeyTest:(id)sender;
// - (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w;
- (void)solveConflict;
- (void)stopTimer;
- (void)fadeStart;
- (void)fadeWork;
@end
