//
//  OVIMListController.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "NSObjectUpdateConfig.h"
#import "OVModuleController.h"
#import "OVHotkeyField.h"

@interface OVModulesListController : NSObject {
	IBOutlet NSOutlineView *u_outlineView;
	IBOutlet NSView *u_settingView;
	IBOutlet NSView *u_mainView;
	IBOutlet OVHotkeyField *u_hotkeyField;
	IBOutlet id m_preferenceController;
	
	NSMutableArray *m_inputMethods;
	NSMutableArray *m_ouputFilters;
	
	id _currentItem;
	id _draggingItem;
}

- (NSView *)view;
- (void)expandAll;
- (void)reload;
- (void)switchToView: (NSView *)view;
- (void)addInputMethod:(id)inputMethod;
- (void)addOutputFilter:(id)outputFilter;
- (void)setCurrentItem:(id)item;
@end
