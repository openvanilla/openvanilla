//
//  MainController.m
//  OVPreference

#import "MainController.h"
#import "MainController+Toolbar.h"
#import "MainController+TableView.h"
#import "OVViewController.h"
#import "OVIMArrayController.h"
#import "OVIMPhoneticController.h"
#import "OVIMTibetanController.h"
#import "OVIMGenericController.h"

@implementation MainController

- (void) dealloc
{
	[_settingDictionary release];
	[_controllersArray release];
	[super dealloc];
}
- (void)awakeFromNib
{
	_settingDictionary = [[NSMutableDictionary alloc] init];
	_controllersArray = [[NSMutableArray alloc] init];
	
//	[self initToolbar];
	[[self window] center];
	[self loadPlist];
	[self initModules];

	[_moduleListTableView setDataSource:self];
	[_moduleListTableView setDelegate:self];
	
	[self setActiveView:_moduleSettingView animate:YES];
	
//	_loader = (id<LVDOInterface>)[NSConnection rootProxyForConnectionWithRegisteredName:LVDOINTERFACE_PROTOCOL_NAME host:nil];
//	// TO DO: retain the object if run under 10.4
//	[(id)_loader setProtocolForProxy:@protocol(LVDOInterface)];
//
//	NSLog(@"loaded module identifiers: %@", [_loader loadedModuleList]);
}

- (NSString *)plistFilePath
{
	NSArray *libPaths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
	NSString *libPath = [libPaths objectAtIndex:0];
	NSString *prefPath = [libPath stringByAppendingPathComponent:@"Preferences"];
	
	BOOL isDir = NO;
	if (![[NSFileManager defaultManager] fileExistsAtPath:prefPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:prefPath attributes:nil];
	}
	if (!isDir) {
		[[NSFileManager defaultManager] removeItemAtPath:prefPath error:nil];
		[[NSFileManager defaultManager] createDirectoryAtPath:prefPath attributes:nil];
	}
	return [prefPath stringByAppendingPathComponent:OPENVANILLA_LOADER_PLIST_FILENAME];
}
- (void)loadPlist
{
	NSString *plistFilePath = [self plistFilePath];
	NSData *data = [NSData dataWithContentsOfFile:plistFilePath];
	NSPropertyListFormat format;
	NSString *error;
	id plist = [NSPropertyListSerialization propertyListFromData:data mutabilityOption:NSPropertyListImmutable format:&format errorDescription:&error];
	if ([plist isKindOfClass:[NSDictionary class]]) {
		[_settingDictionary setDictionary:plist];
	}
}
- (void)updateSettingWithIdentifier:(NSString *)identifier settings:(NSDictionary *)settings
{
	[_settingDictionary setValue:settings forKey:identifier];
	[self writePlist];
}
- (void)writePlist
{
	NSString *plistFilePath = [self plistFilePath];
	[_settingDictionary writeToFile:plistFilePath atomically:YES];
}
- (void)initModules
{
	_loader = (id<LVDOInterface>)[NSConnection rootProxyForConnectionWithRegisteredName:LVDOINTERFACE_PROTOCOL_NAME host:nil];
	// TO DO: retain the object if run under 10.4
	[(id)_loader setProtocolForProxy:@protocol(LVDOInterface)];
	
//	NSLog(@"loaded module identifiers: %@", [_loader loadedModuleList]);
	NSArray *moduleList = [_loader loadedModuleList];
	NSDictionary *module = nil;
	NSEnumerator *enumerator = [moduleList objectEnumerator];
	while (module = [enumerator nextObject]) {
		NSString *identifier = [module valueForKey:@"ModuleIdentifier"];
		NSString *localizedName = [module valueForKey:@"ModuleLocalizedName"];
		NSDictionary *d = [_settingDictionary valueForKey:identifier];
		if ([identifier isEqualToString:@"OVIMArray"]) {
			OVIMArrayController *arrayController = [[[OVIMArrayController alloc] initWithIdentifier:identifier nibName:@"OVIMArrayController"] autorelease];
			[arrayController setDictinary:d];
			[arrayController setLocalizedName:localizedName];
			[_controllersArray addObject:arrayController];
		}
		else if ([identifier isEqualToString:@"OVIMPhonetic"]) {
			OVIMPhoneticController *phoneticController = [[[OVIMPhoneticController alloc] initWithIdentifier:identifier nibName:@"OVIMPhoneticController"] autorelease];	
			[phoneticController setDictinary:d];
			[phoneticController setLocalizedName:localizedName];
			[_controllersArray addObject:phoneticController];
		}
		else if ([identifier isEqualToString:@"OVIMTibetan"]) {
			OVIMTibetanController *tibetanController = [[[OVIMTibetanController alloc] initWithIdentifier:identifier nibName:@"OVIMTibetanController"] autorelease];	
			[tibetanController setDictinary:d];
			[tibetanController setLocalizedName:localizedName];
			[_controllersArray addObject:tibetanController];
		}
		else if ([identifier hasPrefix:@"OVIMGeneric-"]) {
			OVIMGenericController *genericController = [[[OVIMGenericController alloc] initWithIdentifier:identifier nibName:@"OVIMGenericController"] autorelease];	
			[genericController setDictinary:d];
			[genericController setLocalizedName:localizedName];
			[_controllersArray addObject:genericController];
		}
	}
	
	[_moduleListTableView reloadData];
	if ([_controllersArray count]) {
		[_moduleListTableView selectRowIndexes:[NSIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
		NSView *view = [[_controllersArray objectAtIndex:0] view];
		[self setActiveModuleSettingView:view];
		[_titleLabel setStringValue:[[_controllersArray objectAtIndex:0] localizedName]];
	}

}

#pragma mark NSWindowNotifications

- (void)windowWillClose:(NSNotification *)notification
{
	[NSApp terminate:self];
}

@end
