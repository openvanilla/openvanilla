//
//  NSObjectUpdateConfig.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSObject(UpdateConfig)
- (BOOL)updateConfigWithIdentifer:(NSString *)identifier dictionary:(NSDictionary *)dictionary;
- (void)writeConfigWithIdentifer:(NSString *)identifier dictionary:(NSDictionary *)dictionary;
- (void)updateOutputFilterOrder:(NSArray *)order;
- (void)addToExcludeList:(NSString *)identifier;
- (void)removeFromExcludeList:(NSString *)identifier;
@end