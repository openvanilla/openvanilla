#import "VXInfoBoxController.h"

@implementation VXInfoBoxController
-(void)clearContent
{
}
-(void)update
{
}
-(void)append: (const char*)s
{
    [text insertText:[NSString stringWithUTF8String: s]];
}
@end
