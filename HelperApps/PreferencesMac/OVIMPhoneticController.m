//
//  OVIMPhoneticController.m
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

#import "OVIMPhoneticController.h"

@implementation PhoneticSelectionKeyTransformer

+ (Class)transformedValueClass
{
	return [NSString class];
}
+ (BOOL)allowsReverseTransformation
{
	return YES; 
}
- (void) dealloc
{
	_delegate = nil;
	[super dealloc];
}

- (id)transform:(id)value
{
	if (![value isKindOfClass:[NSString class]])
		return @"123456789";
	
	NSString *string = value;
	NSMutableString *s = [NSMutableString string];
	int i = 0;
	for (i = 0; i < [string length]; i++) {
		unichar theChar = [string characterAtIndex:i];
		if ((tolower(theChar) >= 'a' && tolower(theChar) <= 'z') || (theChar >= '0' && theChar <= '9')) {
			BOOL existing = NO;
			int j = 0;
			for (j = 0; j < [s length]; j++) {
				unichar existingChar = [s characterAtIndex:j];
				if (existingChar == tolower(theChar)) {
					existing = YES;
					break;
				}
			}
			if (!existing) {
				[s appendFormat:@"%c", tolower(theChar)];
			}
		}
	}
	
	if ([s length] < 9) {
		return @"123456789";
	}
	else if ([s length] > 9) {
		[s setString:[s substringToIndex:9]];
	}
	
	return s;
}
- (id)transformedValue:(id)value
{
	NSString *s = [self transform:value];
//	if (!s)
//		s = @"1234567890";
	[_delegate valueDidTransform:s];
	return s;
}
- (void)setDelegate:(id)delegate
{
	_delegate = delegate;
}
@end

@implementation OVIMPhoneticController

- (id)initWithIdentifier:(NSString *)identifier nibName:(NSString *)nibName
{
	self = [super init];
	if (self != nil) {
		PhoneticSelectionKeyTransformer *phoneticSelectionKeyTransformer = [[[PhoneticSelectionKeyTransformer alloc] init] autorelease];
		[NSValueTransformer setValueTransformer:phoneticSelectionKeyTransformer forName:@"selectKey"];
		[phoneticSelectionKeyTransformer setDelegate:self];

		BOOL loaded = [NSBundle loadNibNamed:nibName owner:self];
		NSAssert((loaded == YES), @"NIB did not load");	
		_identifier = [identifier retain];
		_dictionary = [[NSMutableDictionary alloc] init];
		_localizedName = nil;
	}
	return self;
}
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
- (void)valueDidTransform:(id)value
{
	[_selectionKeys setStringValue:value];
}


@end
