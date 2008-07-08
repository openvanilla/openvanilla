// OVIMPOJHoloController.m : The controller for settings of the Holo-POJ
// Input Mehtod.
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
    [d setValue:[[NSNumber numberWithInt:0] stringValue] forKey:@"ASCIIOutput"];
	[d setValue:[[NSNumber numberWithInt:0] stringValue] forKey:@"fullPOJOutput"];
	[d setValue:[[NSNumber numberWithInt:0] stringValue] forKey:@"keyboardLayout"];
	[d setValue:[[NSNumber numberWithInt:1] stringValue] forKey:@"warningBeep"];
	
	[d addEntriesFromDictionary:[self dictionary]];
	
	[u_asciiOutput setIntValue:[[d valueForKey:@"ASCIIOutput"] intValue]];
	[u_fullPOJOutput setIntValue:[[d valueForKey:@"fullPOJOutput"] intValue]];
	[u_warningBeep setIntValue:[[d valueForKey:@"warningBeep"] intValue]];
	[u_keyboardLayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout"] intValue]];
		
	[self setDictionary:d];
	[self update];
}

#pragma mark Interface Builder actions.

- (IBAction)changePreference:(id)sender
{
	[self setValue:[[NSNumber numberWithInt:[u_asciiOutput intValue]] stringValue] forKey:@"ASCIIOutput"];
	[self setValue:[[NSNumber numberWithInt:[u_fullPOJOutput intValue]] stringValue] forKey:@"fullPOJOutput"];
	[self setValue:[[NSNumber numberWithInt:[u_warningBeep intValue]] stringValue] forKey:@"warningBeep"];
	[self setValue:[[NSNumber numberWithInt:[u_keyboardLayout indexOfSelectedItem]] stringValue] forKey:@"keyboardLayout"];
	[self updateAndWrite];
}
	
	 
@end
