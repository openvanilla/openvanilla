//
// MyController.m
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

#import "MyController.h"

@protocol CVDisplayServerPart
- (void)sendStringToCurrentComposingBuffer:(NSString *)string;
- (void)sendCharacterToCurrentComposingBuffer:(NSString *)string;
@end

NSPoint setWindowPosition(NSRect windowRect)
{	
	NSPoint point;
//	NSRect frame = [[NSScreen mainScreen] frame];
	point.y = windowRect.size.height + 50;
	point.x = 30;		
	return point;
}

@implementation MyController
- (void)awakeFromNib
{
	_displayServer = [[NSConnection rootProxyForConnectionWithRegisteredName:@"OVNewDisplayServer-0.8.0" host:nil] retain];
		
	if (_displayServer) {
		[_displayServer setProtocolForProxy:@protocol(CVDisplayServerPart)];
		
		[[self window] setLevel:NSScreenSaverWindowLevel];
	}
	else {
		NSLog(@"cannot find display server");
		[[NSApplication sharedApplication] terminate:self];
	}
	[[self window] setFrameTopLeftPoint:setWindowPosition([[self window] frame])];	
}
- (void)windowWillClose:(NSNotification *)notification
{
	NSLog(@"Screen keyboard terminated.");
	[[NSApplication sharedApplication] terminate:self];
}
- (IBAction)characterAction:(id)sender
{
	NSLog(@"sending character %@", [sender title]);
	[_displayServer sendCharacterToCurrentComposingBuffer:[sender title]];
}
- (IBAction)stringAction:(id)sender
{
	NSLog(@"sending composed string %@", [sender title]);
	[_displayServer sendStringToCurrentComposingBuffer:[sender title]];
}
@end
