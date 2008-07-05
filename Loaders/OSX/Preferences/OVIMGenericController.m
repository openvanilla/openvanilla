//
//  OVIMGenericController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVIMGenericController.h"


@implementation OVIMGenericController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVIMGenericController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}

- (void)awakeFromNib
{
	NSMutableDictionary *d = [NSMutableDictionary dictionary];
    [d setValue:[NSNumber numberWithInt:0] forKey:@"autoCompose"];
	[d setValue:[NSNumber numberWithInt:0] forKey:@"hitMaxAndCompose"];
	[d setValue:[NSNumber numberWithInt:20]	forKey:@"maxKeySequenceLength"];
	[d setValue:[NSNumber numberWithInt:0] forKey:@"shiftSelectionKey"];
	[d setValue:[NSNumber numberWithInt:1] forKey:@"warningBeep"];
	
	[d addEntriesFromDictionary:[self dictionary]];
	
	[u_maxLength setIntValue:[[d valueForKey:@"maxKeySequenceLength"] intValue]];
	[u_maxLengthString setIntValue:[[d valueForKey:@"maxKeySequenceLength"] intValue]];	
	[u_shiftSelectionKey setIntValue:[[d valueForKey:@"shiftSelectionKey"] intValue]];
	[u_hitMaxAndCompose setIntValue:[[d valueForKey:@"hitMaxAndCompose"] intValue]];
	[u_autoCompose setIntValue:[[d valueForKey:@"autoCompose"] intValue]];
	[u_warningBeep setIntValue:[[d valueForKey:@"warningBeep"] intValue]];
	 
	[self setDictionary:d];
	[self update];
}

- (IBAction)changeMaxLength: (id)sender
{
	int i = [sender intValue];
	[u_maxLength setIntValue:i];
	[u_maxLengthString setIntValue:i];
	[self changePreference:sender];
}

- (IBAction)changePreference:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[u_maxLength intValue]] forKey:@"maxKeySequenceLength"];
	[self setValue:[NSNumber numberWithInt:[u_shiftSelectionKey intValue]] forKey:@"shiftSelectionKey"];
	[self setValue:[NSNumber numberWithInt:[u_hitMaxAndCompose intValue]] forKey:@"hitMaxAndCompose"];
	[self setValue:[NSNumber numberWithInt:[u_autoCompose intValue]] forKey:@"autoCompose"];
	[self setValue:[NSNumber numberWithInt:[u_warningBeep intValue]] forKey:@"warningBeep"];
	[self updateAndWrite];
}

@end
