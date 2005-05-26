#import "OVUninstaller.h"

@implementation OVUninstaller
- (void) awakeFromNib {
    filelist=[NSMutableArray new];
    script=[NSMutableString new];
    [filelistview setDataSource:self];
    [self prepareFilelist:NO];
}
- (void) dealloc {
    [filelist release];
    [script release];
    [super dealloc];
}

- (void)prepareFilelist:(BOOL)plist {
    [filelist removeAllObjects];
    [script setString:@""];

    system("find -d /Library/Components/OV* ! -regex \".*/Contents.*\" >> /tmp/_ovuninstlst");

    if (plist) {
        system("find -d ~/Library/Preferences/com.apple.HIToolbox* -maxdepth 1 >> /tmp/_ovuninstlst");
        system("find -d ~/Library/Preferences/ByHost/com.apple.HIToolbox* -maxdepth 1 >> /tmp/_ovuninstlst");        
    }
    system("find -d ~/Library/OpenVanilla >> /tmp/_ovuninstlst");
    system("find -d /Library/OpenVanilla ! -regex \".*/Contents.*\" >> /tmp/_ovuninstlst");
    system("find -d /Library/PreferencePanes/OV* -maxdepth 1 >> /tmp/_ovuninstlst");
    system("find -d /Library/Frameworks/Chewing* -maxdepth 1 >> /tmp/_ovuninstlst");
    system("find -d /Library/Frameworks/OpenVanilla* -maxdepth 1 >> /tmp/_ovuninstlst");
    system("find -d /usr/lib/libchewing* -maxdepth 1 >> /tmp/_ovuninstlst");
    system("find -d /usr/local/lib/libchewing* -maxdepth 1 >> /tmp/_ovuninstlst");


    FILE *f=fopen("/tmp/_ovuninstlst", "r");
    if (f) {
        char buf[256];
        while (!feof(f)) {
            bzero(buf, 256);
            fgets(buf, 255, f);
            if (strlen(buf)) {
                buf[strlen(buf)-1]=0;  // chomp 0xa
                NSString *fn=[NSString stringWithUTF8String:buf];
                [filelist addObject:fn];
                [script appendString:[NSString stringWithFormat:@"rm -rf %@\n", fn]];
            }
        }
        fclose(f);
    }
    unlink("/tmp/_ovuninstlst");
    
    [filelistview reloadData];
    [filelistview scrollRowToVisible:0];
    [uninstallscript setString:script];
}

// two delegated methods from NSTableView
- (id) tableView:(NSTableView*)t objectValueForTableColumn:(NSTableColumn *)c row:(int)r {
	return [filelist objectAtIndex:r]; 
}
- (int)numberOfRowsInTableView:(NSTableView *)t {
    return [filelist count];
}

// our own action methods
- (IBAction)previewScript:(id)sender
{
    [scriptwindow orderFront:self];
}

- (IBAction)removeHIToolBoxPlist:(id)sender
{
    if ([sender intValue])
        [self prepareFilelist:YES];
    else
        [self prepareFilelist:NO];
}

- (IBAction)setArchiveFile:(id)sender
{
}

- (IBAction)uninstall:(id)sender
{
}

@end
