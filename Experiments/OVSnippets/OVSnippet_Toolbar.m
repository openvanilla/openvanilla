//
// OVSnippet_Toolbar.m
// The routines to maintain a toolbar
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#import "OVSnippet_AppDelegate.h"

static NSString *drawerToolbarItemIdentifier				= @"ToogleDrawer";

@implementation OVSnippet_AppDelegate(Toolbar)

- (void)setupToolbarForWindow:(NSWindow *)window {
	
	NSToolbar * toolbar = [[[NSToolbar alloc] initWithIdentifier:@"OVSnippetToolbar"] autorelease];
    [toolbar setAllowsUserCustomization:NO];
    [toolbar setAutosavesConfiguration:YES];
	[toolbar setDisplayMode:NSToolbarDisplayModeIconAndLabel];
	[toolbar setDelegate:self];
    [window setToolbar:toolbar];
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar
{
	return [NSArray arrayWithObjects:
		drawerToolbarItemIdentifier,
		NSToolbarSeparatorItemIdentifier, 		
		NSToolbarFlexibleSpaceItemIdentifier,
		nil];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar 
{
	return [NSArray arrayWithObjects:
		drawerToolbarItemIdentifier,
		NSToolbarFlexibleSpaceItemIdentifier,
		NSToolbarSeparatorItemIdentifier, 
        NSToolbarCustomizeToolbarItemIdentifier,
		nil];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar 
	 itemForItemIdentifier:(NSString *)identifier
 willBeInsertedIntoToolbar:(BOOL)willBeInserted 
{
	NSToolbarItem *item = [[[NSToolbarItem alloc] initWithItemIdentifier:identifier] autorelease];
	if ([identifier isEqualToString:drawerToolbarItemIdentifier]) {
		[item setLabel:MSG(drawerToolbarItemIdentifier)];
        [item setPaletteLabel:MSG(drawerToolbarItemIdentifier)];
		[item setImage:[NSImage imageNamed:@"drawer"]];
		[item setTarget:self];
        [item setAction: @selector(toogleDrawer:)];
	} else
		item = nil;
	return item; 
}

- (BOOL)validateToolbarItem:(NSToolbarItem *)item {
    BOOL enable = NO;
    if ([[item itemIdentifier] isEqual:drawerToolbarItemIdentifier]) {
        enable = YES;
    }		
    return enable;
}

- (IBAction) toogleDrawer: (id)sender {
	[drawer toggle:self];
}

@end
