//
// OVAddTableBasedInputMethodViewController.m
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

#import "OVAddTableBasedInputMethodViewController.h"
#import "OVModuleManager.h"
#import "OVNonModalAlertWindowController.h"

@interface OVAddTableBasedInputMethodViewController () <OVNonModalAlertWindowControllerDelegate>
@end

@implementation OVAddTableBasedInputMethodViewController

- (IBAction)importNewTableAction:(id)sender
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setTitle:NSLocalizedString(@"Pick the .cin Table to Import", nil)];
    [panel setLevel:CGShieldingWindowLevel() + 1];
    [panel setAllowsMultipleSelection:NO];
    [panel setAllowedFileTypes:[NSArray arrayWithObject:@"cin"]];

    [panel beginWithCompletionHandler:^(NSInteger result) {
        if (!result == NSFileHandlingPanelOKButton) {
            return;
        }

        NSArray *files = [panel URLs];
        if (![files count]) {
            return;
        }

        NSString *cinPath = [[files objectAtIndex:0] path];
        NSError *error = nil;
        BOOL override = NO;
        BOOL canInstall = [[OVModuleManager defaultManager] canInstallCustomTableBasedInputMethodWithTablePath:cinPath willOverrideBuiltInTable:&override error:&error];
        if (!canInstall) {
            [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Cannot Import Input Method", nil) content:[NSString stringWithFormat:NSLocalizedString(@"\"%@\" is not a valid cin file.", nil), [cinPath lastPathComponent]] confirmButtonTitle:NSLocalizedString(@"Dismiss", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
            return;
        }

        if (override) {
            [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Overwriting Existing Input Method", nil) content:[NSString stringWithFormat:NSLocalizedString(@"\"%@\" will replace an existing input method that you have. Do you want to continue?", nil), [cinPath lastPathComponent]] confirmButtonTitle:NSLocalizedString(@"Overwrite", nil) cancelButtonTitle:NSLocalizedString(@"Cancel", nil) cancelAsDefault:YES delegate:self];
            return;
        }


        [[OVModuleManager defaultManager] installCustomTableBasedInputMethodWithTablePath:cinPath];

        [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Input Method Imported", nil) content:NSLocalizedString(@"Your new input method is ready to use", nil) confirmButtonTitle:NSLocalizedString(@"OK", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
    }];
}

- (void)nonModalAlertWindowControllerDidConfirm:(OVNonModalAlertWindowController *)controller
{
    NSLog(@"!");
}
@end
