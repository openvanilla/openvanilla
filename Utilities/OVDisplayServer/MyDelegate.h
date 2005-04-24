/* MyDelegate */

#import <Cocoa/Cocoa.h>

@interface MyDelegate : NSObject
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
