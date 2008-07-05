//
//  OVPreference_Toolbar.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVPreferenceController.h"


@implementation OVPreferenceController(Toolbar)

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar
{
	return [NSArray arrayWithObjects:
			GeneralToolbarItemIdentifier,
			ModulesToolbarItemIdentifier,				
			nil];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar 
{
	return [NSArray arrayWithObjects:
			GeneralToolbarItemIdentifier,
			ModulesToolbarItemIdentifier,
			nil];
}

- (NSArray *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar
{
	return [NSArray arrayWithObjects:
			GeneralToolbarItemIdentifier,
			ModulesToolbarItemIdentifier,
			nil];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)identifier willBeInsertedIntoToolbar:(BOOL)willBeInserted 
{
	NSToolbarItem *item = [[[NSToolbarItem alloc] initWithItemIdentifier:identifier] autorelease];
	if ([identifier isEqualToString:GeneralToolbarItemIdentifier]) {
		[item setLabel:MSG(GeneralToolbarItemIdentifier)];
		[item setImage:[NSImage imageNamed:NSImageNamePreferencesGeneral]];	
		[item setTarget:self];
//		[item setAction:@selector(toggleActivePreferenceView:)];
	}
	else if ([identifier isEqualToString:ModulesToolbarItemIdentifier]) {
		[item setLabel:MSG(ModulesToolbarItemIdentifier)];
		[item setImage:[NSImage imageNamed:NSImageNamePreferencesGeneral]];
		[item setTarget:self];
//		[item setAction:@selector(toggleActivePreferenceView:)];
	}
	else
		item = nil;
	return item; 
}


@end
