//
//  OVIMListController.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "NSObjectUpdateConfig.h"

@interface OVModulesListController : NSObject {
	IBOutlet NSOutlineView *u_outlineView;
	IBOutlet NSView *u_settingView;
	IBOutlet NSView *u_mainView;
	IBOutlet id delegate;
	
	NSMutableArray *m_inputMethods;
	NSMutableArray *m_ouputFilters;
	
	id _currentItem;
}

- (NSView *)view;
- (void)reload;
- (void)addInputMethod:(id)inputMethod;
- (void)addOutputFilter:(id)outputFilter;
- (void)setCurrentItem:(id)item;
@end
