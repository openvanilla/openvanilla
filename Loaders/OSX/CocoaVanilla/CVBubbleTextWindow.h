//
//  CVBubbleTextWindow.h
//  NewDisplayServer
//

#import <Cocoa/Cocoa.h>

@interface CVBubbleTextWindow : NSWindow {
	NSPoint _initialLocation;
	IBOutlet id _textView;
	BOOL _useWindowAnimation;
}
- (void)setText:(NSString *)newText;
- (void)setBubbleBackgroundColor:(NSColor *)color;
- (void)setTextColor:(NSColor *)color;
- (void)setFont:(NSFont *)font;
- (void)setUseWindowAnimation:(BOOL)animation;
- (void)updateWindowSize;
@end
