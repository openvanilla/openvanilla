/* QueryDelegate */

#import <Cocoa/Cocoa.h>

@interface QueryDelegate : NSObject
{
    IBOutlet id progressIndicator;
    IBOutlet id queryButton;
    IBOutlet id textField;
    IBOutlet id webView;
}
- (IBAction)query:(id)sender;
@end
