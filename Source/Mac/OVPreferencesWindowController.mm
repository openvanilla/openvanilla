//
//  OVPreferencesWindowController.m
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/6/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
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
        [controller synchronize];
    }
    else {
        // [controller synchronize];
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
