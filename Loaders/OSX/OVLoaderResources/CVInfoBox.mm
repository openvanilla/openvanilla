#import "CVInfoBox.h"

@implementation CVInfoBox
- (void)awakeFromNib {
    fprintf (stderr, "awake from nib!!!\n");
    onscreen=NO;
    pos=(NSPoint){0.0, 0.0};
    str=[NSMutableString new];
    
    // trial code
    [text setTextColor:[NSColor whiteColor]];
    [[self window] setBackgroundColor:[NSColor blueColor]];
    // [text setBackgroundColor:blue];
    // [text setDrawsBackground:TRUE];
    [text setFont:[NSFont fontWithName:@"Lucida Grande" size:18.0]];
    
    [self clear];
    [self append:@"OpenVanilla"];
    [self update];
}
- (void)dealloc {
    [str release];
    [super dealloc];
}
- (void)clear {
    NSRange r=(NSRange){0, [str length]};
    [str deleteCharactersInRange:r];
}
- (void)update {
    [text setStringValue:str];
    [text setFrameOrigin:(NSPoint){0, 0}];
    [text sizeToFit];
    NSRect r=[text frame];
    // r.size.width-=1.0;       // must do this to avoid background problem
    [[self window] setContentSize:r.size];
    [[self window] setFrameTopLeftPoint:pos];
}

- (void)append:(NSString*)s {
    [str appendString:s];
}
- (void)setPosition:(Point)p {
    pos.x=p.h;

    // get the first screen (main device) and calculate the y pos
    // we may have to re-do this
    NSArray *screens=[NSScreen screens];
    if ([screens count]) {
        NSRect screenrect=[[screens objectAtIndex:0] frame];
        float newy=NSMaxY(screenrect);
        pos.y=newy-p.v;
    }
}
- (void)show {
    if (!onscreen) [[self window] orderFront:self];
    onscreen=TRUE;
}
- (void)hide {
    if (onscreen) [[self window] orderOut:self];
    onscreen=FALSE;
}
- (BOOL)onScreen {
    return onscreen;
}
- (int)height {
    NSRect r=[[self window] frame];
    return (int)r.size.height;
}
@end
