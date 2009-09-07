// 
// SnippetWindow.m
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
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

#import "SnippetWindow.h"

NSPoint setWindowPosition(NSRect windowRect)
{	
	NSPoint point;
	NSRect frame = [[NSScreen mainScreen] frame];
	point.y = windowRect.size.height + 50;
	point.x = frame.size.width - windowRect.size.width - 20;		
	return point;
}

NSSize setWindowSize(NSRect windowRect)
{
	NSRect frame = [[NSScreen mainScreen] frame];	
	float width = windowRect.size.width;
	float height = frame.size.height;
	NSSize size = NSMakeSize(width,height);
	return size;
}

@implementation SnippetWindow

- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag
{	
    NSWindow *window = [super initWithContentRect:contentRect styleMask: (NSBorderlessWindowMask | NSUtilityWindowMask | NSNonactivatingPanelMask | NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask ) backing:NSBackingStoreBuffered defer:NO]; 
    [window setLevel:NSScreenSaverWindowLevel];
    [window setAlphaValue:1.0];
    [window setOpaque:NO];
    [window setHasShadow:YES];	
	[window setMovableByWindowBackground:YES];
    [window setFrameTopLeftPoint:setWindowPosition([window frame])];	
	[window setMaxSize:setWindowSize([window frame])];	
	[[window toolbar] setDisplayMode:NSToolbarDisplayModeIconOnly];
	[[window toolbar] setSizeMode: NSToolbarSizeModeSmall];	
	[[window toolbar] setVisible: NO];
	
	_isZoomed = NO;
	_soundEffet = [NSSound soundNamed:@"keydown"];	
    return window;
}

- (NSTimeInterval)animationResizeTime:(NSRect)newWindowFrame {
	NSTimeInterval timeInterval = 0.3;
	return timeInterval;
}

- (void)saveSize {
	if(_currentToolbar) {
		_currentHeight = [super frame].size.height + 32;
	} else {
		_currentHeight = [super frame].size.height;	
	}
	if(_currentToolbar) {	
		_currentY = [super frame].origin.y -32;
	} else {
		_currentY = [super frame].origin.y;		
	}
	_currentWidth = [super frame].size.width;	
	_currentX = [super frame].origin.x;
}

- (void)zoom:(id)sender {
	NSPoint point;
	point.y = [super frame].origin.y;
	point.x = [super frame].origin.x;	
	if(_isZoomed == NO) {	
		if([drawer state] == 2) {
			_currentDrawer = YES;
			[drawer toggle:self];
		} else {
			_currentDrawer = NO;			
		}
		if([[super toolbar] isVisible]) {
			_currentToolbar = YES;
			[[super toolbar] setVisible:NO];
		} else {
			_currentToolbar = NO;			
		}
		[self saveSize];
		NSRect rect = NSMakeRect(point.x,point.y + [super frame].size.height - 36 ,[super frame].size.width,36);
		[super setFrame:rect display:YES animate:YES];
		[super setShowsResizeIndicator:NO];
		[_soundEffet play];		
		_isZoomed = YES;
	} else {	
		if(_currentToolbar == YES) {
			[[super toolbar] setVisible:YES];			
		}
		NSRect rect = NSMakeRect(_currentX,_currentY,_currentWidth,_currentHeight);
		[super setFrame:rect display:YES animate:YES];
		[_soundEffet play];	
		if(_currentDrawer == YES) {
			[drawer toggle:self];			
		}	
		[super setShowsResizeIndicator:YES];		
		_isZoomed = NO;			
	}
}

-(BOOL) isZoomed {
	return _isZoomed;
}

- (BOOL)canBecomeKeyWindow
{
    return YES;
}
- (BOOL)canBecomeMainWindow
{
    return NO;
}

@end
