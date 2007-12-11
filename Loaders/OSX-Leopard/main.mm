//
//  main.m
//  LeopardVanilla main
//
//  Copyright © The OpenVanilla Project 2007. All rights reserved.
//

#import "LeopardVanilla.h"
#import "CVLoader.h"
#import "OVDisplayServer.h"

IMKServer *_inputMethodServer = nil;
MenuRef _sharedCarbonMenu;
CVLoader *_sharedLoader = nil;
NSLock *_sharedLock = nil;

@interface LVInitializer : NSObject
- (void)run:(id)server;
@end

@implementation LVInitializer
- (void)run:(id)server
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];

	if (server) ;

	[_sharedLock lock];
	CVLoader *loader = new CVLoader;
	loader->init(_sharedCarbonMenu);
	_sharedLoader = loader;
	[_sharedLock unlock];
	
	[pool drain];
}
@end

int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];

    if (argc > 1 && !strcasecmp(argv[1], "reload")) {
        NSApplicationLoad();
        
        id ovdspsrvr = [[NSConnection rootProxyForConnectionWithRegisteredName:OVDSPSRVR_NAME host:nil] retain];
        
        if (!ovdspsrvr)
            return 1;

        NSLog(@"quitting LeopardVanilla");

        [ovdspsrvr setProtocolForProxy:@protocol(OVDisplayServer)];
        [ovdspsrvr quitLoader];
        
        sleep(3);
        
        return 0;
    }
    	
	_sharedLock = [[NSLock alloc] init];
	_inputMethodServer = [[IMKServer alloc] initWithName:LEOPARD_VANILLA_CONNECTION_NAME bundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];
	
	if (_inputMethodServer) {
		NSLog(@"input method server initialized");
	}
	else {
		NSLog(@"input method server init failed!");
	}
	
	
	[NSBundle loadNibNamed:@"MainMenu" owner:[NSApplication sharedApplication]];

	// create the shared menu
	OSErr status = CreateNewMenu(0, 0, &_sharedCarbonMenu);
	if (status) ;
	
	id initializer = [LVInitializer new];
	
	[NSThread detachNewThreadSelector:@selector(run:) toTarget:initializer withObject:_inputMethodServer];
	
	[[NSApplication sharedApplication] run];
	
	[initializer release];
	[_inputMethodServer release];
	[_sharedLock release];
	[pool drain];
	return 0;
}
