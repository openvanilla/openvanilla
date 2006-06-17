#import "UninstallerDelegate.h"

#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]

#define LCP			"/Library/Components"
#define OV063		"/OVLoader.bundle"
#define OV07x		"/OVInit.bundle"
#define OV072		"/OpenVanilla-TSComponent-0.7.2.bundle"
#define OV072TC		"/OpenVanilla-TSComponent-0.7.2-TC.bundle"
#define OV072SC		"/OpenVanilla-TSComponent-0.7.2-SC.bundle"
#define INFOP		"/Contents/Resources/English.lproj/InfoPlist.strings"
#define OV063Pref	"/Library/PreferencePanes/OVPrefPane.prefPane"
#define VInput		"/VanillaInput.bundle"

@implementation UninstallerDelegate

- (BOOL)resource:(const char *)p contains:(NSString*)v {
	NSString *f;
	
	// determine if is OS X 10.4 or above
	if ([NSString instancesRespondToSelector:@selector(writeToFile:atomically:encoding:error:)]) {
		f=[NSString stringWithContentsOfFile:[NSString stringWithUTF8String:p] encoding:NSUTF8StringEncoding error:NULL];
	}
	else {
		f=[NSString stringWithContentsOfFile:[NSString stringWithUTF8String:p]];
	}
	if (!f) return NO;
	
	NSDictionary *d=[f propertyListFromStringsFileFormat];
	if (!d) return NO;
	
	id o=[d objectForKey:@"CFBundleName"];
	
	if (o)
		if ([o isEqual:v]) return YES;

	return NO;
}

- (BOOL)pathExists:(const char *)cp {
	NSString *p=[[NSString stringWithUTF8String:cp] stringByStandardizingPath];
    struct stat st;
    if (stat([p UTF8String], &st)) return NO;
    return YES;
}

- (void)addItem:(NSString*)ver path:(const char*)p checked:(BOOL)c {
	NSMutableDictionary *d=[NSMutableDictionary new];
	
	// by default, every item is checked
	[d setObject:[NSNumber numberWithInt:c] forKey:@"checked"];
	[d setObject:ver forKey:@"version"];
	[d setObject:[[NSString stringWithUTF8String:p] stringByStandardizingPath] forKey:@"path"];
	[versions addObject:d];
	[d release];
}

- (void)awakeFromNib  {
	versions = [NSMutableArray new];
	
	NSString *item=MSG(@"OpenVanilla (version unknown)");
	
	// detects version 0.7.2 (Unicode script): stable and beta
	if ([self pathExists:LCP OV072]) {
		item=MSG(@"OpenVanilla 0.7.2 (unknown branch)");
		
		if ([self resource:LCP OV072 INFOP contains:@"OpenVanilla 0.7.2"]) {
			item=MSG(@"OpenVanilla 0.7.2 stable");
		}
		else if ([self resource:LCP OV072 INFOP contains:@"OpenVanilla 0.7.2 (beta)"]) {
			item=MSG(@"OpenVanilla 0.7.2 beta");
		}
		else if ([self resource:LCP OV072 INFOP contains:@"OpenVanilla 0.7.2 (for Panther)"]) {
			item=MSG(@"OpenVanilla 0.7.2 stable (built for OS X 10.3.9)");
		}
		
		[self addItem:item path:LCP OV072 checked:YES];
	}


	// detects version 0.7.2 (Traditional Chinese script): stable and beta	
	if ([self pathExists:LCP OV072TC]) {
		item=MSG(@"OpenVanilla 0.7.2 (unknown branch, Traditional Chinese script)");

		if ([self resource:LCP OV072TC INFOP contains:@"OpenVanilla 0.7.2 (TC Script)"]) {
			item=MSG(@"OpenVanilla 0.7.2 stable (Traditional Chinese script)");
		}
		else if ([self resource:LCP OV072TC INFOP contains:@"OpenVanilla 0.7.2 (TC)"]) {
			item=MSG(@"OpenVanilla 0.7.2 beta (Traditional Chinese script)");
		}
		
		[self addItem:item path:LCP OV072TC checked:YES];
	}

	// detects version 0.7.2 (Simplified Chinese script): stable and else
	if ([self pathExists:LCP OV072SC]) {
		item=MSG(@"OpenVanilla 0.7.2 (unknown branch, Simplified Chinese script)");
		
		if ([self resource:LCP OV072SC INFOP contains:@"OpenVanilla 0.7.2 (SC Script)"]) {
			item=MSG(@"OpenVanilla 0.7.2 stable (Simplified Chinese script)");
		}
		
		[self addItem:item path:LCP OV072SC checked:YES];
	}


	// detects version 0.7.1 and 0.7.0
	if ([self pathExists:LCP OV07x]) {
		item=MSG(@"OpenVanilla 0.7.x (anything pre-0.7.2)");

		if ([self resource:LCP OV07x INFOP contains:@"OpenVanilla 0.7.1"]) {
			item=MSG(@"OpenVanilla 0.7.1");
		}
		else if ([self resource:LCP OV07x INFOP contains:@"OpenVanilla 0.7.0rc5"]) { 
			item=MSG(@"OpenVanilla 0.7.0rc5");
		}
		else if ([self resource:LCP OV07x INFOP contains:@"OpenVanilla 0.7.0rc4"]) { 
			item=MSG(@"OpenVanilla 0.7.0rc4");
		}
		else if ([self resource:LCP OV07x INFOP contains:@"OpenVanilla 0.7.0rc3"]) {
			item=MSG(@"OpenVanilla 0.7.0rc3");
		}
		
		[self addItem:item path:LCP OV07x checked:YES];
	}
	
	// detects version 0.6.3
	if ([self pathExists:LCP OV063]) {
		item=MSG(@"OpenVanilla 0.6.3");
		[self addItem:item path:LCP OV063 checked:YES];
	}

	// detects version 0.6.3 Prefernce Pane
	if ([self pathExists:OV063Pref]) {
		item=MSG(@"OpenVanilla 0.6.3 Preference Pane");
		[self addItem:item path:OV063Pref checked:YES];
	}

	
	// detects VanillaInput 0.06x
	if ([self pathExists:LCP VInput]) {
		item=MSG(@"VanillaInput 0.064 (or earlier)");
		[self addItem:item path:LCP VInput checked:YES];
	}

	// NSLog([versions description]);

	[listview setDataSource:self];
}

- (IBAction)uninstall:(id)sender
{
	NSAlert *beginbox=[NSAlert
		alertWithMessageText:MSG(@"Uninstalling OpenVanilla")
		defaultButton:MSG(@"Proceed")
		alternateButton:MSG(@"Cancel")
		otherButton:nil
		informativeTextWithFormat:MSG(@"Confirm to uninstall")];	
	
	if ([beginbox runModal]==0) return;
		
	// prepare for the temp name buffer
	char tmpfn[L_tmpnam];
	tmpnam(tmpfn);
	NSLog(@"Temp script file created as %s", tmpfn);

	// prepare the uninstallation script
	NSString *up=@"~/Library/Preferences";
	NSString *userpref=[up stringByStandardizingPath];
	NSMutableString *script=[NSMutableString new];
	[script autorelease];
	
	if ([resetCheckbox intValue]) {
		[script appendString:
			[NSString stringWithFormat:
				@"rm -rf %@/com.apple.HIToolBox.plist\n", userpref]
		];

		[script appendString:
			[NSString stringWithFormat:
				@"rm -rf %@/ByHost/com.apple.HIToolbox.*.plist\n", userpref]
		];
	}
	
	int i;
	for (i=0; i<[versions count]; i++) {
		NSDictionary *d=[versions objectAtIndex:i];
		if ([[d objectForKey:@"checked"] intValue]) {
			[script appendString:
				[NSString stringWithFormat:
					@"rm -rf %@\n", [d objectForKey:@"path"]
				]
			];
		}
	}

	NSLog(@"Generated script:\n=== BEGIN OF SCRIPT ===\n%@=== END OF SCRIPT ===", script);

	// OS X version check
	BOOL result=NO;
	if ([NSString instancesRespondToSelector:@selector(writeToFile:atomically:encoding:error:)]) {
		result=[script writeToFile:[NSString stringWithUTF8String:tmpfn] atomically:YES encoding:NSUTF8StringEncoding error:NULL];
	}
	else {
		result=[script writeToFile:[NSString stringWithUTF8String:tmpfn] atomically:YES];
	}

	if (!result) {
		NSLog(@"Uninstallation: Failed to create script file");
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
	}
	

    AuthorizationFree (authref, kAuthorizationFlagDefaults);
 
	NSLog(@"deleting tmp file: %s", tmpfn);
	unlink(tmpfn);
	
	
	NSAlert *endbox=[NSAlert
		alertWithMessageText:MSG(@"System Restart Required")
		defaultButton:MSG(@"Restart")
		alternateButton:nil
		otherButton:nil
		informativeTextWithFormat:MSG(@"Restart Explanation")];	
	
	[endbox runModal];

	system("/usr/bin/osascript -e \"tell application \\\"Finder\\\" to restart\"");
	// [[NSApplication sharedApplication] terminate];
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
	return [versions count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
	if ([[aTableColumn identifier] isEqualToString:@"version"]) {
		return [[versions objectAtIndex:rowIndex] objectForKey:@"version"];
	}
	if ([[aTableColumn identifier] isEqualToString:@"checked"]) {
		return [[versions objectAtIndex:rowIndex] objectForKey:@"checked"];
	}
	
	return nil;
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
	if ([[aTableColumn identifier] isEqualToString:@"checked"]) {
		[[versions objectAtIndex:rowIndex] setObject:anObject forKey:@"checked"];
	}
	
	NSLog([versions description]);
}


@end
