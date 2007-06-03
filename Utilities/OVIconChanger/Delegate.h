/* Delegate */

#import <Cocoa/Cocoa.h>

@interface Delegate : NSObject
{
    IBOutlet id listview;
	
	NSMutableArray *items;	
}
- (IBAction)changeIcon:(id)sender;
- (IBAction)convertIcon:(id)sender;
- (IBAction)openIcon:(id)sender;
- (IBAction)openIconFolder:(id)sender;
- (IBAction)previewIcon:(id)sender;
- (IBAction)homepage:(id)sender;
@end
