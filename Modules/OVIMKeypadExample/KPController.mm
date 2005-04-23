#import "KPController.h"

@implementation KPController
- (void)awakeFromNib {
    NSLog(@"KPController: awake from nib!");
    buf=NULL;
}
- (void)setBuffer:(OVBuffer*)b {
    NSLog(@"setbuffer! b=%s", b?"TRUE":"FALSE");
    buf=b;
}
- (IBAction)sendKey:(id)sender
{
    NSLog (@"sendkey! value=%@\n", [sender title]);
    if (buf) {
        NSLog(@"we have buffer!");
        buf->append([[sender title] UTF8String])->send();
    }
}

@end
