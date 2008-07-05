//
//  OVModuleController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVModuleController.h"


@implementation OVModuleController

- (id)initWithIdentifier:(NSString *)identifier localizedName:(NSString *)localizedName dictionary:(NSDictionary *)dictionary delegate:(id)delegate
{
	self = [super init];
	if (self != nil) {
		_identifier = [identifier retain];
		_localizedName = [localizedName retain];
		_dictionary = [NSMutableDictionary dictionaryWithDictionary:dictionary];
		[_dictionary retain];
		_delegate = [delegate retain];
		[self loadNib];
	}
	return self;
}
- (void)loadNib
{
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
- (NSString *)identifer
{
	return _identifier;
}
- (NSString *)localizedName
{
	return _localizedName;
}
- (void)setDictionary:(NSDictionary *)dictionary
{
	if (!_dictionary) {
		_dictionary = [NSMutableDictionary new];
	}
	[_dictionary removeAllObjects];
	[_dictionary addEntriesFromDictionary:dictionary];
}
- (NSDictionary *)dictionary
{
	return _dictionary;
}
- (void)update
{
	[_delegate updateConfigWithIdentifer:_identifier dictionary:_dictionary];	
}
- (void)updateAndWrite
{
	[_delegate writeConfigWithIdentifer:_identifier dictionary:_dictionary];
}
- (void)setValue:(id)value forKey:(NSString *)key
{
	[_dictionary setValue:value forKey:key];
}
- (void)setDelegate:(id)delegate
{
	id tmp = _delegate;
	_delegate = delegate;
	[tmp release];
}
- (id)delegate
{
	return _delegate;
}


@end
