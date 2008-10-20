//
// LVTopMostWindow.mm
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

#import "LVTopMostWindow.h"

@implementation LVTopMostWindow
- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag
{	
    NSWindow *window = [super initWithContentRect:contentRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];
    [window setBackgroundColor:[NSColor clearColor]];
    [window setLevel:NSScreenSaverWindowLevel];
    [window setAlphaValue:1.0];
    [window setOpaque:NO];
    [window setHasShadow:YES];
    return window;
}
- (BOOL)canBecomeKeyWindow
{
    return YES;
}
- (BOOL)canBecomeMainWindow
{
    return NO;
}
- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint currentLocation;
	NSPoint newOrigin;
	NSRect screenFrame = [[NSScreen mainScreen] frame];
	NSRect windowFrame = [self frame];
	
    currentLocation = [self convertBaseToScreen:[self mouseLocationOutsideOfEventStream]];
    newOrigin.x = currentLocation.x - _initialLocation.x;
    newOrigin.y = currentLocation.y - _initialLocation.y;
    
    if ((newOrigin.y + windowFrame.size.height) > (screenFrame.origin.y + screenFrame.size.height)) {
		newOrigin.y=screenFrame.origin.y + (screenFrame.size.height-windowFrame.size.height);
	}
    
    [self setFrameOrigin:newOrigin];
}
- (void)mouseDown:(NSEvent *)theEvent
{
    NSRect windowFrame = [self frame];
	_initialLocation = [self convertBaseToScreen:[theEvent locationInWindow]];
	_initialLocation.x -= windowFrame.origin.x;
	_initialLocation.y -= windowFrame.origin.y;
}
@end
