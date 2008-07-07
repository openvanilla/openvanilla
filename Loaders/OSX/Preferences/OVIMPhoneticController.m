//
//  OVIMPhonetic.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVIMPhoneticController.h"


@implementation OVIMPhoneticController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVIMPhoneticController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}

- (void)awakeFromNib
{
	NSMutableDictionary *d = [NSMutableDictionary dictionary];
	[d setValue:[NSNumber numberWithInt:0]	forKey:@"keyboardLayout"];
	[d setValue:@"123456789" forKey:@"selectKey"];
	[d addEntriesFromDictionary:[self dictionary]];
	if ([[d valueForKey:@"selectKey"] length] != 9) {
		[d setValue:@"123456789" forKey:@"selectKey"];
	}
	
	[u_keyboardLayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout"] intValue]];
	[u_selectKey setStringValue:[d valueForKey:@"selectKey"]];
	
	[self setDictionary:d];
	[self update];
}
- (BOOL)validateSelectionKeys:(NSString *)selectionKeys
{
	int i;
	for (i = 0; i < [selectionKeys length] - 1; i++) {
		char currKey = [selectionKeys characterAtIndex:i];
		int j = i + 1;
		for (j = i + 1; i < [selectionKeys length]; i++) {
			char checkKey = [selectionKeys characterAtIndex:j];
			if (currKey == checkKey)
				return NO;
		}
	}	
	return YES;
}

- (IBAction)changeSelectKey:(id)sender
{
//	[self changePreference:self];
	NSString *newKey = [u_selectKey stringValue];
	if ([newKey length] != 9 || ![self validateSelectionKeys:newKey]) {
		newKey = @"123456789";
	}
	[self setValue:newKey forKey:@"selectKey"];
	[self updateAndWrite];	
	[u_selectKey setStringValue:newKey];		 
}
- (IBAction)changePreference:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[u_keyboardLayout indexOfSelectedItem]] forKey:@"keyboardLayout"];
	[self updateAndWrite];
}

@end
