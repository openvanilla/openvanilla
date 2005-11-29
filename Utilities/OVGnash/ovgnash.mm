// ovgnash.mm: a Growl-like utility making ObjC RPC call to OVDisplayServer
#import <Cocoa/Cocoa.h>
#import "NSStringExtension.h"
#import "OVDisplayServer.h"

id ConnectDisplayServer();
int RunGnash(id ds, int argc, char **argv);

int main(int argc, char *argv[]) {
	NSApplicationLoad();
	[NSAutoreleasePool new];
	
	id srvr=ConnectDisplayServer();
	if (!srvr) {
		fprintf(stderr, "ovgnash: cannot connect to OVDisplayServer, might not be running\n");
		return 1;
	}

	return RunGnash(srvr, argc, argv);
}

id ConnectDisplayServer() {
	id dspsrvr=[[NSConnection rootProxyForConnectionWithRegisteredName:@"OVDisplayServer" host:nil] retain];
	if (dspsrvr) [dspsrvr setProtocolForProxy:@protocol(OVDisplayServer)];
	return dspsrvr;
}

int RunGnash(id ds, int argc, char **argv) {
	if (argc < 2) {
		fprintf (stderr, "usage: ovgnarl message\n");
	}
	
	Point p={100, 100};
	NSMutableDictionary *cfg=[[NSMutableDictionary new] autorelease];
	[cfg setObject:@"1 1 1" forKey:@"foreground"];
	[cfg setObject:@"0.85" forKey:@"opacity"];
	[cfg setObject:@"0 0 0" forKey:@"background"];	// can be transparent or none
	[cfg setObject:@"" forKey:@"backgroundImage"];
	[cfg setObject:@"64" forKey:@"size"];
	[cfg setObject:@"Times" forKey:@"font"];
	
	NSString *msg=[NSString stringWithUTF8String:argv[1]];
	
	[ds setConfig:cfg];
	[ds notifyMessage:msg position:p];
	[ds notifyFade];
	
	return 0;
}
