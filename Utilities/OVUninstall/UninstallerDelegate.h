/* UninstallerDelegate */

#import <Cocoa/Cocoa.h>

@interface UninstallerDelegate : NSObject
{
    IBOutlet id listview;
    IBOutlet id resetCheckbox;
    IBOutlet id window;	
	
	NSMutableArray *versions;
}
- (IBAction)uninstall:(id)sender;
@end
