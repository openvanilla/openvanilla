/* CVPreviewView */

#import <Cocoa/Cocoa.h>

@interface CVPreviewView : NSView
{
	NSTextField *text;
}
- (void)changeConfig:(NSDictionary*)d;
@end
