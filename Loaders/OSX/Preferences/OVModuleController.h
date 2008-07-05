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
	id _delegate;
}

- (id)initWithIdentifier:(NSString *)identifier localizedName:(NSString *)localizedName dictionary:(NSDictionary *)dictionary delegate:(id)delegate;
- (void)loadNib;
- (NSString *)identifer;
- (NSString *)localizedName;
- (void)setDictionary:(NSDictionary *)dictionary;
- (NSDictionary *)dictionary;
- (void)update;
- (void)updateAndWrite;
- (void)setValue:(id)value forKey:(NSString *)key;
- (void)setDelegate:(id)delegate;
- (id)delegate;
- (NSView *)view;

@end
