// OVModuleController.m : The controller for settings of OpenVanilla modules.
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

#import "OVModuleController.h"


@implementation OVModuleController

- (id)copyWithZone:(NSZone *)zone
{
    id copy = [[[self class] allocWithZone: zone] initWithIdentifier:_identifier localizedName:_localizedName dictionary:_dictionary enabled:_enabled delegate:_delegate shortcut:_shortcut];
    return copy;
}

- (id)initWithIdentifier:(NSString *)identifier localizedName:(NSString *)localizedName dictionary:(NSDictionary *)dictionary enabled:(BOOL)enabled delegate:(id)delegate shortcut:(NSString *)shortcut
{
	self = [super init];
	if (self != nil) {
		_identifier = [identifier retain];
		_localizedName = [localizedName retain];
		_dictionary = [[NSMutableDictionary dictionaryWithDictionary:dictionary] retain];
		_enabled = enabled;
		_delegate = [delegate retain];
		_shortcut = [shortcut retain];
		[self loadNib];
	}
	return self;
}
- (void)loadNib
{
	// This class does not load Nib file, but subclasses of this class do.
}
- (void)dealloc
{
	[_identifier release];
	[_localizedName release];
	[_dictionary release];	
	[_delegate release];
	[super dealloc];
}
- (NSView *)view
{
	if (view)
		return view;
	return nil;
}
- (NSString *)moduleIdentifer
{
	return _identifier;
}
- (NSString *)localizedName
{
	return _localizedName;
}
- (NSDictionary *)dictionary
{
	return _dictionary;
}
- (void)setDictionary:(NSDictionary *)dictionary
{
	if (!_dictionary) {
		_dictionary = [NSMutableDictionary new];
	}
	[_dictionary removeAllObjects];
	[_dictionary addEntriesFromDictionary:dictionary];
}
- (NSString *)shortcut
{
	return _shortcut;
}
- (void)setShortcut:(NSString *)shortcut
{
	id tmp = _shortcut;
	_shortcut = [shortcut retain];
	[tmp release];
}
- (void)setShortcut:(NSString *)shortcut fromSender:(id)sender
{
	id tmp = _shortcut;
	_shortcut = [shortcut retain];
	[tmp release];
	[_delegate updateShortcut:shortcut forModule:_identifier];
}
- (id)delegate
{
	return _delegate;
}
- (void)setDelegate:(id)delegate
{
	id tmp = _delegate;
	_delegate = [delegate retain];
	[tmp release];
}
- (BOOL)isEnabled
{
	return _enabled;
}
- (void)setEnabled:(BOOL)enabled
{
	_enabled = enabled;
}
- (void)setValue:(id)value forKey:(NSString *)key
{
	[_dictionary setValue:value forKey:key];
}
- (void)update
{
	[_delegate updateConfigWithIdentifer:_identifier dictionary:_dictionary];	
}
- (void)updateAndWrite
{
	[_delegate writeConfigWithIdentifer:_identifier dictionary:_dictionary];
}
@end
