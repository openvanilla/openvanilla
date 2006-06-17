/* UninstallerDelegate */

#import <Cocoa/Cocoa.h>

@interface UninstallerDelegate : NSObject
{
    IBOutlet id listview;
    IBOutlet id resetCheckbox;
	
	NSMutableArray *versions;
}
- (IBAction)uninstall:(id)sender;
@end
