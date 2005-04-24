/* MyDelegate */

#import <Cocoa/Cocoa.h>

@interface MyDelegate : NSObject
{
    IBOutlet id console;
    FILE *log;
    NSString *monitorMessage;
    NSFont *font;
}
- (IBAction)clearButton:(id)sender;
- (void)awakeFromNib;
- (void)monitor;
- (IBAction)relaunch:(id)sender;
@end
