// Delegate.m
//
// A Utility to change OpenVanilla's icon.
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

#import "Delegate.h"
#import "IconFamily.h"

#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]
#define imageTypes  [NSArray arrayWithObjects:@"jpeg", @"jpg", @"jpe", @"gif", @"png", @"eps", @"epi", @"epsf", @"epsi", @"ps", @"psd", @"tif", @"tiff", @"bmp", nil]

#define LCP			"/Library/Components"
#define OV063		"/OVLoader.bundle"
#define OV07x		"/OVInit.bundle"
#define OV080		"/OpenVanilla-0.8.bundle"
#define MENUICON	"/Contents/Resources/OpenVanillaMenuIcon.icns"
#define USERICONPATH	"~/Library/OpenVanilla/0.8/Icons"

@implementation Delegate

- (BOOL)pathExists:(const char *)cp {
    NSString *p=[[NSString stringWithUTF8String:cp] stringByStandardizingPath];
    struct stat st;
    if (stat([p UTF8String], &st)) return NO;
    return YES;
}

- (BOOL) validateType: (NSString *)ext {
	int i;
	for(i = 0; i < [imageTypes count]; i++) {
		if([ext isEqualToString:[imageTypes objectAtIndex:i]]) {
			return YES;
		}
	}
	return NO;
}

- (void) checkUserIconFolder {
    if(![self pathExists:USERICONPATH]) {
	mkdir([[[NSString stringWithUTF8String:USERICONPATH] stringByStandardizingPath] UTF8String], 0000755);
	NSLog(@"Path not exists!");
    }
}

- (void)addItem:(NSString*)path type:(NSString*)icontype size: (float)size {
    if(![self pathExists: [path UTF8String]]) return;

    NSMutableDictionary *d=[NSMutableDictionary new];
    NSString *filename = [path lastPathComponent];

    if([filename isEqualToString:@"OVIconChanger.icns"]) return; // We do not use the icon of this application
    NSImage *preview = [NSImage new];
    [preview initByReferencingFile:path];
    [preview setSize:NSMakeSize(size, size)];

    [d setObject:[filename stringByDeletingPathExtension] forKey:@"filename"];
    [d setObject:icontype forKey:@"type"];	
    [d setObject:path forKey:@"path"];
    [d setObject:preview forKey:@"preview"];
    [items addObject:d];
    [d release];
}

- (void)scanIcons:(float) size{
    items = [NSMutableArray new];

    if([self pathExists:USERICONPATH]){
	NSString *usericonpath =[[NSString stringWithUTF8String:USERICONPATH] stringByStandardizingPath];
	NSDirectoryEnumerator *direnum = [[NSFileManager defaultManager] enumeratorAtPath:usericonpath];
	NSString *pname;
	while(pname = [direnum nextObject]) {
	    if ([pname hasSuffix: @"icns"]) {
		[self addItem:[usericonpath stringByAppendingPathComponent: pname] type: @"User" size:size];
	    }
	    [direnum skipDescendents];
	}
    }
    NSArray *icons = [[NSBundle mainBundle] pathsForResourcesOfType:@"icns" inDirectory:nil];
    int i;
    for (i=0; i<[icons count]; i++) {
	[self addItem:[icons objectAtIndex: i] type: @"Built-in" size:size];
    }	
}

- (void) refresh {
    [self scanIcons:currSize];
    [listview reloadData];
}

- (void) copyToUserDir: (NSString *) path {
    if(![self pathExists:[path UTF8String]]) {
		NSAlert *errorbox=[NSAlert
	    alertWithMessageText:MSG(@"Error: File does not exist!")
			   defaultButton:MSG(@"OK")
			 alternateButton:nil
				 otherButton:nil
   informativeTextWithFormat:MSG(@"The icon that you want to add exists no more, you might deleted it, or some unknown errors happened.")];
		[errorbox runModal];
		return;
    }
	
    NSString *filename = [path lastPathComponent];
    NSString *targetfilepath = [NSString stringWithFormat:@"%@/%@", [NSString stringWithUTF8String:USERICONPATH], filename];
    if([self pathExists:[targetfilepath UTF8String]]) {
		NSAlert *errorbox=[NSAlert
	    alertWithMessageText:MSG(@"Error: File alreay exists!")
			   defaultButton:MSG(@"OK")
			 alternateButton:nil
				 otherButton:nil
   informativeTextWithFormat:MSG(@"You have already have a icon file called \"%@\". You can not copy it again."), filename];
		[errorbox runModal];
		return;	
    }	
    NSString *cmd = [NSString stringWithFormat:@"/bin/cp %@ %@ \n", path, targetfilepath];
    system([cmd UTF8String]);
    return;
}

- (void) convertLoop:(NSArray*)filenames {
	int i;
	if(![filenames count]) return;
	for(i = 0; i < [filenames count]; i ++) {
		NSString *ext = [[filenames objectAtIndex:i] pathExtension];
		if([self validateType:ext] == NO) {
			continue;
		}
		if([ext isEqualToString:@"icns"]) {
			[self copyToUserDir:[filenames objectAtIndex:i]];
			continue;
		}
	    NSString *sfilename = [[filenames objectAtIndex:i] lastPathComponent];
	    NSString *tfilename = [[sfilename stringByDeletingPathExtension] stringByAppendingString:@".icns"];
	    NSSavePanel *savepanel= [NSSavePanel savePanel];
	    [savepanel setRequiredFileType:@"icns"];
	    [savepanel setCanCreateDirectories:NO];
	    [savepanel setAllowsOtherFileTypes:NO];
	    [savepanel setTitle:MSG(@"Save the Converted Icon")];	
	    [savepanel setMessage:[NSString stringWithFormat:MSG(@"Please set the filename of the icon converted from \"%@\":"), sfilename]];
	    if ([savepanel runModalForDirectory:[NSString stringWithUTF8String:USERICONPATH] file:tfilename] != NSOKButton) {
			continue;
	    }
	    NSString *filepath = [[savepanel filename] stringByStandardizingPath];				
	    NSImage *icon = [NSImage new];
		
	    [icon initWithContentsOfFile:[filenames objectAtIndex:i]];
		
	    if(![icon isValid]) {
			NSAlert *errorbox=[NSAlert
		    alertWithMessageText:MSG(@"Error: Invalid Image file!")
				   defaultButton:MSG(@"OK")
				 alternateButton:nil
					 otherButton:nil
	   informativeTextWithFormat:MSG(@"The selected file is invalid, please check your file.")];
			[errorbox runModal];				
			return;
	    }
	    [icon setScalesWhenResized:TRUE];
	    NSImage *ricon = [[[NSImage alloc] initWithSize:NSMakeSize(16, 16)] autorelease];
	    [ricon lockFocus];
	    [[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];
	    [icon setSize:NSMakeSize(16, 16)];
	    [icon compositeToPoint:NSZeroPoint operation:NSCompositeCopy];
	    [ricon unlockFocus];			
		
	    IconFamily* iconFamily;
		
	    iconFamily = [IconFamily ovicon:ricon];			
	    [iconFamily writeToFile:filepath];			
	    [iconFamily setAsCustomIconForFile:filepath];
	}	
}

- (void)awakeFromNib  {
	[window center];
    currSize = 16;
    [self scanIcons:currSize];
    [listview registerForDraggedTypes:
		[NSArray arrayWithObjects:NSFilenamesPboardType,nil]];	
    [listview setDataSource:self];	
    [listview setRowHeight:24];
    [listview setDoubleAction:@selector(preview)];
}

- (NSDragOperation)tableView:(NSTableView*)tv validateDrop:(id <NSDraggingInfo>)info proposedRow:(int)row proposedDropOperation:(NSTableViewDropOperation)op {
    [tv setDropRow:[tv numberOfRows] dropOperation:NSTableViewDropAbove];
    return NSTableViewDropAbove;
}

- (BOOL)tableView:(NSTableView*)tv acceptDrop:(id <NSDraggingInfo>)info row:(int)row dropOperation:(NSTableViewDropOperation)op
{
    NSPasteboard *myPasteboard=[info draggingPasteboard];
    NSArray *typeArray=[NSArray arrayWithObjects:NSFilenamesPboardType,nil];
    NSString *availableType;
    NSArray *filesList;
	
    availableType=[myPasteboard availableTypeFromArray:typeArray];
    filesList=[myPasteboard propertyListForType:availableType];
	[self convertLoop:filesList];
	[self refresh];
    return YES;
}

- (BOOL)tableView:(NSTableView *)tableView shouldEditTableColumn:(NSTableColumn *)tableColumn row:(int)row{ 
	return NO; 
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
    return [items count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
    if ([[aTableColumn identifier] isEqualToString:@"filename"]) {
	return MSG([[items objectAtIndex:rowIndex] objectForKey:@"filename"]);
    }
    if ([[aTableColumn identifier] isEqualToString:@"preview"]) {
	return [[items objectAtIndex:rowIndex] objectForKey:@"preview"];
    }

    if ([[aTableColumn identifier] isEqualToString:@"type"]) {
	return MSG([[items objectAtIndex:rowIndex] objectForKey:@"type"]);
    }

    return nil;
}

- (IBAction)openIcon:(id)sender {
    [self checkUserIconFolder];
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    NSString *importsPath = [ @"~/Desktop" stringByStandardizingPath ];	
    [panel setAllowsMultipleSelection:YES];
    [panel setCanCreateDirectories:NO];
    [panel setMessage:MSG(@"Please choose the icon file which you want to add (File extension must be \".icns\".):")];
    if([panel runModalForDirectory:importsPath file:nil
	    types:[NSArray arrayWithObjects:@"icns", nil]]  == NSOKButton) {
	NSArray *filenames = [panel filenames];
	int i;
	if(![filenames count]) return;
	for(i = 0; i < [filenames count]; i ++) {
	    [self copyToUserDir:[[filenames objectAtIndex:i] stringByStandardizingPath]];
	}
	NSAlert *msgbox=[NSAlert
	    alertWithMessageText:MSG(@"Done!")
	    defaultButton:MSG(@"OK")
	    alternateButton:nil
	    otherButton:nil
	    informativeTextWithFormat:MSG(@"Complete copying.")];
	[msgbox runModal];
	[self refresh];	
    }
    return;	
}

- (IBAction)convertIcon:(id)sender {
    [self checkUserIconFolder];
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    NSString *importsPath = [ @"~/Pictures" stringByStandardizingPath ];
    [panel setAllowsMultipleSelection:YES];
    [panel setCanCreateDirectories:NO];
    [panel setTitle:MSG(@"Choose the Source Image")];
    [panel setMessage:MSG(@"Please choose the image file that you want to convert:")];
    if([panel runModalForDirectory:importsPath file:nil types:imageTypes]  == NSOKButton) {
		[self convertLoop:[panel filenames]];
    }
    [self refresh];	
    return;	
}

- (IBAction)openIconFolder:(id)sender {
    [self checkUserIconFolder];
    NSString *folderpath = [[NSString stringWithUTF8String:USERICONPATH] stringByStandardizingPath];
    NSString *cmd = [@"open -a Finder " stringByAppendingFormat:folderpath];
    system([cmd UTF8String]);
    return;
}

- (IBAction)homepage:(id)sender {
    system("open http://openvanilla.org");
    return;
}

- (BOOL)checkSelection {
    if([listview numberOfSelectedRows] != 1) {
	NSAlert *errorbox=[NSAlert
	    alertWithMessageText:MSG(@"Error: Please select one of the icons in the list.")
	    defaultButton:MSG(@"OK")
	    alternateButton:nil
	    otherButton:nil
	    informativeTextWithFormat:MSG(@"It seems that you did not select any icon, or you selected whole column but not any unique icon.")];
	[errorbox runModal];
	return FALSE;
    }
    return TRUE;
}

- (IBAction)deleteIcon:(id)sender {
    if(![self checkSelection]) return;	
    NSString *type = [[items objectAtIndex:[listview selectedRow]] objectForKey:@"type"];
    if([type isEqualToString:@"Built-in"]) {
	NSAlert *errorbox=[NSAlert
	    alertWithMessageText:MSG(@"Error: You cannot delete the built-in icons.")
	    defaultButton:MSG(@"OK")
	    alternateButton:nil
	    otherButton:nil
	    informativeTextWithFormat:MSG(@"You can delete your own customzied icons only.")];
	[errorbox runModal];		
	return;
    }

    NSString *path = [[items objectAtIndex:[listview selectedRow]] objectForKey:@"path"];
    NSWorkspace *ws = [NSWorkspace sharedWorkspace];
    [ws performFileOperation:NSWorkspaceRecycleOperation
	source:[path stringByDeletingLastPathComponent] destination:@""
	files:[NSArray arrayWithObject:[path lastPathComponent]] tag:0];
    [self refresh];
}

- (void)preview {
    if(![self checkSelection]) return;
    NSString *cmd =[NSString stringWithFormat:@"open -a Preview %@", [[items objectAtIndex:[listview selectedRow]] objectForKey:@"path"]];	
    system([cmd UTF8String]);
    return;
}

- (IBAction)previewIcon:(id)sender {
    return [self preview];
}

- (void) copyToTarget:(const char *) target source: (NSString *)source script: (NSMutableString *)script{
    NSString *t = [NSString stringWithUTF8String:target];
    NSString *line = [NSString stringWithFormat:@"/bin/cp %@ %@ \n", source, t];
    NSLog(line);
    [script appendString:line];
}

- (IBAction)changeIcon:(id)sender {
    if(![self checkSelection]) return;	
    NSString *iconpath = [[items objectAtIndex:[listview selectedRow]] objectForKey:@"path"];
    NSString *iconfilename = [[items objectAtIndex:[listview selectedRow]] objectForKey:@"filename"];

    NSAlert *beginbox=[NSAlert
	alertWithMessageText:MSG(@"Chaning the icon of OpenVanilla")
	defaultButton:MSG(@"Proceed")
	alternateButton:MSG(@"Cancel")
	otherButton:nil
	informativeTextWithFormat:MSG(@"Do you really want to change the icon to \"%@\" ?"), MSG(iconfilename)];

    if ([beginbox runModal]==0) return;	

    char tmpfn[L_tmpnam];
    tmpnam(tmpfn);
    NSLog(@"Temp script file created as %s", tmpfn);
    NSMutableString *script=[NSMutableString new];
    [script autorelease];
	
    if ([self pathExists:LCP OV080]) {
		[self copyToTarget: LCP OV080 MENUICON source: iconpath script:script];
    }
	/*
    if ([self pathExists:LCP OV080TC]) {
		[self copyToTarget: LCP OV080TC MENUICON source: iconpath script:script];
    }
	
    if ([self pathExists:LCP OV080SC]) {
		[self copyToTarget: LCP OV080SC MENUICON source: iconpath script:script];		
    }*/	

    NSLog(@"Scripts: %d", [script length]);

    if([script length] ==0) {
	NSAlert *errorbox=[NSAlert
	    alertWithMessageText:MSG(@"You did not install any newer version then 0.8.0 of OpenVanilla!")
	    defaultButton:MSG(@"OK")
	    alternateButton:nil			
	    otherButton:nil
	    informativeTextWithFormat:MSG(@"An installation of OpenVanilla or newer version is required. The process of chaning icon will be terminated.")];
	[errorbox runModal];
	return;
    }

    NSLog(@"Generated script:\n=== BEGIN OF SCRIPT ===\n%@=== END OF SCRIPT ===", script);

    BOOL result=NO;
    if ([NSString instancesRespondToSelector:@selector(writeToFile:atomically:encoding:error:)]) {
	result=[script writeToFile:[NSString stringWithUTF8String:tmpfn] atomically:YES encoding:NSUTF8StringEncoding error:NULL];
    }
    else {
	result=[script writeToFile:[NSString stringWithUTF8String:tmpfn] atomically:YES];
    }

    if (!result) {
	NSLog(@"Error: Failed to create script file");
	return;
    }

    AuthorizationFlags authflags = kAuthorizationFlagDefaults;
    AuthorizationRef authref;
    OSStatus s;
    s = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, authflags, &authref);
    if (s != errAuthorizationSuccess) {
	NSLog(@"Authorization failed: on creation");
    }

    AuthorizationItem reqitem = {kAuthorizationRightExecute, 0, NULL, 0};
    AuthorizationRights reqright = {1, &reqitem};

    authflags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed |
	kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;

    s = AuthorizationCopyRights (authref, &reqright, NULL, authflags, NULL );
    if (s != errAuthorizationSuccess) {
	NSLog(@"Authorization failed: on copying rights");
    }

    const char *cmdpath="/bin/sh";
    char *argv[] = {tmpfn, NULL};
    FILE *fpipe = NULL;
    char buf[128];

    s = AuthorizationExecuteWithPrivileges(authref, cmdpath, kAuthorizationFlagDefaults, argv, &fpipe);

    if (s == errAuthorizationSuccess)
    {
	for(;;) {
	    int b = read (fileno(fpipe), buf, sizeof(buf));
	    if (b < 1) break;
	    write (fileno(stderr), buf, b);
	}
	fclose(fpipe);
	AuthorizationFree (authref, kAuthorizationFlagDefaults);
    }

    AuthorizationFree (authref, kAuthorizationFlagDefaults);

    NSLog(@"deleting tmp file: %s", tmpfn);
    unlink(tmpfn);
    if (s != errAuthorizationSuccess) return;

    NSAlert *endbox=[NSAlert
	alertWithMessageText:MSG(@"Logout and Re-login Required")
	defaultButton:MSG(@"Logout")
	alternateButton:nil
	otherButton:nil
	informativeTextWithFormat:MSG(@"Please logout and re-login to make the effect take place.")];	

    [endbox runModal];

    system("/usr/bin/osascript -e \"tell application \\\"system events\\\" to log out\"");		
}

- (void)windowWillClose:(NSNotification*)n {
    [[NSApplication sharedApplication] terminate:self];
}

- (IBAction)refreshList:(id)sender {
    [self refresh];
}

- (IBAction)viewStanard:(id)sender {
    currSize = 16;
    [self refresh];
}

- (IBAction)viewBigger:(id)sender {
    currSize = 40;	
    [self refresh];	
}

@end
