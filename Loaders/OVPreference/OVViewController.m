//
//  OVViewController.m
//  OVPreference

#import "OVViewController.h"
#import "MainController.h"

@implementation OVViewController

- (void) dealloc
{
	[_dictionary release];
	[_identifier release];
	[_localizedName release];
	[super dealloc];
}

- (id)initWithIdentifier:(NSString *)identifier nibName:(NSString *)nibName
{
	self = [super init];
	if (self != nil) {
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
	[_dictionary setDictionary:dictionary];
}
- (NSDictionary *)dictionary
{
	return _dictionary;
}
- (void)setValue:(id)value forKey:(NSString *)key
{
	[_dictionary setValue:value forKey:key];
}
- (void)setIdentifier:(NSString *)identifier
{
	id tmp = _identifier;
	_identifier = [identifier retain];
	[tmp release];
}
- (NSString *)identifier
{
	return _identifier;
}
- (void)setLocalizedName:(NSString *)localizedName
{
	id tmp = _localizedName;
	_localizedName = [localizedName retain];
	[tmp release];
}
- (NSString *)localizedName
{
	return _localizedName;
}

- (NSView *)view
{
	return _view;
}
- (void)writeSetting
{
	MainController *controller = (MainController *)[NSApp delegate];
	[controller updateSettingWithIdentifier:_identifier settings:_dictionary];
}

#pragma mark -

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
	if ([object isKindOfClass:[NSDictionary class]]) {
		NSLog(@"%s", __PRETTY_FUNCTION__);
		NSLog(@"keyPath %@", keyPath);
		NSLog(@"object %@", [object description]);
		NSLog(@"change %@", [change description]);
		[self writeSetting];
	}
}

- (void)didChangeValueForKey:(NSString *)key
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, key);
}

@end
