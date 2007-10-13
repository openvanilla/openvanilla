// CVPreviewView.mm: Preview view for color/font settings
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#import "CVPreviewView.h"
#import "NSDictionaryExtension.h"

@implementation CVPreviewView
- (void)awakeFromNib {
    NSLog(@"view awake from nib!");
}

- (id)initWithFrame:(NSRect)frameRect
{
	if ((self = [super initWithFrame:frameRect]) != nil) {
		_text = [[CVBubbleTextView alloc] initWithFrame:NSMakeRect(0, 0, 200, 50)];
        [self addSubview:_text];
	}	
	return self;
}

- (void)drawRect:(NSRect)rect
{
    [[NSColor whiteColor] set];
    NSRectFill(rect);
	
	NSShadow *dropShadow = [[[NSShadow alloc] init] autorelease];
	[dropShadow setShadowColor:[[NSColor lightGrayColor] colorWithAlphaComponent:0.1]];
	[dropShadow setShadowBlurRadius:20];
	[dropShadow setShadowOffset:NSMakeSize(0,-5)];

	[NSGraphicsContext saveGraphicsState];	
	[dropShadow set];	
	NSRectFill([_text frame]);
	[NSGraphicsContext restoreGraphicsState];	
} 

- (void)changeConfig:(NSDictionary*)d {
    float alpha=1.0;	
	NSColor* fc=[[d valueForKey:@"foreground" default:@"1.0 1.0 1.0"] colorByString];
	NSColor* bc=[[d valueForKey:@"background" default:@"0.0 0.0 0.0"] colorByString];	
	NSString* font=[d valueForKey:@"font" default:@"Lucida Grande"];
	float s=[[d valueForKey:@"size" default:@"18"] floatValue];
	
    [_text setFont:[NSFont fontWithName:font size:s]];
	[_text setTextColor:[fc colorWithAlphaComponent:alpha]]; 	
	[_text setBackgroundColor:[bc colorWithAlphaComponent:alpha]];	
	[_text setText:@"OpenVanilla"];	
	
    NSRect r=[_text boundingRect];	
    NSRect vf=[self frame];
    [_text setFrameOrigin:NSMakePoint((vf.size.width-r.size.width)/2, (vf.size.height-r.size.height)/2)];	
	[_text setFrameSize:r.size];	
	[self setNeedsDisplay:TRUE];	
}

@end
