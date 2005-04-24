/* MyDelegate */

#import <Cocoa/Cocoa.h>

@interface MyDelegate : NSObject
{
    IBOutlet id text;
    IBOutlet id text2;
    IBOutlet id window;
	FILE *o;
    IBOutlet id window2;
}
- (void)evil;
- (void)awakeFromNib;
- (IBAction)adjustWindowLevel:(id)sender;
@end
