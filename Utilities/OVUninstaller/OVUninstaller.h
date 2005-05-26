/* OVUninstaller */

#import <Cocoa/Cocoa.h>

@interface OVUninstaller : NSObject
{
    IBOutlet id archivefiletag;
    IBOutlet id confirmwindow;
    IBOutlet id filelistview;
    IBOutlet id restartwindow;
    IBOutlet id scriptwindow;
    IBOutlet id uninstallscript;
    
    NSMutableArray *filelist;
    NSMutableString *script;
}
- (void)prepareFilelist:(BOOL)plist;
- (IBAction)previewScript:(id)sender;
- (IBAction)removeHIToolBoxPlist:(id)sender;
- (IBAction)setArchiveFile:(id)sender;
- (IBAction)uninstall:(id)sender;
@end
