//
//  OVPreferencesWindowController.m
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/6/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import "OVPreferencesWindowController.h"

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

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        _items = [[NSMutableArray alloc] init];
        // Initialization code here.

        [_items addObject:@"General"];
        [_items addObject:@"CJ"];
        [_items addObject:@"Simplex"];
    }
    
    return self;
}

- (void)dealloc
{
    [_items release];
    [super dealloc];
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    [self.tableView selectRowIndexes:[NSIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
    NSLog(@"%@", aNotification);
    if (self.currentPreferencesViewController) {
        [[self.currentPreferencesViewController view] removeFromSuperview];
        self.currentPreferencesViewController = nil;
    }

    NSInteger selection = [self.tableView selectedRow];
    if (selection == -1) {
        return;
    }

    if (selection == 0) {
        self.currentPreferencesViewController = self.generalPreferencesViewController;
    }
    else {
        self.currentPreferencesViewController = self.tableBasedMoudlePreferencesViewController;
    }

    [self.modulePreferencesContainerView addSubview:[self.currentPreferencesViewController view]];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [self.items count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    return [self.items objectAtIndex:rowIndex];
}
@end
