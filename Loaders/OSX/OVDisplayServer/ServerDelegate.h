/* MyDelegate */

#import <Cocoa/Cocoa.h>
#import "OVDisplayServer.h"

@interface ServerDelegate : NSObject <OVDisplayServer>
{
    IBOutlet id text;
    IBOutlet id text2;
    IBOutlet id window;
    IBOutlet id window2;
    IBOutlet id web;
	FILE *o;
}
- (void)evil;
- (void)awakeFromNib;
- (IBAction)adjustWindowLevel:(id)sender;
@end
