//
// OVIMGenericController.m
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


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
