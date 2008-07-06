//
//  OVModuleController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVModuleController.h"


@implementation OVModuleController

- (id)copyWithZone:(NSZone *)zone
{
    id copy = [[[self class] allocWithZone: zone] initWithIdentifier:_identifier localizedName:_localizedName dictionary:_dictionary enabled:_enabled delegate:_delegate];
    return copy;
}

// - (id)initWithIdentifier:(NSString *)identifier localizedName:(NSString *)localizedName dictionary:(NSDictionary *)dictionary delegate:(id)delegate
- (id)initWithIdentifier:(NSString *)identifier localizedName:(NSString *)localizedName dictionary:(NSDictionary *)dictionary enabled:(BOOL)enabled delegate:(id)delegate;

{
	self = [super init];
	if (self != nil) {
		_identifier = [identifier retain];
		_localizedName = [localizedName retain];
		_dictionary = [NSMutableDictionary dictionaryWithDictionary:dictionary];
		[_dictionary retain];
		_enabled = enabled;
		_delegate = [delegate retain];
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
