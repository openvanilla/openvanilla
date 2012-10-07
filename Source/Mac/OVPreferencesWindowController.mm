//
// OVPreferencesWindowController.mm
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

#import "OVPreferencesWindowController.h"
#import "OVModuleManager.h"

static NSString *const kIdentifierKey = @"IdentifierKey";
static NSString *const kLocalizedNameKey = @"LocalizedNameKey";
static NSString *const kControllerKey = @"ControllerKey";
static NSString *const kGeneralSettingIdentifier = @"GeneralSettingIdentifier";
static NSString *const kAddInputMethodIdentifier = @"AddInputMethodIdentifier";
static NSString *const kCheckUpdateIdentifier = @"CheckUpdateIdentifier";

static NSDictionary *Item(NSString *identifier, NSString *localizedName, OVBasePreferencesViewController *controller)
{
    return [NSDictionary dictionaryWithObjectsAndKeys:identifier, kIdentifierKey, localizedName, kLocalizedNameKey, controller, kControllerKey, nil];
}

@interface OVPreferencesWindowController ()
@property (assign, nonatomic) NSViewController *currentPreferencesViewController;
@property (retain, nonatomic) NSMutableArray *items;
@end

@implementation OVPreferencesWindowController
@synthesize items = _items;
@synthesize tableView = _tableView;
@synthesize modulePreferencesContainerView = _modulePreferencesContainerView;
@synthesize generalPreferencesViewController = _generalPreferencesViewController;
@synthesize tableBasedMoudlePreferencesViewController = _tableBasedMoudlePreferencesViewController;
@synthesize currentPreferencesViewController = _currentPreferencesViewController;

- (void)dealloc
{
    [_items release];
    [super dealloc];
}

- (void)windowDidLoad
{
    [super windowDidLoad];

    if (!_items) {
        _items = [[NSMutableArray alloc] init];
    }

    [_items removeAllObjects];

    [_items addObject:Item(kGeneralSettingIdentifier, NSLocalizedString(@"General Settings", nil), self.generalPreferencesViewController)];

    for (NSString *moduleIdentifier in [OVModuleManager defaultManager].inputMethodIdentifiers) {
        [_items addObject:Item(moduleIdentifier, [[OVModuleManager defaultManager] localizedInputMethodName:moduleIdentifier], self.tableBasedMoudlePreferencesViewController)];
    }

    [_items addObject:Item(kAddInputMethodIdentifier, NSLocalizedString(@"Add New Input Method", nil), self.generalPreferencesViewController)];

    [self.tableView selectRowIndexes:[NSIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
    if (self.currentPreferencesViewController) {
        [[self.currentPreferencesViewController view] removeFromSuperview];
        self.currentPreferencesViewController = nil;
    }

    NSInteger selection = [self.tableView selectedRow];
    if (selection == -1) {
        return;
    }

    NSDictionary *item = [self.items objectAtIndex:selection];

    OVBasePreferencesViewController *controller = [item objectForKey:kControllerKey];

    self.currentPreferencesViewController = controller;
    if (controller == self.tableBasedMoudlePreferencesViewController) {
        controller.moduleIdentifier = [item objectForKey:kIdentifierKey];
        [controller loadPreferences];
    }
    else {
        // [controller loadPreferences];
    }

    [self.modulePreferencesContainerView addSubview:[controller view]];

    NSString *title = [NSString stringWithFormat:NSLocalizedString(@"OpenVanilla - %@", nil), [item objectForKey:kLocalizedNameKey]];
    [[self window] setTitle:title];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [self.items count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    return [[self.items objectAtIndex:rowIndex] objectForKey:kLocalizedNameKey];
}
@end
