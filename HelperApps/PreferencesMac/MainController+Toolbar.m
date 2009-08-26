//
//  MainController+Toolbar.m
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#import "MainController+Toolbar.h"

static NSString *toolbarIdentifier = @"Toolbar";
static NSString *loaderSettingToolbarItemIndentifier = @"Loader";
static NSString *modulesSettingToolbarItemIndentifier = @"Modules";

@implementation MainController(Toolbar)

- (void)initToolbar
{
	NSToolbar *toolbar = [[[NSToolbar alloc] initWithIdentifier:toolbarIdentifier] autorelease];
	[toolbar setDelegate:self];
	[toolbar setSelectedItemIdentifier:modulesSettingToolbarItemIndentifier];
	[[self window] setToolbar:toolbar];
}

- (void)setActiveView:(NSView *)view animate:(BOOL)flag
{	
	NSRect windowFrame = [[self window] frame];
	windowFrame.size.height = [view frame].size.height + WINDOW_TITLE_HEIGHT;
	windowFrame.size.width = [view frame].size.width;
	windowFrame.origin.y = NSMaxY([[self window] frame]) - ([view frame].size.height + WINDOW_TITLE_HEIGHT);
	
	NSView *mainView = [[self window] contentView];
	
	if ([[mainView subviews] count] != 0)
		[[[mainView subviews] objectAtIndex:0] removeFromSuperview];
	
	[[self window] setFrame:windowFrame display:YES animate:flag];		
	[mainView setFrame:[view frame]];
	[mainView addSubview:view];
}


- (void)toggleActivePreferenceView:(id)sender
{
	NSView *view;
	
//	if ([[sender itemIdentifier] isEqualToString:loaderSettingToolbarItemIndentifier])
//		view = [m_displayController view];
//	else if ([[sender itemIdentifier] isEqualToString:modulesSettingToolbarItemIndentifier])
//		view = [m_moduleListController view];
//	
//	[self setActiveView:view animate:YES];
//	[[self window] setTitle:MSG([sender itemIdentifier])];
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar
{
	return [NSArray arrayWithObjects:
//			loaderSettingToolbarItemIndentifier,
			modulesSettingToolbarItemIndentifier,				
			nil];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar 
{
	return [NSArray arrayWithObjects:
//			loaderSettingToolbarItemIndentifier,
			modulesSettingToolbarItemIndentifier,
			nil];
}

- (NSArray *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar
{
	return [NSArray arrayWithObjects:
//			loaderSettingToolbarItemIndentifier,
			modulesSettingToolbarItemIndentifier,
			nil];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)identifier willBeInsertedIntoToolbar:(BOOL)willBeInserted 
{
	NSToolbarItem *item = [[[NSToolbarItem alloc] initWithItemIdentifier:identifier] autorelease];
	if ([identifier isEqualToString:loaderSettingToolbarItemIndentifier]) {
		[item setLabel:NSLocalizedString(loaderSettingToolbarItemIndentifier, @"")];
		[item setImage:[NSImage imageNamed:@"General"]];	
		[item setTarget:self];
		[item setAction:@selector(toggleActivePreferenceView:)];
	}
	else if ([identifier isEqualToString:modulesSettingToolbarItemIndentifier]) {
		[item setLabel:NSLocalizedString(modulesSettingToolbarItemIndentifier, @"")];
		[item setImage:[NSImage imageNamed:@"Modules"]];
		[item setTarget:self];
		[item setAction:@selector(toggleActivePreferenceView:)];
	}
	else
		item = nil;
	return item; 
}

@end
