/* OVPrefDelegate */

#import <Cocoa/Cocoa.h>

@interface OVPrefDelegate : NSObject
{
    IBOutlet id loading;
    IBOutlet id progress;
}
- (void)startRead;
@end
