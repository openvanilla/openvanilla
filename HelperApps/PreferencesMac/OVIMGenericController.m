//
//  OVIMGenericController.m
//  OpenVanilla


#import "OVIMGenericController.h"


@implementation OVIMGenericController

- (IBAction)changeMaxKeySequenceLengthAction:(id)sender
{
	NSInteger i =  [sender intValue];
	[_maxKeySequenceLengthTextField setStringValue:[NSString stringWithFormat:@"%d", i]];
	[_stepper setIntValue:i];
	[self setValue:[[NSNumber numberWithInt:i] stringValue] forKey:@"maxKeySequenceLength"];
}

- (void)setDictinary:(NSDictionary *)dictionary
{
	BOOL optionChanged = NO;
	
	[super setDictinary:dictionary];
	NSLog(@"dictionary:%@", [[self dictionary] description]);
	if (![[self dictionary] valueForKey:@"autoCompose"]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:@"autoCompose"];
		optionChanged = YES;
	}
	[_autoComposeCheckBox setIntValue:[[[self dictionary] valueForKey:@"hitMaxAndCompose"] intValue]];

	if (![[self dictionary] valueForKey:@"hitMaxAndCompose"]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:@"hitMaxAndCompose"];
		optionChanged = YES;
	}
	[_hitMaxAndComposeCheckBox setIntValue:[[[self dictionary] valueForKey:@"hitMaxAndCompose"] intValue]];
	
	if (![[self dictionary] valueForKey:@"shiftSelectionKey"]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:@"shiftSelectionKey"];
		optionChanged = YES;
	}
	[_shiftSelectionKeyCheckBox setIntValue:[[[self dictionary] valueForKey:@"shiftSelectionKey"] intValue]];
	
	if (![[self dictionary] valueForKey:@"warningBeep"]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:@"warningBeep"];
		optionChanged = YES;
	}
	[_warningBeepCheckBox setIntValue:[[[self dictionary] valueForKey:@"warningBeep"] intValue]];
	
	if (![[self dictionary] valueForKey:@"maxKeySequenceLength"]) {
		[self setValue:[[NSNumber numberWithInt:5] stringValue] forKey:@"maxKeySequenceLength"];
		optionChanged = YES;
	}
	id length = [[self dictionary] valueForKey:@"maxKeySequenceLength"];
	if ([length isKindOfClass:[NSNumber class]]) {
		[_maxKeySequenceLengthTextField setStringValue:[length stringValue]];
		[_stepper setIntValue:[length intValue]];
	}
	else if ([length isKindOfClass:[NSString class]]) {
		[_maxKeySequenceLengthTextField setStringValue:length];
		[_stepper setIntValue:[length intValue]];
	}
	
	if (![[self dictionary] valueForKey:@"matchOneChar"]) {
		[self setValue:@"" forKey:@"matchOneChar"];
		optionChanged = YES;
	}
	[_matchOneCharTextField setStringValue:[[self dictionary] valueForKey:@"matchOneChar"]];	
	
	if (![[self dictionary] valueForKey:@"matchZeroOrMoreChar"]) {
		[self setValue:@"" forKey:@"matchZeroOrMoreChar"];
		optionChanged = YES;
	}
	[_matchZeroOrMoreCharTextField setStringValue:[[self dictionary] valueForKey:@"matchZeroOrMoreChar"]];
	
	if (optionChanged) {
		[self writeSetting];
	}
	
	[[self dictionary] addObserver:self forKeyPath:@"autoCompose" options:NSKeyValueObservingOptionNew context:NULL];
	[[self dictionary] addObserver:self forKeyPath:@"hitMaxAndCompose" options:NSKeyValueObservingOptionNew context:NULL];
	[[self dictionary] addObserver:self forKeyPath:@"matchOneChar" options:NSKeyValueObservingOptionNew context:NULL];
	[[self dictionary] addObserver:self forKeyPath:@"matchZeroOrMoreChar" options:NSKeyValueObservingOptionNew context:NULL];
	[[self dictionary] addObserver:self forKeyPath:@"maxKeySequenceLength" options:NSKeyValueObservingOptionNew context:NULL];
	[[self dictionary] addObserver:self forKeyPath:@"shiftSelectionKey" options:NSKeyValueObservingOptionNew context:NULL];
	[[self dictionary] addObserver:self forKeyPath:@"warningBeep" options:NSKeyValueObservingOptionNew context:NULL];

	
}

@end
