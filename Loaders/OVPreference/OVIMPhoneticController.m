//
//  OVIMPhoneticController.m
//  OVPreference

#import "OVIMPhoneticController.h"


@implementation OVIMPhoneticController

- (void)setDictinary:(NSDictionary *)dictionary
{
	[super setDictinary:dictionary];

	if (![[self dictionary] objectForKey:@"keyboardLayout"]) {
		[[self dictionary] setValue:[NSNumber numberWithInt:0] forKey:@"keyboardLayout"];
	}	
	[_keyboardLayout selectItemAtIndex:[[[self dictionary] valueForKey:@"keyboardLayout"] intValue]];
	[[self dictionary] addObserver:self forKeyPath:@"keyboardLayout" options:NSKeyValueObservingOptionNew context:NULL];

	NSString *selectKey = [[self dictionary] valueForKey:@"selectKey"];
	if (!selectKey) {
		selectKey = @"123456789";
		[self setValue:selectKey forKey:@"selectKey"];
		[self writeSetting];
	}
	[_selectionKeys setStringValue:selectKey];
	[[self dictionary] addObserver:self forKeyPath:@"selectKey" options:NSKeyValueObservingOptionNew context:NULL];
	
}



@end
