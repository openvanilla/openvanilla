//
//  MainController+Toolbar.h
//  OVPreference

#import <Cocoa/Cocoa.h>
#import "MainController.h"

//#define WINDOW_TITLE_HEIGHT 78
#define WINDOW_TITLE_HEIGHT 22

@interface MainController(Toolbar)

- (void)initToolbar;
- (void)setActiveView:(NSView *)view animate:(BOOL)flag;

@end
