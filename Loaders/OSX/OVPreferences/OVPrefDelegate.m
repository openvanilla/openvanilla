#import "OVPrefDelegate.h"
#include <unistd.h>
@implementation OVPrefDelegate
- (void)awakeFromNib {
    [NSThread detachNewThreadSelector:@selector(startRead) toTarget:self withObject:nil];
}
- (void)startRead {
    id ap=[NSAutoreleasePool new];
    int c;
    [loading setStringValue:@"loading"];
    [progress startAnimation:self];
    for (c=0; c<10; c++) {
        NSLog([loading stringValue]);
        [loading setStringValue:[[loading stringValue] stringByAppendingString:@"."]];
        [loading setNeedsDisplay:YES];
        sleep(1);
    }
    [progress stopAnimation:self];
    [ap release];
    [NSThread exit];
}
@end
