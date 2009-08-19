//
//  MainController.h
//  OVPreference

#import <Cocoa/Cocoa.h>


@interface MainController : NSWindowController 
{
	NSMutableDictionary *_settingDictionary;
	NSMutableArray *_controllersArray;
	
	IBOutlet NSView *_loaderSettingView;
	IBOutlet NSView *_moduleSettingView;
	IBOutlet NSView *_moduleContentView;
	IBOutlet NSTableView *_moduleListTableView;
}

- (NSString *)plistFilePath;
- (void)loadPlist;
- (void)writePlist;
- (void)initModules;

@end
