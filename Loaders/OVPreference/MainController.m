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
	
	[self initToolbar];
	[[self window] center];
	[self loadPlist];
	[self initModules];

	[_moduleListTableView setDataSource:self];
	[_moduleListTableView setDelegate:self];
	
	[self setActiveView:_moduleSettingView animate:YES];
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
	return [prefPath stringByAppendingPathComponent:@"org.openvanilla.plist"];
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
	NSDictionary *d = [_settingDictionary valueForKey:@"OVIMArray"];
	OVIMArrayController *arrayController = [[[OVIMArrayController alloc] initWithIdentifier:@"OVIMArray" nibName:@"OVIMArrayController"] autorelease];
	[arrayController setDictinary:d];
	[arrayController setLocalizedName:@"Array"];
	[_controllersArray addObject:arrayController];
	
	d = [_settingDictionary valueForKey:@"OVIMPhonetic"];
	OVIMPhoneticController *phoneticController = [[[OVIMPhoneticController alloc] initWithIdentifier:@"OVIMPhonetic" nibName:@"OVIMPhoneticController"] autorelease];	
	[phoneticController setDictinary:d];
	[phoneticController setLocalizedName:@"Phonetic"];
	[_controllersArray addObject:phoneticController];

	d = [_settingDictionary valueForKey:@"OVIMTibetan"];
	OVIMTibetanController *tibetanController = [[[OVIMTibetanController alloc] initWithIdentifier:@"OVIMTibetan" nibName:@"OVIMTibetanController"] autorelease];	
	[tibetanController setDictinary:d];
	[tibetanController setLocalizedName:@"Tibetan"];
	[_controllersArray addObject:tibetanController];
	
	[_moduleListTableView reloadData];
	if ([_controllersArray count]) {
		[_moduleListTableView selectRowIndexes:[NSIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
		NSView *view = [[_controllersArray objectAtIndex:0] view];
		[self setActiveModuleSettingView:view];
	}

}

#pragma mark NSWindowNotifications

- (void)windowWillClose:(NSNotification *)notification
{
	[NSApp terminate:self];
}

@end
