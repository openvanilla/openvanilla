/* VXInfoBoxController */

#import <Cocoa/Cocoa.h>

@interface VXInfoBoxController : NSWindowController
{
    IBOutlet id text;
}
-(void)clearContent;
-(void)update;
-(void)append: (const char*)s;
@end
