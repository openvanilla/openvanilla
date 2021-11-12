//
// OVAFAssociatedPhrasesPreferencesViewController.m
//
// Copyright (c) 2004-2017 The OpenVanilla Project Authors.
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

#import "OVAFAssociatedPhrasesPreferencesViewController.h"

static NSString *const kModuleIdentifier = @"org.openvanilla.OVAFAssociatedPhrases";

@implementation OVAFAssociatedPhrasesPreferencesViewController

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        self.moduleIdentifier = kModuleIdentifier;
        _defaultSelectionKeys = @[@"!@#$%^&*()", @"!@#$%^&*(", @"1234567890", @"123456789"];
        _defaultSelectionKeyTitles = @[@"Shift-1 ~ Shift-0", @"Shift-1 ~ Shift-9", @"1234567890", @"123456789"];
    }
    return self;
}

- (IBAction)updateField:(id)sender
{
    [self setBoolValue:([self.fieldContinuousAssociation state] == NSOnState) forKey:@"ContinuousAssociation"];

    NSInteger selectedIndex = [self.fieldSelectionKeys indexOfSelectedItem];
    if (selectedIndex == -1) {
        selectedIndex = 0;
    }

    NSString *newSelectionKeys;
    newSelectionKeys = (selectedIndex < _defaultSelectionKeyTitles.count ? _defaultSelectionKeys : _defaultSelectionKeyTitles)[selectedIndex];
    [self setStringValue:newSelectionKeys forKey:@"SelectionKeys"];
}

- (void)setStateForButton:(NSButton *)button forKey:(NSString *)key
{
    button.state = [self boolValueForKey:key] ? NSOnState : NSOffState;
}

- (void)loadPreferences
{
    [super loadPreferences];

    [self.fieldSelectionKeys removeAllItems];

    NSString *selectionKeys = [self stringValueForKey:@"SelectionKeys"];
    NSInteger selectedIndex = 0;

    if ([selectionKeys length] > 0) {
        selectedIndex = [_defaultSelectionKeys indexOfObject:selectionKeys];
    }

    if (selectedIndex == NSNotFound) {
        NSArray *titles = [_defaultSelectionKeyTitles arrayByAddingObject:selectionKeys];
        [self.fieldSelectionKeys addItemsWithTitles:titles];
        [self.fieldSelectionKeys selectItemAtIndex:[titles count] - 1];
    }
    else {
        [self.fieldSelectionKeys addItemsWithTitles:_defaultSelectionKeyTitles];
        [self.fieldSelectionKeys selectItemAtIndex:selectedIndex];
    }

    [self setStateForButton:self.fieldContinuousAssociation forKey:@"ContinuousAssociation"];
}
@end
