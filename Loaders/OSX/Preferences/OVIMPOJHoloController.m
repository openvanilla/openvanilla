//
//  OVIMPOJHoloController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVIMPOJHoloController.h"


@implementation OVIMPOJHoloController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVIMPOJHoloController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}

- (void)awakeFromNib
{
	NSMutableDictionary *d = [NSMutableDictionary dictionary];
    [d setValue:[NSNumber numberWithInt:0] forKey:@"ASCIIOutput"];
	[d setValue:[NSNumber numberWithInt:0] forKey:@"fullPOJOutput"];
	[d setValue:[NSNumber numberWithInt:0]	forKey:@"keyboardLayout"];
	[d setValue:[NSNumber numberWithInt:1] forKey:@"warningBeep"];
	
	[d addEntriesFromDictionary:[self dictionary]];
	
	[u_asciiOutput setIntValue:[[d valueForKey:@"ASCIIOutput"] intValue]];
	[u_fullPOJOutput setIntValue:[[d valueForKey:@"fullPOJOutput"] intValue]];
	[u_warningBeep setIntValue:[[d valueForKey:@"warningBeep"] intValue]];
	[u_keyboardLayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout"] intValue]];
		
	[self setDictionary:d];
	[self update];
}

- (IBAction)changePreference:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[u_asciiOutput intValue]] forKey:@"ASCIIOutput"];
	[self setValue:[NSNumber numberWithInt:[u_fullPOJOutput intValue]] forKey:@"fullPOJOutput"];
	[self setValue:[NSNumber numberWithInt:[u_warningBeep intValue]] forKey:@"warningBeep"];
	[self setValue:[NSNumber numberWithInt:[u_keyboardLayout indexOfSelectedItem]] forKey:@"keyboardLayout"];
	[self updateAndWrite];
}
	
	 
@end
