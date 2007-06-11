// CinDocument.mm
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.w
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
#include "OVCIN.h"

@implementation CinDocument

- (void) showMsgWindow: (NSString *) msg {
	[msgIndicator startAnimation:self];
	[msgText setStringValue:msg];
	[msgWindow orderFront:self];
	[msgWindow center];
}

- (void) hideMsgWindow {
	[msgIndicator stopAnimation:self];	
	[msgWindow orderOut:self];
}

- (void) editCin{
	isEdited = YES;
}

- (IBAction)keynameAdd:(id)sender {
	int i = -1;
	i = [k addRow:[listKeyname selectedRow]];
	[listKeyname reloadData];
	if(i > -1) {
		[listKeyname selectRowIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(i,1)] byExtendingSelection:YES];
	}	
	[self editCin];
}

- (IBAction)keynameRemove:(id)sender {
	if([listKeyname selectedRow] > -1) {
		[k removeRow:[listKeyname selectedRow]];
		[listKeyname reloadData];
		[self editCin];
	}
}

- (IBAction)chardefAdd:(id)sender {
	int i = -1;
	i = [c addRow:[listChardef selectedRow]];
	[listChardef reloadData];
	if(i > -1) {
		[listChardef selectRowIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(i,1)] byExtendingSelection:YES];		
	}
	[self editCin];	
}

- (IBAction)chardefRemove:(id)sender {
	if([listChardef selectedRow] > -1) {
		[c removeRow:[listChardef selectedRow]];
		[listChardef reloadData];
		[self editCin];
	}
}

- (void) saveExec: (NSString *) savePath  {
	NSMutableString  * cin = [NSMutableString new];
	[cin appendFormat:@"%cgen_inp\n", '%'];
	if([txtEname stringValue]) {
		[cin appendFormat:@"%cename %@\n", '%', [txtEname stringValue]];
	}
	if([txtCname stringValue]) {
		[cin appendFormat:@"%ccname %@\n", '%', [txtCname stringValue]];
	}
	[cin appendFormat:@"%ctcname %@\n", '%', [txtTcname stringValue]];
	[cin appendFormat:@"%cscname %@\n", '%', [txtScname stringValue]];
	[cin appendFormat:@"%cselkey %@\n", '%', [txtSelkey stringValue]];
	[cin appendFormat:@"%cendkey %@\n", '%', [k dumpEndkey]];		
	[cin appendFormat:@"%cencoding UTF-8\n", '%'];	
	[cin appendFormat:@"%ckeyname begin\n", '%'];
	int i;
	for(i = 0; i < [k count]; i++) {
		[cin appendString:[k dumpline:i]];
	}
	[cin appendFormat:@"%ckeyname end\n", '%'];
	[cin appendFormat:@"%cchardef begin\n", '%'];
	for(i = 0; i < [c count]; i++) {
		[cin appendString:[c dumpline:i]];
	}
	[cin appendFormat:@"%cchardef end\n", '%'];	
	[cin writeToFile:savePath
		  atomically:TRUE
			encoding:NSUTF8StringEncoding
			   error:NULL
		];
}

- (void) openCin {
	[self showMsgWindow:MSG(@"Opening a Input Method Table file. If you have a lot of definitions within this file, it may take time. Please be patient.")];
	OVCIN *cin = new OVCIN();
	cin->load([[currentPath stringByDeletingLastPathComponent] UTF8String], [[currentPath lastPathComponent] UTF8String]);
	_ename = [NSString stringWithUTF8String: cin->get_ename()];
	_cname = [NSString stringWithUTF8String: cin->get_cname()];
	_tcname = [NSString stringWithUTF8String: cin->get_tcname()];
	_scname = [NSString stringWithUTF8String: cin->get_scname()];	
	_selkey = [NSString stringWithUTF8String: cin->get_selkey()];
	int i;	
	k = [keyTable alloc];
	[k init];
	for(i = 0; i < cin->keycount(); i++) {
		Keyname keyname=cin->get_key(i);
		NSMutableDictionary *d=[NSMutableDictionary new];
		if(strstr(cin->get_endkey(), keyname.key.c_str())) {
			[d setObject:[NSNumber numberWithInt:YES] forKey:@"endkey"];
		} else {
			[d setObject:[NSNumber numberWithInt:NO] forKey:@"endkey"];			
		}
		[d setObject:[NSString stringWithUTF8String:keyname.key.c_str()] forKey:@"key"];
		[d setObject:[NSString stringWithUTF8String:keyname.value.c_str()] forKey:@"value"];
		[k addKey: d];
		[d release];		
	}	
	c = [charTable alloc];
	[c init];
	for(i = 0; i < cin->charcount(); i++) {
		Chardef chardef=cin->get_char(i);
		NSMutableDictionary *d=[NSMutableDictionary new];		
		[d setObject:[NSString stringWithUTF8String:chardef.key.c_str()] forKey:@"key"];
		[d setObject:[NSString stringWithUTF8String:chardef.value.c_str()] forKey:@"value"];
		[c addKey: d];
	}
	[self hideMsgWindow];	
}

- (void) makeNewCin {
	_ename = @"New Input Method";
	_cname = @"New Input Method";
	_tcname = @"New Input Method";
	_scname = @"New Input Method";
	_selkey = @"1234567890";
	k = [keyTable alloc];
	[k init];
	int i;
	for(i = 0; i < 26; i++) {
		NSMutableDictionary *d=[NSMutableDictionary new];		
		[d setObject:[NSNumber numberWithInt:NO] forKey:@"endkey"];
		unichar c = 'a' + i;
		NSString *a = [[NSString alloc] initWithCharacters:&c length:1];		
		[d setObject:a forKey:@"key"];
		[d setObject:a forKey:@"value"];
		[k addKey: d];
		[d release];
		[a release];
	}
	c = [charTable alloc];
	[c init];
}

- (void) awakeFromNib {
	if(![currentPath length]) {
		[self makeNewCin];
	} else {
		[self openCin];		
	}
	[listKeyname setDataSource:k];
	[listChardef setDataSource:c];	
	[txtEname setStringValue:_ename];
	[txtCname setStringValue:_cname];
	[txtTcname setStringValue:_tcname];	
	[txtScname setStringValue:_scname];	
	[txtSelkey setStringValue:_selkey];
	isEdited = NO;
}

- (IBAction)newDocument:(id)sender{
	NSLog(@"Creating new input method table");
	[[NSDocumentController sharedDocumentController] newDocument: sender];
}

- (IBAction)openDocument:(id)sender{
	[[NSDocumentController sharedDocumentController] openDocument: sender];
}

/* NSDocument routines */

- (id)init {
    self = [super init];
    return self;
}

- (NSString *)windowNibName {
    return @"CinDocument";
}

- (BOOL)readFromURL:(NSURL *)inAbsoluteURL ofType:(NSString *)inTypeName error:(NSError **)outError {
	currentPath = [inAbsoluteURL path];
    return YES;
}

- (BOOL)writeToURL:(NSURL *)inAbsoluteURL ofType:(NSString *)inTypeName error:(NSError **)outError {
	[self showMsgWindow:MSG(@"Svaing your Input Method Table file. If you have a lot of definitions within this file, it may take time. Please be patient.")];	
	[self saveExec:[inAbsoluteURL path]];
	_ename = [txtEname stringValue];
	_cname = [txtCname stringValue];
	_tcname = [txtTcname stringValue];	
	_scname = [txtScname stringValue];	
	_selkey = [txtSelkey stringValue];
	[k reset];
	[c reset];
	isEdited = NO;		
	[self hideMsgWindow];	
	return YES;
}

- (void)windowControllerDidLoadNib:(NSWindowController *) windowController {
    [super windowControllerDidLoadNib:windowController];
	// Adding toolbar to a window
    [self setupToolbarForWindow:[windowController window]];		
}

- (NSData *)dataRepresentationOfType:(NSString *)aType {	
    return nil;
}

- (BOOL)loadDataRepresentation:(NSData *)data ofType:(NSString *)aType {  
    return YES;
}

/* Actions of opening helpbook */

- (IBAction)keymapHelp:(id)sender {
	[[NSHelpManager sharedHelpManager] openHelpAnchor:@"keymap" inBook:[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"]];
}

- (IBAction)chardefHelp:(id)sender {
	[[NSHelpManager sharedHelpManager] openHelpAnchor:@"chardef" inBook:[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"]];
}


/* Window delegate */

- (BOOL)windowShouldClose:(id)sender {
	if([k isEdited] || [c isEdited]) {
		[self editCin];
	}
	if(isEdited) {
		NSAlert *alert = [NSAlert alertWithMessageText:MSG(@"You changes will be lost!") 
										 defaultButton:MSG(@"Save") 
									   alternateButton:MSG(@"Continue Editing") 
										   otherButton:MSG(@"Close without Saving") 
							 informativeTextWithFormat:MSG(@"If you do not save file before closing window, your changes will be lost. Do you want to save?")];
		int i =  [alert runModal];
		if(i == 1) {
			[self saveDocument:sender];
			return NO;
		} 
		return i;
	}
	return YES;
}

- (void)windowWillClose:(NSNotification *)aNotification{
	[k release];
	[c release];
}

- (BOOL)windowShouldZoom:(NSWindow *)window toFrame:(NSRect)newFrame {
    return YES;
}

@end