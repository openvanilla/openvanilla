/* KPController */

#import <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>

@interface KPController : NSWindowController
{
    OVBuffer *buf;
}
- (void)awakeFromNib;
- (void)setBuffer:(OVBuffer*)b;
- (IBAction)sendKey:(id)sender;
@end
