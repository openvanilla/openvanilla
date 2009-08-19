//
//  OVIMTibetanController.m
//  OVPreference

#import "OVIMTibetanController.h"


@implementation OVIMTibetanController

- (void)setDictinary:(NSDictionary *)dictionary
{
	[super setDictinary:dictionary];
	if (![[self dictionary] objectForKey:@"keyboardLayout"]) {
		[[self dictionary] setValue:[NSNumber numberWithInt:0] forKey:@"keyboardLayout"];
	}	
	[_keyboardLayout selectItemAtIndex:[[[self dictionary] valueForKey:@"keyboardLayout"] intValue]];
	[[self dictionary] addObserver:self forKeyPath:@"keyboardLayout" options:NSKeyValueObservingOptionNew context:NULL];
	
}

@end
