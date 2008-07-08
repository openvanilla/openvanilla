// OVIMPhonetic.m : The controller for settings of the Traditional
// Phonetic Input Mehtod.
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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
		if (currKey == ' ')
			return NO;
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
