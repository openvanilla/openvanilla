//
//  OVPreferencesWindowController.h
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/6/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface OVPreferencesWindowController : NSWindowController <NSTableViewDataSource, NSTableViewDelegate>
{
    NSMutableArray *_items;
    NSTableView *_tableView;
    NSView *_modulePreferencesContainerView;
    NSViewController *_generalPreferencesViewController;
    NSViewController *_tableBasedMoudlePreferencesViewController;
    NSViewController *_currentPreferencesViewController;
}
@property (assign, nonatomic) IBOutlet NSTableView *tableView;
@property (assign, nonatomic) IBOutlet NSView *modulePreferencesContainerView;
@property (assign, nonatomic) IBOutlet NSViewController *generalPreferencesViewController;
@property (assign, nonatomic) IBOutlet NSViewController *tableBasedMoudlePreferencesViewController;
@end
