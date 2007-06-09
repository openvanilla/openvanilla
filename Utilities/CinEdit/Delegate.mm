// Delegate.mm
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

#import "Delegate.h"
#include "OVCIN.h"

#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]
#define ALERT(title, text, ok, cancel)  [[NSAlert alertWithMessageText:MSG(title) defaultButton:MSG(ok) alternateButton:MSG(cancel) otherButton:nil informativeTextWithFormat:MSG(text)] runModal]

#define OVCINPATH "/Library/OpenVanilla/0.7.2/Modules/OVIMGeneric/"

@implementation Delegate

- (void) editCin{
	isEdited = TRUE;
	NSLog(@"Cin edited");
}

- (IBAction)keynameAdd:(id)sender {
	[k addRow:[listKeyname selectedRow]];
	[listKeyname reloadData];
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
	[c addRow:[listChardef selectedRow]];
	[listChardef reloadData];
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

- (IBAction) save:(id)sender {
	// [self saveExec];
}

- (IBAction) saveAs:(id)sender {
	NSSavePanel *savePanel= [NSSavePanel savePanel];
	[savePanel setRequiredFileType:@"cin"];
	[savePanel setCanCreateDirectories:NO];
	[savePanel setAllowsOtherFileTypes:NO];
	[savePanel setTitle:MSG(@"Save")];	
	[savePanel setMessage:MSG(@"Savin cin file")];
	if ([savePanel runModalForDirectory:[NSString stringWithUTF8String:OVCINPATH] file:@"new.cin"] != NSOKButton) {
		return;
	}
	NSString *filepath = [[savePanel filename] stringByStandardizingPath];
	[self saveExec:filepath];
}

-(void) confirmToSave {
	if(isEdited) {
		ALERT(@"You changes will be lost!", @"If you do not save file before closing window, your changes will be lost. Do you want to save?", @"Save", @"Continue Editing");
	}	
}

- (void) makeNewCin {
	NSLog(@"Generate New Input Method");
	[winCin setTitle:@"Untitled Cin file"];
	[txtEname setStringValue:@"New Input Method"];
	[txtCname setStringValue:@"New Input Method"];
	[txtTcname setStringValue:@"New Input Method"];
	[txtScname setStringValue:@"New Input Method"];
	[txtSelkey setStringValue:@"1234567890"];
	k = [keyTable alloc];
	[k init];
	int i;
	for(i = 0; i < 26; i++) {
		NSMutableDictionary *d=[NSMutableDictionary new];		
		[d setObject:[NSNumber numberWithInt:FALSE] forKey:@"endkey"];
		unichar c = 'a' + i;
		NSString *a = [[NSString alloc] initWithCharacters:&c length:1];		
		[d setObject:a forKey:@"key"];
		[d setObject:a forKey:@"value"];
		[k addKey: d];
		[d release];		
	}	
	[listKeyname setDataSource:k];	
	c = [charTable alloc];
	[c init];
	[listChardef setDataSource:c];	
	isEdited = FALSE;
	[winCin orderFront:self];
	[winCin center];	
}

- (IBAction) newCin:(id)sender {
	[self confirmToSave];
	[self makeNewCin];
}

- (void)loadCin: (NSString *) cinPath {
	[winLoad orderFront:self];
	[indicatorLoad startAnimation:self];
	OVCIN *cin = new OVCIN();
	cin->load([[cinPath stringByDeletingLastPathComponent] UTF8String], [[cinPath lastPathComponent] UTF8String]);
	NSString *shortflename = [NSString stringWithUTF8String: cin->get_filename()];	
	NSString *ename = [NSString stringWithUTF8String: cin->get_ename()];
	NSString *cname = [NSString stringWithUTF8String: cin->get_cname()];
	NSString *tcname = [NSString stringWithUTF8String: cin->get_tcname()];
	NSString *scname = [NSString stringWithUTF8String: cin->get_scname()];	
	NSString *selkey = [NSString stringWithUTF8String: cin->get_selkey()];
	int i;
	k = [keyTable alloc];
	[k init];
	for(i = 0; i < cin->keycount(); i++) {
		Keyname keyname=cin->get_key(i);
		NSMutableDictionary *d=[NSMutableDictionary new];
		if(strstr(cin->get_endkey(), keyname.key.c_str())) {
			[d setObject:[NSNumber numberWithInt:TRUE] forKey:@"endkey"];
		} else {
			[d setObject:[NSNumber numberWithInt:FALSE] forKey:@"endkey"];			
		}
		[d setObject:[NSString stringWithUTF8String:keyname.key.c_str()] forKey:@"key"];
		[d setObject:[NSString stringWithUTF8String:keyname.value.c_str()] forKey:@"value"];
		[k addKey: d];
		[d release];		
	}
	[listKeyname setDataSource:k];

	c = [charTable alloc];
	[c init];
	for(i = 0; i < cin->charcount(); i++) {
		Chardef chardef=cin->get_char(i);
		NSMutableDictionary *d=[NSMutableDictionary new];		
		[d setObject:[NSString stringWithUTF8String:chardef.key.c_str()] forKey:@"key"];
		[d setObject:[NSString stringWithUTF8String:chardef.value.c_str()] forKey:@"value"];
		[c addKey: d];
	}
	[listChardef setDataSource:c];	

	[winCin setTitle:shortflename];
	[txtEname setStringValue:ename];
	[txtCname setStringValue:cname];
	[txtTcname setStringValue:tcname];	
	[txtScname setStringValue:scname];	
	[txtSelkey setStringValue:selkey];
	isEdited = FALSE;
	[indicatorLoad stopAnimation:self];	
	[winLoad orderOut:self];	
	[winCin orderFront:self];
	[winCin center];	
}

- (void)open {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    NSString *ovCinPath = [[NSString stringWithUTF8String:OVCINPATH] stringByStandardizingPath];	
    [panel setAllowsMultipleSelection:NO];
    [panel setCanCreateDirectories:NO];
    [panel setMessage:MSG(@"Please choose the cin file which you want to edit:")];
    if([panel runModalForDirectory:ovCinPath file:nil
							 types:[NSArray arrayWithObjects:@"cin", nil]]  == NSOKButton) {
		NSArray *filenames = [panel filenames];
		if(![filenames count]) return;
		[self loadCin:[[filenames objectAtIndex:0] stringByStandardizingPath]];
    }
    return;	
}


- (IBAction) openCin:(id)sender {
	[self open];
}

- (void)awakeFromNib {

}

- (BOOL)windowShouldClose:(id)sender {
	return TRUE;
}

@end