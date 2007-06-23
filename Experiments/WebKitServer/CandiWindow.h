/* CandiWindow */
#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

@interface CandiWindow : NSWindow
{
    NSPoint initialLocation;
    WebView* candiweb;	
}
@end
