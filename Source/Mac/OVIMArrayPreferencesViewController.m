//
// OVIMArrayPreferencesViewController.m
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

#import "OVIMArrayPreferencesViewController.h"

static NSString *const kArrayModuleIdentifier = @"org.openvanilla.OVIMArray";

@implementation OVIMArrayPreferencesViewController

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        self.moduleIdentifier = kArrayModuleIdentifier;
    }
    return self;
}

- (IBAction)updateField:(id)sender
{
    [self setBoolValue:([self.fieldAutoSP state] == NSOnState) forKey:@"SpecialCodePrompt"];
    [self setBoolValue:([self.fieldForceSP state] == NSOnState) forKey:@"QuickMode"];
}

- (void)setStateForButton:(NSButton *)button forKey:(NSString *)key
{
    [button setState:([self boolValueForKey:key] ? NSOnState : NSOffState)];
}

- (void)loadPreferences
{
    [super loadPreferences];
    [self setStateForButton:self.fieldAutoSP forKey:@"SpecialCodePrompt"];
    [self setStateForButton:self.fieldForceSP forKey:@"QuickMode"];
}
@end
