//
//  OVBasePreferencesViewController.m
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/7/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import "OVBasePreferencesViewController.h"

@implementation OVBasePreferencesViewController
@synthesize moduleIdentifier = _moduleIdentifier;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)dealloc
{
    [_moduleIdentifier release];
    [super dealloc];
}

- (void)synchronize
{
    CFPreferencesSynchronize((CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
}

- (BOOL)boolValueForKey:(NSString *)key
{
    if (!self.moduleIdentifier) {
        return NO;
    }

    CFTypeRef valueRef = CFPreferencesCopyValue((CFStringRef)key, (CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
    id value = [NSMakeCollectable(valueRef) autorelease];
    if (![value isKindOfClass:[NSNumber class]]) {
        return NO;
    }

    return [value boolValue];
}

- (void)setBoolValue:(BOOL)value forKey:(NSString *)key
{
    CFPreferencesSetValue((CFStringRef)key, (value ? kCFBooleanTrue : kCFBooleanFalse), (CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
    CFPreferencesSynchronize((CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
}
@end
