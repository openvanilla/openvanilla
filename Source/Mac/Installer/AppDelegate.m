//
// AppDelegate.m
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

#import "AppDelegate.h"

static NSString *const kTargetBin = @"OpenVanilla";
static NSString *const kTargetType = @"app";
static NSString *const kTargetBundle = @"OpenVanilla.app";
static NSString *const kDestinationPartial = @"~/Library/Input Methods/";
static NSString *const kTargetPartialPath = @"~/Library/Input Methods/OpenVanilla.app";
static NSString *const kTargetFullBinPartialPath = @"~/Library/Input Methods/OpenVanilla.app/Contents/MacOS/OpenVanilla";
static NSString *const kLegacyAppPath = @"/Library/Input Methods/OpenVanilla.app";
static NSString *const kLogoutRequirementExplanationURLString = @"http://openvanilla.org/docs/why-logout-is-needed.html";
static NSString *const kLegacyMigrationURLString = @"http://openvanilla.org/docs/migrate-from-old-openvanilla.html";
static NSString *const kMcBopomofoURLString = @"http://mcbopomofo.openvanilla.org";
static NSString *const kPreviousVersionSettings = @"~/Library/Preferences/org.openvanilla.plist";

static NSString *const kLegacyOVIMGenericUserTablePath = @"~/Library/Application Support/OpenVanilla/UserData/OVIMGeneric";
static NSString *const kNewOVIMTableBasedUserTablePath = @"~/Library/Application Support/OpenVanilla/UserData/TableBased";

@implementation AppDelegate
@synthesize installButton = _installButton;
@synthesize cancelButton = _cancelButton;
@synthesize textView = _textView;

- (void)dealloc
{
    [_installingVersion release];
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    if ([[NSFileManager defaultManager] fileExistsAtPath:kLegacyAppPath]) {
        NSUInteger result = NSRunAlertPanel(NSLocalizedString(@"Cannot Upgrade from Legacy Version", nil), NSLocalizedString(@"A legacy version of OpenVanilla (prior to 0.9) exists.\n\nPlease delete it before installing OpenVanilla.", nil), NSLocalizedString(@"Quit Installer", nil), NSLocalizedString(@"Uninstall Instructions", nil), nil);

        if (result == NSAlertAlternateReturn) {
            [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:kLegacyMigrationURLString]];
        }

        [[NSApplication sharedApplication] performSelector:@selector(terminate:) withObject:self afterDelay:0.1];
        return;
    }

    [self.cancelButton setNextKeyView:self.installButton];
    [self.installButton setNextKeyView:self.cancelButton];
    [[self window] setDefaultButtonCell:[self.installButton cell]];

    NSAttributedString *attrStr = [[[NSAttributedString alloc] initWithRTF:[NSData dataWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"License" ofType:@"rtf"]] documentAttributes:NULL] autorelease];

    [[self.textView textStorage] setAttributedString:attrStr];
    
    NSBundle *installingBundle = [NSBundle bundleWithPath:[[NSBundle mainBundle] pathForResource:kTargetBin ofType:kTargetType]];
    _installingVersion = [[[installingBundle infoDictionary] objectForKey:@"CFBundleShortVersionString"] retain];
    
    [[self window] setTitle:[NSString stringWithFormat:NSLocalizedString(@"%@ (for version %@)", nil), [[self window] title], _installingVersion]];
    
    if ([[NSFileManager defaultManager] fileExistsAtPath:[kTargetPartialPath stringByExpandingTildeInPath]]) {
        NSBundle *currentBundle = [NSBundle bundleWithPath:[kTargetPartialPath stringByExpandingTildeInPath]];

        NSString *currentVersion = nil;

        currentVersion = [[currentBundle infoDictionary] objectForKey:@"CFBundleShortVersionString"];
        if (currentVersion && [currentVersion compare:_installingVersion] == NSOrderedAscending) {
            _upgrading = YES;
        }
        else if (!currentVersion) {
            // legacy OV (pre-1.0, most likely 0.9.0)
            _upgrading = YES;
            _upgradingFromLegacy = YES;

            // see if the user is using a bopomofo input method
            NSDictionary *oldSettings = [NSDictionary dictionaryWithContentsOfFile:[kPreviousVersionSettings stringByExpandingTildeInPath]];
            if (oldSettings) {
                NSString *primaryInputMethod = [oldSettings objectForKey:@"primaryInputMethod"];

                _wasUsingOVIMGeneric = [primaryInputMethod hasPrefix:@"OVIMGeneric"];

                if ([primaryInputMethod isEqualToString:@"OVIMPhonetic"] || [primaryInputMethod isEqualToString:@"OVIMSpaceChewing"] || [primaryInputMethod isEqualToString:@"OVIMChewing"]) {                    
                    NSUInteger result = NSRunAlertPanel(NSLocalizedString(@"Unsupported Input Method", nil), NSLocalizedString(@"You are using a Bopomofo-based input method, which OpenVanilla no longer supports.\n\nWe recommend you install McBopomofo instead.", nil), NSLocalizedString(@"Visit McBopomofo Website", nil), NSLocalizedString(@"Continue Anyway", nil), NSLocalizedString(@"Quit Installer", nil));

                    if (result == NSAlertDefaultReturn) {
                        [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:kMcBopomofoURLString]];
                    }

                    if (result != NSAlertAlternateReturn) {
                        [[NSApplication sharedApplication] performSelector:@selector(terminate:) withObject:self afterDelay:0.1];
                        return;
                    }
                }
            }
        }
    }

    if (_upgrading) {
        [_installButton setTitle:NSLocalizedString(@"Agree and Upgrade", nil)];
    }

    [[self window] center];
    [[self window] orderFront:self];
    [[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
}

- (IBAction)agreeAndInstallAction:(id)sender
{
    [_cancelButton setEnabled:NO];
    [_installButton setEnabled:NO];

    if ([[NSFileManager defaultManager] fileExistsAtPath:[kTargetPartialPath stringByExpandingTildeInPath]]) {
        // http://www.cocoadev.com/index.pl?MoveToTrash
        NSString *sourceDir = [kDestinationPartial stringByExpandingTildeInPath];
        NSString *trashDir = [NSHomeDirectory() stringByAppendingPathComponent:@".Trash"];
        NSInteger tag;
        
        [[NSWorkspace sharedWorkspace] performFileOperation:NSWorkspaceRecycleOperation source:sourceDir destination:trashDir files:[NSArray arrayWithObject:kTargetBundle] tag:&tag]; 
        (void)tag;

        // alno need to restart SystemUIServer to ensure that the icon is fully cleaned up
        if (_upgradingFromLegacy) {
            NSTask *restartSystemUIServerTask = [NSTask launchedTaskWithLaunchPath:@"/usr/bin/killall" arguments:[NSArray arrayWithObjects: @"-9", @"SystemUIServer", nil]];
            [restartSystemUIServerTask waitUntilExit];
        }

        NSTask *killTask = [NSTask launchedTaskWithLaunchPath:@"/usr/bin/killall" arguments:[NSArray arrayWithObjects: @"-9", kTargetBin, nil]];
        [killTask waitUntilExit];
    }
    
    NSTask *cpTask = [NSTask launchedTaskWithLaunchPath:@"/bin/cp" arguments:[NSArray arrayWithObjects:@"-R", [[NSBundle mainBundle] pathForResource:kTargetBin ofType:kTargetType], [kDestinationPartial stringByExpandingTildeInPath], nil]];
    [cpTask waitUntilExit];
    if ([cpTask terminationStatus] != 0) {
        NSRunAlertPanel(NSLocalizedString(@"Install Failed", nil), NSLocalizedString(@"Cannot copy the file to the destination.", nil),  NSLocalizedString(@"Cancel", nil), nil, nil);
        [NSApp terminate:self];        
    }

    NSArray *installArgs = [NSArray arrayWithObjects:@"install", nil];
    NSTask *installTask = [NSTask launchedTaskWithLaunchPath:[kTargetFullBinPartialPath stringByExpandingTildeInPath] arguments:installArgs];
    [installTask waitUntilExit];
    if ([installTask terminationStatus] != 0) {
        NSRunAlertPanel(NSLocalizedString(@"Install Failed", nil), NSLocalizedString(@"Cannot activate the input method.", nil),  NSLocalizedString(@"Cancel", nil), nil, nil);
        [NSApp terminate:self];        
    }

    if (_upgradingFromLegacy) {
        if ([[NSFileManager defaultManager] fileExistsAtPath:[kLegacyOVIMGenericUserTablePath stringByExpandingTildeInPath]] && ![[NSFileManager defaultManager] fileExistsAtPath:[kNewOVIMTableBasedUserTablePath stringByExpandingTildeInPath]]) {
            [[NSFileManager defaultManager] moveItemAtPath:[kLegacyOVIMGenericUserTablePath stringByExpandingTildeInPath] toPath:[kNewOVIMTableBasedUserTablePath stringByExpandingTildeInPath] error:NULL];
        }

        if (_wasUsingOVIMGeneric) {
            NSRunAlertPanel(NSLocalizedString(@"Table-Based Input Method Setup Notice", nil), NSLocalizedString(@"Since you have been using a table-based input method, please go to OpenVanilla preferences to update your input method settings after installation.", nil),  NSLocalizedString(@"OK", nil), nil, nil);
        }

        NSInteger result = NSRunAlertPanel(NSLocalizedString(@"Upgrade Successful", nil), NSLocalizedString(@"OpenVanilla is ready to use.\n\nSince you have upgraded from an older version of OpenVanilla (before 1.0), we recommend you log out to make sure that when you come back, every app works with OpenVanilla.", nil), NSLocalizedString(@"Log Out", nil), NSLocalizedString(@"Why I Need This?", nil), NSLocalizedString(@"Finish", nil));
        if (result == NSAlertDefaultReturn) {
            NSString *scriptSource = @"tell application \"System Events\" to log out";
            NSAppleScript *appleScript = [[[NSAppleScript alloc] initWithSource:scriptSource] autorelease];
            NSDictionary *errDict = nil;
            [appleScript executeAndReturnError:&errDict];
        }
        else if (result == NSAlertAlternateReturn) {
            [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:kLogoutRequirementExplanationURLString]];
        }
    }
    else {
        NSRunAlertPanel(NSLocalizedString(@"Installation Successful", nil), NSLocalizedString(@"OpenVanilla is ready to use.", nil),  NSLocalizedString(@"OK", nil), nil, nil);
    }

    [[NSApplication sharedApplication] performSelector:@selector(terminate:) withObject:self afterDelay:0.1];
}
                                   

- (IBAction)cancelAction:(id)sender
{
    [NSApp terminate:self];
}

- (void)windowWillClose:(NSNotification *)notification
{
    [NSApp terminate:self];    
}
@end
