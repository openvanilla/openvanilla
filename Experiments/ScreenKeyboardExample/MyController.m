//
//  MyController.m
//  ScreenKeyboardExample
//
//  Created by Lukhnos D. Liu on 9/19/2007.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "MyController.h"

@protocol CVDisplayServerPart
- (void)sendStringToCurrentComposingBuffer:(NSString *)string;
- (void)sendCharacterToCurrentComposingBuffer:(NSString *)string;
@end

@implementation MyController
- (void)awakeFromNib
{
	_displayServer = [[NSConnection rootProxyForConnectionWithRegisteredName:OVDSPSRVR_NAME host:nil] retain];	
	
	if (_displayServer) {
		[_displayServer setProtocolForProxy:@protocol(CVDisplayServerPart)];
		
		[[self window] setLevel:NSScreenSaverWindowLevel];
	}
	else {
		NSLog(@"cannot find display server");
		[[NSApplication sharedApplication] terminate:self];
	}
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
