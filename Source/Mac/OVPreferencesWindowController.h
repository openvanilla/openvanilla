//
//  OVPreferencesWindowController.h
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/6/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVBasePreferencesViewController.h"

@interface OVPreferencesWindowController : NSWindowController <NSTableViewDataSource, NSTableViewDelegate>
{
    NSMutableArray *_items;
    NSTableView *_tableView;
    NSView *_modulePreferencesContainerView;
    OVBasePreferencesViewController *_generalPreferencesViewController;
    OVBasePreferencesViewController *_tableBasedMoudlePreferencesViewController;
    OVBasePreferencesViewController *_currentPreferencesViewController;
}
@property (assign, nonatomic) IBOutlet NSTableView *tableView;
@property (assign, nonatomic) IBOutlet NSView *modulePreferencesContainerView;
@property (assign, nonatomic) IBOutlet OVBasePreferencesViewController *generalPreferencesViewController;
@property (assign, nonatomic) IBOutlet OVBasePreferencesViewController *tableBasedMoudlePreferencesViewController;
@end
