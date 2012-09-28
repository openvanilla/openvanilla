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

#import "OVBasePreferencesViewController.h"
#import <Carbon/Carbon.h>

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

+ (void)configureKeyboardLayoutList:(NSPopUpButton *)popUpButton selectedLayoutIdentifier:(NSString *)layoutIdentifier defaultIdentifier:(NSString *)defaultIdentifier
{
    CFArrayRef list = TISCreateInputSourceList(NULL, true);
    NSMenuItem *usKeyboardLayoutItem = nil;
    NSMenuItem *chosenItem = nil;

    [[popUpButton menu] removeAllItems];

    for (int i = 0; i < CFArrayGetCount(list); i++) {
        TISInputSourceRef source = (TISInputSourceRef)CFArrayGetValueAtIndex(list, i);

        CFStringRef category = TISGetInputSourceProperty(source, kTISPropertyInputSourceCategory);
        if (CFStringCompare(category, kTISCategoryKeyboardInputSource, 0) != kCFCompareEqualTo) {
            continue;
        }

        CFBooleanRef asciiCapable = TISGetInputSourceProperty(source, kTISPropertyInputSourceIsASCIICapable);
        if (!CFBooleanGetValue(asciiCapable)) {
            continue;
        }

        CFStringRef sourceType = TISGetInputSourceProperty(source, kTISPropertyInputSourceType);
        if (CFStringCompare(sourceType, kTISTypeKeyboardLayout, 0) != kCFCompareEqualTo) {
            continue;
        }

        NSString *sourceID = (NSString *)TISGetInputSourceProperty(source, kTISPropertyInputSourceID);
        NSString *localizedName = (NSString *)TISGetInputSourceProperty(source, kTISPropertyLocalizedName);

        NSMenuItem *item = [[[NSMenuItem alloc] init] autorelease];
        [item setTitle:localizedName];
        [item setRepresentedObject:sourceID];

        if ([sourceID isEqualToString:defaultIdentifier]) {
            usKeyboardLayoutItem = item;
        }

        // false if nil
        if ([layoutIdentifier isEqualToString:sourceID]) {
            chosenItem = item;
        }

        [[popUpButton menu] addItem:item];
    }

    if (chosenItem) {
        [popUpButton selectItem:chosenItem];
    }
    else if (usKeyboardLayoutItem) {
        [popUpButton selectItem:usKeyboardLayoutItem];
    }
    
    CFRelease(list);

}
- (void)synchronize
{
    CFPreferencesSynchronize((CFStringRef)self.moduleIdentifier, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
}

@end
