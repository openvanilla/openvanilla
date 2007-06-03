#import "Delegate.h"
#import "IconFamily.h"

#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]

#define LCP			"/Library/Components"
#define OV063		"/OVLoader.bundle"
#define OV07x		"/OVInit.bundle"
#define OV072		"/OpenVanilla-TSComponent-0.7.2.bundle"
#define OV072TC		"/OpenVanilla-TSComponent-0.7.2-TC.bundle"
#define OV072SC		"/OpenVanilla-TSComponent-0.7.2-SC.bundle"
#define MENUICON	"/Contents/Resources/OpenVanillaMenuIcon.icns"
#define USERICONPATH	"~/Library/OpenVanilla/0.7.2/Icons"

@implementation Delegate

- (NSString *)takeFilenameFromPath: (NSString *)path {
	if([path length]) {
		NSArray *pathComponents = [path pathComponents];
		NSString *filename = [pathComponents lastObject];	
		return filename;
	}
	return @"";
}

- (BOOL)pathExists:(const char *)cp {
	NSString *p=[[NSString stringWithUTF8String:cp] stringByStandardizingPath];
    struct stat st;
    if (stat([p UTF8String], &st)) return NO;
    return YES;
}

- (void) checkUserIconFolder {
	if(![self pathExists:USERICONPATH]) {
		mkdir([[[NSString stringWithUTF8String:USERICONPATH] stringByStandardizingPath] UTF8String], 0000755);
		NSLog(@"Path not exists!");
	}
}

- (void)addItem:(NSString*)path type:(NSString*)icontype {
	if(![self pathExists: [path UTF8String]]) return;

	NSMutableDictionary *d=[NSMutableDictionary new];
	NSString *filename = [self takeFilenameFromPath:path];
	
	if([filename isEqualToString:@"DisplayServerIcon.icns"]) return; // We do not use the icon of this application
	NSImage *preview = [NSImage new];
	[preview initByReferencingFile:path];
	[preview setSize:NSMakeSize(16.0, 16.0)];

	[d setObject:filename forKey:@"filename"];
	[d setObject:icontype forKey:@"type"];	
	[d setObject:path forKey:@"path"];
	[d setObject:preview forKey:@"preview"];
	[items addObject:d];
	[d release];
}


- (void)scanIcons {
	items = [NSMutableArray new];
	
	NSArray *icons = [[NSBundle mainBundle] pathsForResourcesOfType:@"icns" inDirectory:nil];
	int i;
	for (i=0; i<[icons count]; i++) {
		[self addItem:[icons objectAtIndex: i] type: @"Built-in"];
	}
	if(![self pathExists:USERICONPATH]) return;
	NSString *usericonpath =[[NSString stringWithUTF8String:USERICONPATH] stringByStandardizingPath];
	NSDirectoryEnumerator *direnum = [[NSFileManager defaultManager] enumeratorAtPath:usericonpath];
	NSString *pname;
	while(pname = [direnum nextObject]) {
		if ([pname hasSuffix: @"icns"]) {
			[self addItem:[usericonpath stringByAppendingPathComponent: pname] type: @"User"];
		}
		[direnum skipDescendents];
	}
}

-	(void) refresh {
	[self scanIcons];
	[listview reloadData];	
}

- (void)awakeFromNib  {
	[self scanIcons];
	[listview setDataSource:self];	
	[listview setRowHeight:24];
	[listview setDoubleAction:@selector(preview)];
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
	return [items count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
	if ([[aTableColumn identifier] isEqualToString:@"filename"]) {
		return [[items objectAtIndex:rowIndex] objectForKey:@"filename"];
	}
	if ([[aTableColumn identifier] isEqualToString:@"preview"]) {
		return [[items objectAtIndex:rowIndex] objectForKey:@"preview"];
	}
	
	if ([[aTableColumn identifier] isEqualToString:@"type"]) {
		return MSG([[items objectAtIndex:rowIndex] objectForKey:@"type"]);
	}
	
	return nil;
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
	
	NSString *filename = [self takeFilenameFromPath:path];
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

- (IBAction)openIcon:(id)sender {
	[self checkUserIconFolder];
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	[panel setAllowsMultipleSelection:YES];
	[panel setCanCreateDirectories:NO];
	[panel setMessage:MSG(@"Please choose the icon file which you want to add (File extension must be \".icns\".):")];
	if([panel runModalForTypes:[NSArray arrayWithObjects:@"icns", nil]]  == NSOKButton) {
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
	[panel setAllowsMultipleSelection:YES];
	[panel setCanCreateDirectories:NO];
	[panel setTitle:MSG(@"Choose the Source Image")];
	[panel setMessage:MSG(@"Please choose the image file that you would like to convert (File extension coud be \".jpeg\", \".gif\", \".png\", \".jsd\" and so on.):")];
	if([panel runModalForTypes:[NSArray arrayWithObjects:@"jpeg", @"jpg", @"gif", @"png", @"psd", nil]]  == NSOKButton) {
		NSArray *filenames = [panel filenames];
		int i;
		if(![filenames count]) return;
		for(i = 0; i < [filenames count]; i ++) {
			NSString *sfilename = [self takeFilenameFromPath:[filenames objectAtIndex:i]];
			NSSavePanel *savepanel= [NSSavePanel savePanel];
			[savepanel setRequiredFileType:@"icns"];
			[savepanel setCanCreateDirectories:NO];
			[savepanel setAllowsOtherFileTypes:NO];
			[savepanel setTitle:MSG(@"Save the Converted Icon")];	
			[savepanel setMessage:[NSString stringWithFormat:MSG(@"Please set the filename of the icon converted from \"%@\":"), sfilename]];
			if ([savepanel runModalForDirectory:[NSString stringWithUTF8String:USERICONPATH] file:@"newicon.icns"] != NSOKButton) {
				continue;
			}
			NSString *filepath = [[savepanel filename] stringByStandardizingPath];				
			NSImage *icon = [NSImage new];

			[icon initWithContentsOfFile:[filenames objectAtIndex:i]];
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
			// [iconFamily setAsCustomIconForFile:filepath];
			
			/* Save to jpeg
			NSBitmapImageRep *imageRep = [NSBitmapImageRep imageRepWithData:[ricon TIFFRepresentation]];
			NSDictionary *imageProps = [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:0.9] forKey:NSImageCompressionFactor];
			NSData *photoData = [NSData data];
			photoData = [imageRep representationUsingType:NSJPEGFileType properties:imageProps];
			[photoData writeToFile:[@"~/Desktop/test.jpg" stringByStandardizingPath] atomically:NO];
			 */
		}
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

- (void)preview {
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
	if([listview numberOfSelectedRows] != 1) {
		NSAlert *errorbox=[NSAlert
		alertWithMessageText:MSG(@"Error: Please select one of the icons in the list.")
			   defaultButton:MSG(@"OK")
			 alternateButton:nil
				 otherButton:nil
   informativeTextWithFormat:MSG(@"It seems that you did not select any icon, or you selected whole column but not any unique icon.")];
		[errorbox runModal];
		return;
	}
	
	NSString *iconpath = [[items objectAtIndex:[listview selectedRow]] objectForKey:@"path"];
	NSString *iconfilename = [[items objectAtIndex:[listview selectedRow]] objectForKey:@"filename"];
	
	NSAlert *beginbox=[NSAlert
		alertWithMessageText:MSG(@"Chaning the icon of OpenVanilla")
			   defaultButton:MSG(@"Proceed")
			 alternateButton:MSG(@"Cancel")
				 otherButton:nil
   informativeTextWithFormat:MSG(@"Do you really want to change the icon to \"%@\" ?"), iconfilename];	
	
	if ([beginbox runModal]==0) return;	
	
	char tmpfn[L_tmpnam];
	tmpnam(tmpfn);
	NSLog(@"Temp script file created as %s", tmpfn);
	NSMutableString *script=[NSMutableString new];
	[script autorelease];	
	
	if ([self pathExists:LCP OV072]) {
		[self copyToTarget: LCP OV072 MENUICON source: iconpath script:script];
	}
	
	if ([self pathExists:LCP OV072TC]) {
		[self copyToTarget: LCP OV072TC MENUICON source: iconpath script:script];
	}
	
	if ([self pathExists:LCP OV072SC]) {
		[self copyToTarget: LCP OV072SC MENUICON source: iconpath script:script];		
	}
	
	NSLog(@"Scripts: %d", [script length]);
	
	if([script length] ==0) {
		NSAlert *errorbox=[NSAlert
		alertWithMessageText:MSG(@"You did not install any newer version then 0.72 of OpenVanilla!")
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
		alertWithMessageText:MSG(@"System Restart Required")
			   defaultButton:MSG(@"Restart")
			 alternateButton:nil
				 otherButton:nil
   informativeTextWithFormat:MSG(@"Restart Explanation")];	
	
	[endbox runModal];
	
	system("/usr/bin/osascript -e \"tell application \\\"Finder\\\" to restart\"");		
}

- (void)windowWillClose:(NSNotification*)n {
	[[NSApplication sharedApplication] terminate:self];
}

@end
