/* Delegate */

#import <Cocoa/Cocoa.h>

@interface Delegate : NSObject
{
    IBOutlet id listview;
	
	NSMutableArray *items;	
}
- (IBAction)changeIcon:(id)sender;
@end
