//
//  CVBubbleTextWindow.h
//  NewDisplayServer
//

#import <Cocoa/Cocoa.h>

@interface CVBubbleTextWindow : NSWindow {
	NSPoint _initialLocation;
	IBOutlet id _textView;
}
- (void)setText:(NSString *)newText;
- (void)setBubbleBackgroundColor:(NSColor *)color;
- (void)setTextColor:(NSColor *)color;
- (void)setFont:(NSFont *)font;
- (void)updateWindowSizeWithAnimation;
@end
