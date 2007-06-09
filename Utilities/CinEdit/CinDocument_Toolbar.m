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

#import "CinDocument.h"

static NSString *newToolbarItemIdentifier				= @"New";
static NSString *openToolbarItemIdentifier				= @"Open";
static NSString *saveToolbarItemIdentifier				= @"Save";

@implementation CinDocument(Toolbar)

- (void)setupToolbarForWindow:(NSWindow *)window {
	
	NSToolbar * toolbar = [[[NSToolbar alloc] initWithIdentifier:@"cinToolbar"] autorelease];
    [toolbar setAllowsUserCustomization:YES];
    [toolbar setAutosavesConfiguration:YES];
	[toolbar setDisplayMode:NSToolbarDisplayModeIconAndLabel];
	[toolbar setDelegate:self];
    [window setToolbar:toolbar];
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar
{
	return [NSArray arrayWithObjects:
		newToolbarItemIdentifier,
		openToolbarItemIdentifier,
		saveToolbarItemIdentifier,		
		NSToolbarFlexibleSpaceItemIdentifier,
		NSToolbarSeparatorItemIdentifier, 
        NSToolbarCustomizeToolbarItemIdentifier,
		nil];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar 
{
	return [NSArray arrayWithObjects:
		newToolbarItemIdentifier,
		openToolbarItemIdentifier,
		saveToolbarItemIdentifier,			
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
	if ([identifier isEqualToString:newToolbarItemIdentifier]) {
		[item setLabel:MSG(newToolbarItemIdentifier)];
        [item setPaletteLabel:MSG(newToolbarItemIdentifier)];
		[item setImage:[NSImage imageNamed:@"new"]];
		[item setTarget:self];
        [item setAction: @selector(newDocument:)];
	} else if ([identifier isEqualToString:openToolbarItemIdentifier]) {
		[item setLabel:MSG(openToolbarItemIdentifier)];
        [item setPaletteLabel:MSG(openToolbarItemIdentifier)];
		[item setImage:[NSImage imageNamed:@"open"]];
		[item setTarget:self];
        [item setAction: @selector(openDocument:)];
	} else if ([identifier isEqualToString:saveToolbarItemIdentifier]) {		
		[item setLabel:MSG(saveToolbarItemIdentifier)];
        [item setPaletteLabel:MSG(saveToolbarItemIdentifier)];
		[item setImage:[NSImage imageNamed:@"save"]];
		[item setTarget:self];
        [item setAction: @selector(saveDocument:)];
	} else
		item = nil;
	return item; 
}

- (BOOL)validateToolbarItem:(NSToolbarItem *)item {
    BOOL enable = NO;
    if ([[item itemIdentifier] isEqual:newToolbarItemIdentifier]) {
        enable = YES;
    } else if ([[item itemIdentifier] isEqual:openToolbarItemIdentifier]) {
        enable = YES;
    } else if ([[item itemIdentifier] isEqual:saveToolbarItemIdentifier]) {
        enable = YES;
    }		
    return enable;
}

@end