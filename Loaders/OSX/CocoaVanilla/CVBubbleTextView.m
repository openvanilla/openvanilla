//
//  CVBubbleTextView.m
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#import "CVBubbleTextView.h"
#import "NSBezierPathAdditions.h"
#import "CTGradient.h"

#define CVBubbleTextViewLineHeightThreshold	12.0

@implementation CVBubbleTextView
- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
		_attributedString = [NSMutableAttributedString new];
		_textColor = nil;
		_backgroundFromColor = nil;
		_backgroundToColor = nil;
		_font = [[NSFont fontWithName:@"Helvetica" size:CVBubbleTextViewLineHeightThreshold] retain];
		_outerBezelPath = nil;
		_innerBezelPath = nil;
		_boundingRect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
		_padding = 4.0;
		_radius = 4.0;
		
		[self setTextColor:[NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:1.0]];
		[self setBackgroundColor:[NSColor colorWithCalibratedRed:0.0 green:0.0 blue:0.5 alpha:1.0]];
		[self setText:@""];
    }
    return self;
}
- (void)dealloc
{
	[_backgroundFromColor release];
	[_backgroundToColor release];
	[_textColor release];
	[_font release];
	[_attributedString release];
	[super dealloc];
}
- (void)setText:(NSString *)newText
{
	[[_attributedString mutableString] setString:newText];

	NSShadow *s = [[[NSShadow alloc] init] autorelease];
	[s setShadowBlurRadius:(float)([_font pointSize] / 6)];
	[s setShadowColor:[NSColor colorWithCalibratedHue:[_textColor hueComponent]
										   saturation:[_textColor saturationComponent]
										   brightness:[_textColor brightnessComponent]
												alpha:0.5]];
	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:_font, NSFontAttributeName, _textColor, NSForegroundColorAttributeName, s, NSShadowAttributeName, nil];
	NSRange range = NSMakeRange(0, [[_attributedString mutableString] length]);
	[_attributedString setAttributes:attributes range:range];
	
	// TO DO: Find out the Cocoa graphics system's maximum allowed width/height
	NSRect textBound = [_attributedString boundingRectWithSize:NSMakeSize(10240.0, 10240.0) options:NSStringDrawingUsesLineFragmentOrigin];

	if (textBound.size.height < CVBubbleTextViewLineHeightThreshold) textBound.size.height = CVBubbleTextViewLineHeightThreshold;
	
	NSRect viewBound = textBound;
	viewBound.size.width += _padding * 2.0;
	viewBound.size.height += _padding * 2.0;
	
	NSBezierPath *outerPath;
	NSBezierPath *innerPath;
	
	outerPath = [NSBezierPath bezelPathWithRect:viewBound radius:_radius];
	
	NSRect innerBound = viewBound;
	
	// shrink the viewBound
	innerBound.origin.x += 1.5;
	innerBound.size.width -= 3.0;
	innerBound.size.height = _padding + CVBubbleTextViewLineHeightThreshold / 2.0;
	
	// move the innerBound's y up
	innerBound.origin.y = viewBound.size.height - innerBound.size.height - 2.0;
	
	_innerBound = innerBound;
	innerPath = [NSBezierPath bezelPathWithRect:innerBound radius:_radius / 2.0];
	
	NSBezierPath *tmp;
	tmp = _outerBezelPath;
	_outerBezelPath = [outerPath retain];
	[tmp release];
	
	tmp = _innerBezelPath;
	_innerBezelPath = [innerPath retain];
	[tmp release];
	
	_boundingRect = viewBound;
	[self setNeedsDisplay:YES];
}
- (void)setBackgroundColor:(NSColor *)color
{
	NSColor *fromColor;
	NSColor *toColor;
	NSColor *tmp;
	
	fromColor = color;
	
	float hue = [color hueComponent];
	float saturation = [color saturationComponent] / 2.0;
	float brightness = [color brightnessComponent];
	
	if (brightness < 0.83) {
		// use the brightest color of the given hue/saturation pair
		brightness = brightness * 1.2;
	}
	else {
		// otherwise we reduce the brightness, and let toColor use this brighter color
		fromColor = [NSColor colorWithCalibratedHue:hue saturation:saturation brightness:(brightness / 1.2) alpha:1.0];
	}
	
	toColor = [NSColor colorWithCalibratedHue:hue saturation:saturation brightness:brightness alpha:1.0];
	
	tmp = _backgroundFromColor;
	_backgroundFromColor = [fromColor retain];
	[tmp release];

	tmp = _backgroundToColor;
	_backgroundToColor = [toColor retain];
	[tmp release];	
}
- (void)setTextColor:(NSColor *)color
{
	NSColor *tmp = _textColor;
	_textColor = [color retain];
	[tmp release];
}
- (void)setFont:(NSFont *)font
{
	NSFont *tmp = _font;
	_font = [font retain];
	[tmp release];
}

- (NSRect)boundingRect;
{
	return _boundingRect;
}
- (void)drawRect:(NSRect)rect
{
	#if MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_4
		// gradient fill the outer bezel
		NSGradient *gradient;
		
		if (_outerBezelPath) {
			gradient = [[[NSGradient alloc] initWithStartingColor:_backgroundFromColor endingColor:_backgroundToColor] autorelease];
			[gradient drawInBezierPath:_outerBezelPath angle:270.0];
		}
		
		if (_innerBezelPath) {
			// then use the transparent white to fill the inner bezel
			NSColor *whiteColor1 = [NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:0.5];
			NSColor *whiteColor2 = [NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:0.0];
			gradient = [[[NSGradient alloc] initWithStartingColor:whiteColor1 endingColor:whiteColor2] autorelease];
			[gradient drawInBezierPath:_innerBezelPath angle:270.0];
		}
	#else 
		CTGradient *gradient;

		gradient = [CTGradient gradientWithBeginningColor:_backgroundFromColor endingColor:_backgroundToColor];
		[gradient fillRect:_boundingRect angle:90 radius: _radius];
		
		NSColor *whiteColor1 = [NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:0.5];
		NSColor *whiteColor2 = [NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:0.0];
		gradient = [CTGradient gradientWithBeginningColor:whiteColor1 endingColor:whiteColor2];
		[gradient fillRect:_innerBound angle:270 radius: _radius];
	#endif
	
	[_attributedString drawAtPoint:NSMakePoint(_padding, _padding)];
}
@end
