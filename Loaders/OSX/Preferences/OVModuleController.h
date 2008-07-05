//
//  OVModuleController.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "NSObjectUpdateConfig.h"

@interface OVModuleController : NSObject {
	IBOutlet NSView *view;
	NSString *_identifier;
	NSString *_localizedName;
	NSMutableDictionary *_dictionary;
	BOOL _enabled;
	id _delegate;
}

- (id)initWithIdentifier:(NSString *)identifier localizedName:(NSString *)localizedName dictionary:(NSDictionary *)dictionary enabled:(BOOL)enabled delegate:(id)delegate;
- (void)loadNib;
- (NSString *)identifer;
- (NSString *)localizedName;
- (NSDictionary *)dictionary;
- (void)setDictionary:(NSDictionary *)dictionary;
- (BOOL)isEnabled;
- (void)setEnabled:(BOOL)enabled;
- (id)delegate;
- (void)setDelegate:(id)delegate;
- (void)setValue:(id)value forKey:(NSString *)key;
- (void)update;
- (void)updateAndWrite;
- (NSView *)view;

@end
