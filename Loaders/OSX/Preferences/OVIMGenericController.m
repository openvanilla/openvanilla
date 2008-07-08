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
    [d setValue:[[NSNumber numberWithInt:0] stringValue] forKey:@"autoCompose"];
	[d setValue:[[NSNumber numberWithInt:0] stringValue] forKey:@"hitMaxAndCompose"];
	[d setValue:[[NSNumber numberWithInt:20] stringValue] forKey:@"maxKeySequenceLength"];
	[d setValue:[[NSNumber numberWithInt:0] stringValue] forKey:@"shiftSelectionKey"];
	[d setValue:[[NSNumber numberWithInt:1] stringValue] forKey:@"warningBeep"];
	[d setValue:@"" forKey:@"matchOneChar"];		
	[d setValue:@"" forKey:@"matchZeroOrMoreChar"];	
	
	[d addEntriesFromDictionary:[self dictionary]];
	
	[u_maxLength setIntValue:[[d valueForKey:@"maxKeySequenceLength"] intValue]];
	[u_maxLengthString setIntValue:[[d valueForKey:@"maxKeySequenceLength"] intValue]];	
	[u_shiftSelectionKey setIntValue:[[d valueForKey:@"shiftSelectionKey"] intValue]];
	[u_hitMaxAndCompose setIntValue:[[d valueForKey:@"hitMaxAndCompose"] intValue]];
	[u_autoCompose setIntValue:[[d valueForKey:@"autoCompose"] intValue]];
	[u_warningBeep setIntValue:[[d valueForKey:@"warningBeep"] intValue]];
	[u_matchOneChar setStringValue:[d valueForKey:@"matchOneChar"]];
	[u_matchZeroOrMoreChar setStringValue:[d valueForKey:@"matchZeroOrMoreChar"]];
	 
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
	[self setValue:[[NSNumber numberWithInt:[u_maxLength intValue]] stringValue] forKey:@"maxKeySequenceLength"];
	[self setValue:[[NSNumber numberWithInt:[u_shiftSelectionKey intValue]] stringValue] forKey:@"shiftSelectionKey"];
	[self setValue:[[NSNumber numberWithInt:[u_hitMaxAndCompose intValue]] stringValue] forKey:@"hitMaxAndCompose"];
	[self setValue:[[NSNumber numberWithInt:[u_autoCompose intValue]] stringValue] forKey:@"autoCompose"];
	[self setValue:[[NSNumber numberWithInt:[u_warningBeep intValue]] stringValue] forKey:@"warningBeep"];
	[self updateAndWrite];
}

- (IBAction)changeMatchOneChar:(id)sender
{
	if ([[u_matchOneChar stringValue] length] > 1) {
		unichar theChar;
		theChar = [[u_matchOneChar stringValue] characterAtIndex:0];
		[u_matchOneChar setStringValue:[NSString stringWithCharacters:&theChar length:1]];
	}	
	if ([[u_matchOneChar stringValue] isEqualToString:[u_matchZeroOrMoreChar stringValue]]) {
		NSString *MatchOneChar = [[self dictionary] valueForKey:@"matchOneChar"];
		[u_matchOneChar setStringValue:MatchOneChar];
		return;
	}
	else {
		[self setValue:[u_matchOneChar stringValue] forKey:@"matchOneChar"];
		[self updateAndWrite];
	}
}
- (IBAction)changeMatchZeroOrMoreChar:(id)sender
{
	if ([[u_matchZeroOrMoreChar stringValue] length] > 1) {
		unichar theChar;
		theChar = [[u_matchZeroOrMoreChar stringValue] characterAtIndex:0];
		[u_matchZeroOrMoreChar setStringValue:[NSString stringWithCharacters:&theChar length:1]];
	}	
	if ([[u_matchZeroOrMoreChar stringValue] isEqualToString:[u_matchOneChar stringValue]]) {
		NSString *MatchZeroOrMoreChar = [[self dictionary] valueForKey:@"matchZeroOrMoreChar"];
		[u_matchZeroOrMoreChar setStringValue:MatchZeroOrMoreChar];
		return;
	}
	else {
		[self setValue:[u_matchZeroOrMoreChar stringValue] forKey:@"matchZeroOrMoreChar"];		
		[self updateAndWrite];
	}	
}
@end
