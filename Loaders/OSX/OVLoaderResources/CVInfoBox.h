// CVInfoBox.h

#ifndef __CVInfoBox_h
#define __CVInfoBox_h

#import <Cocoa/Cocoa.h>

@interface CVInfoBox : NSWindowController
{
    IBOutlet id text;
    NSMutableString *str;
    Point pos;
    BOOL onscreen;
}
- (void)awakeFromNib;
- (void)dealloc;
- (void)clear;
- (void)update;
- (void)append:(NSString*)s;
- (void)setPosition:(Point)p;
- (void)show;
- (void)hide;
- (BOOL)onScreen;
- (int)height;
@end

#endif
