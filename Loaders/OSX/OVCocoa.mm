#include <Cocoa/Cocoa.h>

extern "C" int EntryPoint()
{
	if (!NSApplicationLoad()) 
	{
		fprintf (stderr, "NSApplicationLoad() failed!\n");
		return 0;
	}
	
	id p=[NSAutoreleasePool new];
	
	NSString *f;
	NSBundle *b=[NSBundle mainBundle];
	
	f=[b localizedStringForKey: @"hello" value: nil table: nil];
	
	fprintf(stderr, "initialized result=%s\n", [f UTF8String]);
	
	return 1;
}
