#import "VXInfoBoxController.h"

@implementation VXInfoBoxController
-(void)clearContent
{
}
-(void)update
{
    fprintf (stderr, "updating content rect!\n");
    NSRect r=[[self window] frame];
    r=[[self window] contentRectForFrameRect: r];
    [text setFrame: r];
    [text display];
}
-(void)append: (const char*)s
{
    fprintf (stderr, "updating content rect!\n");
    NSPoint p={0,0};
    [text setFrameOrigin:p];
    [text setBoundsOrigin:p];

    NSTextContainer *c=[text textContainer];
    

    [text display];

    [text insertText:[NSString stringWithUTF8String: s]];
/*    NSRect r=[text frame];
    r.size=[[self window] frameRectForContentRect: r].size;
    r.origin=[[self window] frame].origin;
    [[self window] setFrame:r display:YES]; */
}
@end
