#import "Delegate.h"

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


@implementation Delegate

- (BOOL)pathExists:(const char *)cp {
	NSString *p=[[NSString stringWithUTF8String:cp] stringByStandardizingPath];
    struct stat st;
    if (stat([p UTF8String], &st)) return NO;
    return YES;
}

- (void)addItem:(NSString*)path {
	NSMutableDictionary *d=[NSMutableDictionary new];
	
	NSArray *pathComponents = [path pathComponents];
	NSString *filename = [pathComponents objectAtIndex:([pathComponents count] -1)];
	
	NSImage *preview = [NSImage new];
	[preview initByReferencingFile:path];

	[d setObject:filename forKey:@"filename"];	
	[d setObject:path forKey:@"path"];
	[d setObject:preview forKey:@"preview"];	
	[items addObject:d];
	[d release];
}

- (void)awakeFromNib  {
	items = [NSMutableArray new];
	
	NSArray *pics = [[NSBundle mainBundle] pathsForResourcesOfType:@"icns" inDirectory:nil];
	int i;
	for (i=0; i<[pics count]; i++) {
		[self addItem:[pics objectAtIndex: i]];
	} 
	
	[listview setRowHeight:20];
	// [listview setAllowsColumnReordering:true];
	[listview setDataSource:self];
	[listview setDoubleAction:@selector(preview)];
}

- (void)preview {
	NSString *cmd =[NSString stringWithFormat:@"open -a Preview %@", [[items objectAtIndex:[listview selectedRow]] objectForKey:@"path"]];	
	system([cmd UTF8String]);
	return;
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
	
	return nil;
}

- (void)windowWillClose:(NSNotification*)n {
	[[NSApplication sharedApplication] terminate:self];
}

- (void) CopyToTarget:(const char *) target source: (NSString *)source script: (NSMutableString *)script{
	NSString *t = [NSString stringWithUTF8String:target];
	NSString *line = [NSString stringWithFormat:@"/bin/cp %@ %@ \n", source, t];
	NSLog(line);
	[script appendString:line];
}

- (IBAction)changeIcon:(id)sender
{
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
		[self CopyToTarget: LCP OV072 MENUICON source: iconpath script:script];
	}
	
	if ([self pathExists:LCP OV072TC]) {
		[self CopyToTarget: LCP OV072TC MENUICON source: iconpath script:script];
	}
	
	if ([self pathExists:LCP OV072SC]) {
		[self CopyToTarget: LCP OV072SC MENUICON source: iconpath script:script];		
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
	
	// system("/usr/bin/osascript -e \"tell application \\\"Finder\\\" to restart\"");	
	
}

@end
