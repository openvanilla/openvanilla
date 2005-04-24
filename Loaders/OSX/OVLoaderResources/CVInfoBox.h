// CVInfoBox.h

#ifndef __CVInfoBox_h
#define __CVInfoBox_h

#import <Cocoa/Cocoa.h>

struct CVInfoBoxState {
    CVInfoBoxState(NSString *s, Point p, BOOL o);
    ~CVInfoBoxState();
    NSString *str;
    Point pos;
    BOOL onscreen;
};

@interface CVInfoBox : NSWindowController
{
    IBOutlet id text;
    NSMutableString *str;
	NSString *name;
    Point pos;
    BOOL onscreen;
    NSTimer *timer;
    float alpha;
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
- (CVInfoBoxState*)saveState;       // return a newed CVInfoBoxState object
- (void)setName:(NSString*)n;
- (void)restoreState:(CVInfoBoxState*)s;
- (void)fade;
- (void)stopTimer;
- (void)fadeStart;
- (void)fadeWork;
@end

#endif
