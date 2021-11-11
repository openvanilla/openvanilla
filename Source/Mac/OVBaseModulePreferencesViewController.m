//
// OVBasePreferencesViewController.m
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import "OVBaseModulePreferencesViewController.h"

@implementation OVBaseModulePreferencesViewController
- (BOOL)boolValueForKey:(NSString *)key
{
    if (!self.moduleIdentifier) {
        return NO;
    }

    CFTypeRef valueRef = CFPreferencesCopyValue((CFStringRef)key, (CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
    id value = CFBridgingRelease(valueRef);
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

- (NSString *)stringValueForKey:(NSString *)key
{
    if (!self.moduleIdentifier) {
        return nil;
    }

    CFTypeRef valueRef = CFPreferencesCopyValue((CFStringRef)key, (CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
    id value = CFBridgingRelease(valueRef);
    if (![value isKindOfClass:[NSString class]]) {
        return nil;
    }

    return value;
}

- (void)setStringValue:(NSString *)value forKey:(NSString *)key
{
    CFPreferencesSetValue((CFStringRef)key, (CFStringRef)value, (CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
    CFPreferencesSynchronize((CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
}

- (NSUInteger)unsignedIntegerValueForKey:(NSString *)key
{
    if (!self.moduleIdentifier) {
        return NO;
    }

    CFTypeRef valueRef = CFPreferencesCopyValue((CFStringRef)key, (CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
    id value = CFBridgingRelease(valueRef);
    if (![value isKindOfClass:[NSNumber class]]) {
        return NO;
    }

    return [value unsignedIntegerValue];
}

- (void)setUnsignedIntegerValue:(NSUInteger)value forKey:(NSString *)key
{
    CFPreferencesSetValue((CFStringRef)key, (CFNumberRef)[NSNumber numberWithUnsignedInteger:value], (CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
    CFPreferencesSynchronize((CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
}

@end
