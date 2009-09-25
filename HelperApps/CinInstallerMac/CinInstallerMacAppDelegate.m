//
// CinInstallerMacAppDelegate.m
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

#import "CinInstallerMacAppDelegate.h"

@implementation CinInstallerMacAppDelegate

- (NSString *)targetPath
{
	NSArray *libPaths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
	NSString *libPath = [libPaths objectAtIndex:0];
	NSString *appSupportPath = [libPath stringByAppendingPathComponent:@"Application Support"];
	BOOL isDir = NO;
	if (![[NSFileManager defaultManager] fileExistsAtPath:appSupportPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:appSupportPath attributes:nil];
	}
	if (!isDir) {
		[[NSFileManager defaultManager] removeItemAtPath:appSupportPath error:nil];
		[[NSFileManager defaultManager] createDirectoryAtPath:appSupportPath attributes:nil];
	}
	NSString *ovPath = [appSupportPath stringByAppendingPathComponent:@"OpenVanilla"];
	if (![[NSFileManager defaultManager] fileExistsAtPath:ovPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:ovPath attributes:nil];
	}
	if (!isDir) {
		[[NSFileManager defaultManager] removeItemAtPath:ovPath error:nil];
		[[NSFileManager defaultManager] createDirectoryAtPath:ovPath attributes:nil];
	}
	NSString *genericPath = [ovPath stringByAppendingPathComponent:@"OVIMGeneric"];
	if (![[NSFileManager defaultManager] fileExistsAtPath:genericPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:genericPath attributes:nil];
	}
	if (!isDir) {
		[[NSFileManager defaultManager] removeItemAtPath:genericPath error:nil];
		[[NSFileManager defaultManager] createDirectoryAtPath:genericPath attributes:nil];
	}
	return genericPath;
}

- (void)showOpenPanel
{
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	NSArray *desktopPaths = NSSearchPathForDirectoriesInDomains(NSDesktopDirectory, NSUserDomainMask, YES);
	NSString *desktopPath = [desktopPaths objectAtIndex:0];
	[openPanel setDirectory:desktopPath];
	[openPanel setAllowsMultipleSelection:NO];
	[openPanel setTitle:NSLocalizedString(@"Install .cin table file", @"")];
	[openPanel setAllowedFileTypes:[NSArray arrayWithObject:@"cin"]];
	[openPanel setAllowsOtherFileTypes:NO];
	[openPanel setPrompt:NSLocalizedString(@"Choose", @"")];
	[openPanel setCanCreateDirectories:NO];
	int result = [openPanel runModal];
	if (result != NSOKButton) {
		[NSApp terminate:self];
		return;
	}
	NSString *fullPath = [openPanel filename];
	NSString *filename = [fullPath lastPathComponent];
	NSString *ext = [fullPath pathExtension];
	if (![ext isEqualToString:@"cin"]) {
		NSAlert *alert = [NSAlert alertWithMessageText:NSLocalizedString(@"Selected file is invalid!", @"") defaultButton:NSLocalizedString(@"OK", @"") alternateButton:nil otherButton:nil informativeTextWithFormat:@""];
		[alert runModal];
		[NSApp terminate:self];
		return;
	}
	NSString *targetPath = [self targetPath];
	targetPath = [targetPath stringByAppendingPathComponent:filename];
	if ([[NSFileManager defaultManager] fileExistsAtPath:targetPath]) {
		NSAlert *alert = [NSAlert alertWithMessageText:NSLocalizedString(@"File already exists, do you want to replace it?", @"") defaultButton:NSLocalizedString(@"Replace", @"") alternateButton:NSLocalizedString(@"Cancel", @"") otherButton:nil informativeTextWithFormat:@""];
		result = [alert runModal];
		if (result != NSOKButton) {
			[NSApp terminate:self];
			return;			
		}
		[[NSFileManager defaultManager] removeFileAtPath:targetPath handler:nil];
	}
	BOOL copyResult = [[NSFileManager defaultManager] copyPath:fullPath toPath:targetPath handler:nil];
	if (!copyResult) {
		NSAlert *alert = [NSAlert alertWithMessageText:NSLocalizedString(@"Failed to copy the .cin file!", @"") defaultButton:NSLocalizedString(@"OK", @"") alternateButton:nil otherButton:nil informativeTextWithFormat:@""];
		[alert runModal];
		[NSApp terminate:self];
		return;
	}
	[NSApp terminate:self];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	[self showOpenPanel];
}

@end
