//
//  main.m
//  LeopardVanilla main
//
//  Copyright © The OpenVanilla Project 2007. All rights reserved.
//

#import "LeopardVanilla.h"
#import "CVLoader.h"

IMKServer *_inputMethodServer = nil;
MenuRef _sharedCarbonMenu;
CVLoader *_sharedLoader = nil;

int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
	
	_inputMethodServer = [[IMKServer alloc] initWithName:LEOPARD_VANILLA_CONNECTION_NAME bundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];
	[NSBundle loadNibNamed:@"MainMenu" owner:[NSApplication sharedApplication]];

	OSErr status = CreateNewMenu(0, 0, &_sharedCarbonMenu);
	NSLog(@"menu create = %d", status);
	
	_sharedLoader = new CVLoader;
	_sharedLoader->init(_sharedCarbonMenu);
	
	[[NSApplication sharedApplication] run];
	
	[pool drain];
	return 0;
}
