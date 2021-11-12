//
// OVGeneralPreferencesViewController.m
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

#import "OVGeneralPreferencesViewController.h"
#import "OVConstants.h"
#import "OVUpdateChecker.h"

@interface OVGeneralPreferencesViewController ()
- (void)handleUpdateCheckDidComplete:(NSNotification *)notification;
@end

@implementation OVGeneralPreferencesViewController

- (void)awakeFromNib
{
    // this is never deallocated (only on app exit), so no need to remove observer
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleUpdateCheckDidComplete:) name:OVUpdateCheckerDidFinishCheckingNotification object:nil];
}

- (void)loadPreferences
{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    [userDefaults synchronize];

    NSString *candidateSize = [NSString stringWithFormat:@"%ju", (uintmax_t)[userDefaults integerForKey:OVCandidateListTextSizeKey]];
    [self.fieldCandidateSize selectItemWithTitle:candidateSize];
    if (![self.fieldCandidateSize selectedItem]) {
        candidateSize = [NSString stringWithFormat:@"%ju", (uintmax_t)OVDefaultCandidateListTextSize];
        [self.fieldCandidateSize selectItemWithTitle:candidateSize];
    }

    if (![[userDefaults objectForKey:OVCandidateListStyleNameKey] isEqual:OVHorizontalCandidateListStyleName]) {
        [self.fieldCandidateSytle selectCellWithTag:0];
    }
    else {
        [self.fieldCandidateSytle selectCellWithTag:1];
    }

    [self configureKeyboardLayoutList:self.fieldAlphanumericKeyboardLayout];

    [self.fieldPlaySound setState:([userDefaults boolForKey:OVMakeSoundFeedbackOnInputErrorKey] ? NSOnState : NSOffState)];
    [self.fieldCheckForUpdate setState:([userDefaults boolForKey:OVCheckForUpdateKey] ? NSOnState : NSOffState)];

    if ([OVUpdateChecker sharedInstance].busy) {
        [self.checkForUpdateButton setEnabled:NO];
    }

    NSDate *lastCheckDate = [OVUpdateChecker sharedInstance].lastUpdateCheckDate;
    if (!lastCheckDate) {
        [self.lastUpdateCheckDateLabel setHidden:YES];
        return;
    }

    [self.lastUpdateCheckDateLabel setHidden:NO];

    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateStyle:NSDateFormatterShortStyle];
    [dateFormatter setTimeStyle:NSDateFormatterShortStyle];
    NSString *lastCheckString = [NSString stringWithFormat:NSLocalizedString(@"Last checked: %@", nil), [dateFormatter stringFromDate:lastCheckDate]];
    [self.lastUpdateCheckDateLabel setStringValue:lastCheckString];
}

- (IBAction)checkForUpdateAction:(id)sender
{
    [self.checkForUpdateButton setEnabled:NO];
    [[OVUpdateChecker sharedInstance] checkForUpdate];
}

- (IBAction)updateField:(id)sender
{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    [userDefaults setInteger:[[[self.fieldCandidateSize selectedItem] title] integerValue] forKey:OVCandidateListTextSizeKey];
    if ([[self.fieldCandidateSytle selectedCell] tag] == 0) {
        [userDefaults setObject:OVVerticalCandidateListStyleName forKey:OVCandidateListStyleNameKey];
    }
    else {
        [userDefaults setObject:OVHorizontalCandidateListStyleName forKey:OVCandidateListStyleNameKey];
    }

    [self setSharedAlphanumericKeyboardLayout:[[self.fieldAlphanumericKeyboardLayout selectedItem] representedObject]];

    [userDefaults setBool:(self.fieldPlaySound.state == NSOnState) forKey:OVMakeSoundFeedbackOnInputErrorKey];
    [userDefaults setBool:(self.fieldCheckForUpdate.state == NSOnState) forKey:OVCheckForUpdateKey];
    [userDefaults synchronize];
}

#pragma mark -

- (void)handleUpdateCheckDidComplete:(NSNotification *)notification
{
    [self.checkForUpdateButton setEnabled:YES];
    [self loadPreferences];
}
@end
