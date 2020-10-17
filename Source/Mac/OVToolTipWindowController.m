//
// OVToolTipWindowController.m
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import "OVToolTipWindowController.h"

@implementation OVToolTipWindowController
- (id)init
{
    NSRect contentRect = NSZeroRect;
    NSUInteger styleMask = NSBorderlessWindowMask | NSNonactivatingPanelMask;

    NSPanel *panel = [[[NSPanel alloc] initWithContentRect:contentRect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO] autorelease];
    [panel setLevel:CGShieldingWindowLevel() + 1];
    [panel setHasShadow:YES];

    self = [self initWithWindow:panel];
    if (self) {
        _toolTip = [[NSTextField alloc] initWithFrame:contentRect];
        [_toolTip setEditable:NO];
        [_toolTip setSelectable:NO];
        [_toolTip setTextColor:[NSColor blackColor]];
        [_toolTip setBackgroundColor:[NSColor yellowColor]];
        [_toolTip setBezeled:NO];
        [_toolTip setFont:[NSFont systemFontOfSize:14.0]];
        [[panel contentView] addSubview:_toolTip];
    }  

    return self;
}
- (void)dealloc
{
	[_toolTip release];
	[super dealloc];
}

- (void)setToolTipText:(NSString *)text atOrigin:(NSPoint)origin fromTopLeft:(BOOL)topLeft
{
    [_toolTip setStringValue:text];
    [_toolTip sizeToFit];
    NSRect frameRect;
    frameRect.origin = origin;
    frameRect.size = [_toolTip bounds].size;

    if (topLeft) {
        frameRect.origin.y -= frameRect.size.height;
    }

    [[self window] setFrame:frameRect display:YES];
}
@end
