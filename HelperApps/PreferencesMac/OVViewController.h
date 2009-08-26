//
//  OVViewController.h
//  OVPreference

#import <Cocoa/Cocoa.h>


@interface OVViewController : NSObject 
{
//@private
	IBOutlet NSView *_view;
	NSMutableDictionary *_dictionary;
	NSString *_identifier;
	NSString *_localizedName;
}

#pragma mark Getters and setters

- (id)initWithIdentifier:(NSString *)identifier nibName:(NSString *)nibName;

- (void)setDictinary:(NSDictionary *)dictionary;
- (NSDictionary *)dictionary;
- (void)setValue:(id)value forKey:(NSString *)key;
- (void)setIdentifier:(NSString *)identifier;
- (NSString *)identifier;
- (void)setLocalizedName:(NSString *)localizedName;
- (NSString *)localizedName;
- (NSView *)view;
- (void)writeSetting;

@end
