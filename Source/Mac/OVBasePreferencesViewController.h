//
//  OVBasePreferencesViewController.h
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/7/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface OVBasePreferencesViewController : NSViewController
{
    NSString *_moduleIdentifier;
}
- (void)synchronize;
- (BOOL)boolValueForKey:(NSString *)key;
- (void)setBoolValue:(BOOL)value forKey:(NSString *)key;
- (NSUInteger)unsignedIntegerValueForKey:(NSString *)key;
- (void)setUnsignedIntegerValue:(NSUInteger)value forKey:(NSString *)key;
@property (retain, nonatomic) NSString *moduleIdentifier;
@end
