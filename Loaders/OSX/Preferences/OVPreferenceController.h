//
//  OVPreferenceController.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CVEmbeddedLoader.h"
#import "CVWrappers.h"
#import "NSObjectUpdateConfig.h"
#import "OVModulesListController.h"
#import "OVModuleController.h"
#import "OVIMGenericController.h"
#import "OVTableModuleController.h"

#define WINDOW_TITLE_HEIGHT 78
#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]

static NSString *GeneralToolbarItemIdentifier				= @"General";
static NSString *ModulesToolbarItemIdentifier				= @"Modules";

@interface OVPreferenceController : NSWindowController {
	IBOutlet NSView *u_mainView;
	IBOutlet OVModulesListController *m_moduleListController;
	
    CVEmbeddedLoader *_loader;
    NSMutableDictionary *_config;
}
- (void)writeConfig;
- (void)setActiveView:(NSView *)view animate:(BOOL)flag;
@end
