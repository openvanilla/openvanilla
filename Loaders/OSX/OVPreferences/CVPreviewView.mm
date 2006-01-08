// CVPreviewView.mm: Preview view for color/font settings
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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
#import "NSStringExtension.h"
#import "NSDictionaryExtension.h"
@implementation CVPreviewView
- (void)awakeFromNib {
    NSLog(@"view awake from nib!");
}

- (id)initWithFrame:(NSRect)frameRect
{
	if ((self = [super initWithFrame:frameRect]) != nil) {
        text=[[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 100, 50)];
        [text setEditable:NO];
        [text setSelectable:NO];
        [text setBordered:NO];
        [text setDrawsBackground:YES];
        [text setFrameOrigin:NSMakePoint(0,0)];
        [text setStringValue:@"OpenVanilla"];
        [self addSubview:text];
	}
	return self;
}

- (void)drawRect:(NSRect)rect
{
    [[NSColor whiteColor] set];
    NSRectFill(rect);
}

- (void)changeConfig:(NSDictionary*)d {
	// float alpha=[[d valueForKey:@"opacity" default:@"1.0"] floatValue];
    float alpha=1.0;
	NSColor *fc=[[d valueForKey:@"foreground" default:@"1.0 1.0 1.0"] colorByString];
	NSString *img=[d valueForKey:@"backgroundImage" default:@""];
	NSString *font=[d valueForKey:@"font" default:@"Lucida Grande"];
	float s=[[d valueForKey:@"size" default:@"18"] floatValue];

    if ([img length]) {
		NSImage *i=[[NSImage alloc] initByReferencingFile:img];
		if (i && [i isValid]) {
			[i autorelease];
			NSColor *c=[NSColor colorWithPatternImage:i];
			[text setBackgroundColor:[c colorWithAlphaComponent:alpha]];
		}
	}
    else {
        // no image, use color
        NSString *bcstr=[d valueForKey:@"background" default:@"1.0 1.0 1.0"];
        if ([bcstr isEqualToString:@"transparent"]) {
            [text setBackgroundColor:[NSColor clearColor]];
//           [text setBackgroundColor:[NSColor whiteColor]];
        }
        else {
            NSColor *bc=[bcstr colorByString];
            [text setBackgroundColor:[bc colorWithAlphaComponent:alpha]];
        }
    }
    
    [text setFont:[NSFont fontWithName:font size:s]];
	[text setTextColor:[fc colorWithAlphaComponent:alpha]];    
    [text sizeToFit];
    NSRect r=[text frame];
    NSRect vf=[self frame];
    [text setFrameOrigin:NSMakePoint((vf.size.width-r.size.width)/2, (vf.size.height-r.size.height)/2)];
    
    [self setNeedsDisplay:TRUE];
}

@end
