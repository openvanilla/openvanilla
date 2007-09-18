//
//  CVBubbleTextView.h
//  NewDisplayServer
//
//  Created by Lukhnos D. Liu on 9/19/2007.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface CVBubbleTextView : NSView {
	NSMutableAttributedString *_attributedString;
	NSBezierPath *_outerBezelPath;
	NSBezierPath *_innerBezelPath;	
	NSColor *_backgroundToColor;
	NSColor *_backgroundFromColor;
	NSColor *_textColor;
	NSFont *_font;
	NSRect _boundingRect;
	float _padding;
	float _radius;
}
- (void)setText:(NSString *)newText;
- (void)setBackgroundColor:(NSColor *)color;
- (void)setTextColor:(NSColor *)color;
- (void)setFont:(NSFont *)font;
- (NSRect)boundingRect;
@end
