//
//  OVIMTibetan.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVIMTibetanController.h"


@implementation OVIMTibetanController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVIMTibetanController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}

- (void)awakeFromNib
{
	NSMutableDictionary *d = [NSMutableDictionary dictionary];
	[d setValue:[NSNumber numberWithInt:0]	forKey:@"keyboardLayout"];	
	[d addEntriesFromDictionary:[self dictionary]];	
	[u_keyboardLayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout"] intValue]];	
	[self setDictionary:d];
	[self update];
}

- (IBAction)changePreference:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[u_keyboardLayout indexOfSelectedItem]] forKey:@"keyboardLayout"];
	[self updateAndWrite];
}

@end
