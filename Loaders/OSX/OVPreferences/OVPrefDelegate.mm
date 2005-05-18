#import "OVPrefDelegate.h"
#import "CVWrappers.h"
#import "CVLoaderUtility.h"
#import "CVDictionary.h"
#import "CVKeyCode.h"
#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"
#import "CVPreviewView.h"

#define MSG(x)      x

@interface CVModuleItem : NSObject {
    NSString *modid;
    NSString *modname;
    NSString *shortcut;
    BOOL loaded;
}
- (id)initWithModuleID:(NSString*)i name:(NSString*)n shortcut:(NSString*)s loaded:(BOOL)e;
- (NSString*)name;
- (NSString*)identifier;
- (NSString*)shortcut;
- (BOOL)loaded;
- (void)setLoaded:(BOOL)e;
- (void)setShortcut:(NSString*)s;
@end

@implementation OVPrefDelegate
- (void)awakeFromNib {
    // set user interface state defaults
    modtab_modlist_currentrow=-1;
    fastimswitchkey=[[NSString alloc] initWithString:@""];
    sound=nil;
    gimcurrentindex=propeditcurrentindex=-1;
    gimmodlist=[NSMutableArray new];
    propeditmodlist=[NSMutableArray new];

    loader=[[CVEmbeddedLoader alloc] init];
    if (loader) {
//        NSLog(@"embedded loader inited");
//        NSLog([[loader config] description]);
//        NSLog([[loader moduleList] description]);
          NSLog([[loader loadHistory] description]);
    }
    else {
        // what if not...? how to abort?
    }
    
    // copy our own dictionary
    config=[[NSMutableDictionary dictionaryWithDictionary:[[loader config] dictionary]] retain];
    
    // get different nodes
    loadercfg=[config valueForKey:@"OVLoader" default:[[NSMutableDictionary new] autorelease]];
    dsrvrcfg=[config valueForKey:@"OVDisplayServer" default:[[NSMutableDictionary new] autorelease]];
    menucfg=[config valueForKey:@"OVMenuManager" default:[[NSMutableDictionary new] autorelease]];

    
    // create the lists...
    modlist=[[CVPrefArray alloc] initWithDragDropSetting:FALSE];
    oflist=[[CVPrefArray alloc] initWithDragDropSetting:TRUE];

    // we retain an output filter list
    outputfilters=[NSMutableArray new];

    NSEnumerator *e;
    NSArray *ldmodlist=[loader moduleList];
    NSArray *libexclude=[loadercfg valueForKey:@"excludeLibraryList" default:[[NSArray new] autorelease]];
    NSArray *modexclude=[loadercfg valueForKey:@"excludeModuleList" default:[[NSArray new] autorelease]];
    
    // shared tab settings
    NSString *shortcut=[menucfg valueForKey:@"fastIMSwitch" default:@""];
    CVKeyCode cvkc(shortcut);
    if (cvkc.isCommand()) [sharetab_cmd setIntValue:1];
    if (cvkc.isOpt()) [sharetab_option setIntValue:1];
    if (cvkc.isCtrl()) [sharetab_control setIntValue:1];
    if (cvkc.isShift()) [sharetab_shift setIntValue:1];
    [sharetab_keylist removeAllItems];
    [sharetab_keylist addItemsWithTitles:cvkc.getKeyList()];
    [sharetab_keylist selectItemWithTitle:cvkc.getKeyCodeString()];
    [self sharetab_shortcutKeyChange:self];

    // set user interface defaults
    [modtab_box_keylist removeAllItems];
    [modtab_box_keylist addItemsWithTitles:cvkc.getKeyList()];
    [modtab_box_keylist selectItemWithTitle:@""];
    
    
    // shared tab
    NSString *notifystyle=[dsrvrcfg valueForKey:@"notificationStyle" default:@"default"];
    if ([notifystyle isEqualToString:@"default"])
        [sharetab_shownotify setIntValue:1];
    else
        [sharetab_shownotify setIntValue:0];
    
	float opacity=[[dsrvrcfg valueForKey:@"opacity" default:@"1.0"] floatValue];
	NSColor *fc=[[dsrvrcfg valueForKey:@"foreground" default:@"1.0 1.0 1.0"] colorByString];
	NSString *img=[dsrvrcfg valueForKey:@"backgroundImage" default:@""];
    NSString *bcstr=[dsrvrcfg valueForKey:@"background" default:@"1.0 1.0 1.0"];
	NSString *fontname=[dsrvrcfg valueForKey:@"font" default:@"Lucida Grande"];
	float fontsize=[[dsrvrcfg valueForKey:@"size" default:@"18"] floatValue];
    
    NSFont *font=[NSFont fontWithName:fontname size:fontsize];
    [sharetab_forecolor setColor:fc];
    [sharetab_transparencyslider setFloatValue:opacity*100];
    
    if ([img length]) {
        // NEED TO USE SHORTENED FORM, OTHERWISE IT'S TOO LONG
        [sharetab_backimage setStringValue:[self shortenedFilename:img maxLength:26]];
    }
    else {
        [sharetab_backimage setStringValue:MSG(@"(none)")];
        [sharetab_backcolor setColor:[NSColor whiteColor]];
        if ([bcstr isEqualToString:@"none"]) [sharetab_backimage setStringValue:MSG(@"(aqua)")];
        else if ([bcstr isEqualToString:@"transparent"]) [sharetab_backimage setStringValue:MSG(@"(transparent)")];
        else [sharetab_backcolor setColor:[bcstr colorByString]];
    }
    
    // set font manager too
    [fontmanager setDelegate:self];
    [fontmanager setSelectedFont:[fontmanager convertFont:font] isMultiple:NO];
    [self changeFont:fontmanager];
    [self sharetab_changeTransparency:sharetab_transparencyslider];
    [sharetab_previewview changeConfig:dsrvrcfg];
    
    
    // setup sound
    [self setupSound];
    
    // get the comprehensive exclude list, first of all we
    // rebuild the tree of load history
    loadhistory=[NSMutableDictionary new];
    NSDictionary *history=[loader loadHistory];
    NSArray *lhiskeys=[history allKeys];
    e=[lhiskeys objectEnumerator];
    NSString *lk;
    while (lk=[e nextObject]) {
        NSArray *lhnode=[history valueForKey:lk];
        NSMutableDictionary *newnode=[[NSMutableDictionary new] autorelease];
        NSEnumerator *lhe=[lhnode objectEnumerator];
        NSString *s;
        while (s=[lhe nextObject]) [newnode setValue:[NSNumber numberWithBool:TRUE] forKey:s];
        [loadhistory setValue:newnode forKey:[lk lastPathComponent]];        
    }
//    NSLog(@"load history=%@", [loadhistory description]);
    
    NSMutableArray *excludelist=[NSMutableArray arrayWithArray:modexclude];
    e=[libexclude objectEnumerator];
    NSString *s;
    while (s=[e nextObject]) {
        NSDictionary *libmods=[loadhistory valueForKey:s];
        if (libmods) [excludelist addObjectsFromArray:[libmods allKeys]];
    }
//    NSLog(@"exclude list=%@", [excludelist description]);

    NSMutableArray *oforderlist=[[NSMutableArray new] autorelease];

    // we have everything now, let's do the business
    e=[ldmodlist objectEnumerator];
    CVModuleWrapper *w;
    const char *lc=[loader service]->locale();
    while (w=[e nextObject]) {
        NSString *mid=[w identifier];
        BOOL loaded=YES;
        if (CVStringIsInArray(mid, excludelist)) loaded=NO;

        // if it begins with OVIMGeneric...
        if ([[w identifier] hasPrefix:@"OVIMGeneric-"] && [[w moduleType] isEqualToString:@"OVInputMethod"]) {
            [gimmodlist addObject:mid];
        }
        else {      // otherwise put in our propedit's modlist
            [propeditmodlist addObject:mid];
        }
        
        // if it's OF, add to our "trial for fun" list
        if ([[w moduleType] isEqualToString:@"OVOutputFilter"]) [outputfilters addObject: w];
        
        OVModule *ovm=[w module];
        NSString *name=[NSString stringWithUTF8String:ovm->localizedName(lc)];        
        NSString *shortcut=[menucfg valueForKey:mid default:@""];
        
        CVModuleItem *i=[[CVModuleItem alloc] initWithModuleID:mid name:name shortcut:shortcut loaded:loaded];
        [[modlist array] addObject:i];
        if ([[w moduleType] isEqualToString:@"OVOutputFilter"]) [oforderlist addObject:i];
    }

    [oftab_convertfilter removeAllItems];
    int i, c=[outputfilters count];
    for (i=0; i<c; i++) {
        OVModule *ovm=[[outputfilters objectAtIndex:i] module];
        [oftab_convertfilter addItemWithTitle:[NSString stringWithUTF8String:ovm->localizedName(lc)]];
    }

    NSArray *cfgoforder=[menucfg valueForKey:@"outputFilterOrder" default:[[NSArray new] autorelease]];
    e=[cfgoforder objectEnumerator];
    NSString *cs;
    while (cs=[e nextObject]) {
        int c=[oforderlist count];
        for (int i=0; i<c; i++) {
            CVModuleItem *cvmi=[oforderlist objectAtIndex:i];
            if ([[cvmi identifier] isEqualToString:cs]) {
                [[oflist array] addObject:cvmi];
                [oforderlist removeObjectAtIndex:i];
                break;
            }
        }
    }
    [[oflist array] addObjectsFromArray:oforderlist];

    // setup IM settings (OVIMPhonetics, OVIMChewing, OVIMPOJ-Holo, OVIMTibetan)
    [self setupIMSettings];
    [self setupGenericIMSettings];

    // we can finally set up propedit!
    


    // register pasteboard for drag-and-drop functionality
    [oftab_oforderlist registerForDraggedTypes:[NSArray arrayWithObject:NSStringPboardType]];
    [oftab_oforderlist setDataSource:oflist];
    [modtab_modlist setDataSource:modlist];
    [modtab_modlist setDelegate:self];
    
    [[NSApplication sharedApplication] setDelegate:self];
}
 
- (void)dealloc {
    if (sound) [sound release];
    [gimmodlist release];
    [propeditmodlist release];
    [outputfilters release];
    [modlist release];
    [oflist release];
    [loader release];
    [config release];
    [loadhistory release];
    [fastimswitchkey release];
    [super dealloc];
}
- (void)windowWillClose:(NSNotification *)aNotification {
    [[NSApplication sharedApplication] terminate:self];
}
- (void)applicationWillTerminate:(NSNotification *)aNotification {
    [self pref_writeConfig:self];
}
- (BOOL)tableView:(NSTableView *)t shouldSelectRow:(int)r {
    if (t==modtab_modlist) {
        modtab_modlist_currentrow=r;
        
        NSString *mid=[[[modlist array] objectAtIndex:r] identifier];
        CVModuleWrapper *w=CVFindModule([loader moduleList], mid);
        if (w) {
            [modtab_loadedfrom setStringValue:[w fromLibrary]];
        }
        
        NSString *shortcut=[[[modlist array] objectAtIndex:r] shortcut];
        CVKeyCode cvkc(shortcut);
        [modtab_box_cmd setIntValue:0];
        [modtab_box_option setIntValue:0];
        [modtab_box_control setIntValue:0];
        [modtab_box_shift setIntValue:0];
        if (cvkc.isCommand()) [modtab_box_cmd setIntValue:1];
        if (cvkc.isOpt()) [modtab_box_option setIntValue:1];
        if (cvkc.isCtrl()) [modtab_box_control setIntValue:1];
        if (cvkc.isShift()) [modtab_box_shift setIntValue:1];
        [modtab_box_keylist selectItemWithTitle:cvkc.getKeyCodeString()];
    }
    return TRUE;
}
- (IBAction)sharetab_shortcutKeyChange:(id)sender {
    char buf[32];
    bzero(buf, 32);
    if ([sharetab_cmd intValue]) strcat(buf, "m");
    if ([sharetab_option intValue]) strcat(buf, "o");
    if ([sharetab_control intValue]) strcat(buf, "c");
    if ([sharetab_shift intValue]) strcat(buf, "s");
    NSString *key=[sharetab_keylist titleOfSelectedItem];
    
    [fastimswitchkey release];
    if ([key length] && strlen(buf)) {
        fastimswitchkey=[[NSString stringWithFormat:@"%@ %s", key, buf] retain];
    }
    else {
        fastimswitchkey=[[NSString stringWithString:@""] retain];
    }
}
- (IBAction)modtab_shortcutKeyChange:(id)sender {
    if (modtab_modlist_currentrow==-1) return;
    char buf[32];
    bzero(buf, 32);
    if ([modtab_box_cmd intValue]) strcat(buf, "m");
    if ([modtab_box_option intValue]) strcat(buf, "o");
    if ([modtab_box_control intValue]) strcat(buf, "c");
    if ([modtab_box_shift intValue]) strcat(buf, "s");
    NSString *key=[modtab_box_keylist titleOfSelectedItem];
    if ([key length] && strlen(buf)) {
        [[[modlist array] objectAtIndex:modtab_modlist_currentrow]
            setShortcut:[NSString stringWithFormat:@"%@ %s", key, buf]];
        [modtab_modlist reloadData];
    }
}
- (IBAction)oftab_convert:(id)sender {
    CVModuleWrapper *w=[outputfilters objectAtIndex:[oftab_convertfilter indexOfSelectedItem]];
    NSLog(@"using %@", [w identifier]);
    
    // initialize it first
    CVDictionary mcd([self getConfigNode:[w identifier]]);
    [w initializeWithConfig:&mcd service:[loader service]];
    OVOutputFilter *of=(OVOutputFilter*)[w module];
    NSString *output=[NSString stringWithUTF8String:of->process(
        [[oftab_inputtext string] UTF8String],
        [loader service])];
        
    [oftab_outputtext setString:output];
    [oftab_notifymessage setString:[loader notifyMessage]];
    [loader clearNotification];
}
- (IBAction)sharetab_changeColor:(id)sender {
    if (sender==sharetab_forecolor) {
        [dsrvrcfg setValue:[NSString stringByColor:[sharetab_forecolor color]] forKey:@"foreground"];
    }
    else {
        NSString *img=[sharetab_backimage stringValue];
        if ([img isEqualToString:MSG(@"(none)")]) {
            [dsrvrcfg setValue:[NSString stringByColor:[sharetab_backcolor color]] forKey:@"background"];
        }
    }

    [sharetab_previewview changeConfig:dsrvrcfg];
}
- (IBAction)sharetab_changeImage:(id)sender {
    // we use this trick to tell which button is which
    NSString *button=[sender alternateTitle];
    
    if ([button isEqualToString:@"set"]) {
        NSOpenPanel *op=[NSOpenPanel openPanel];
        [op setAllowsMultipleSelection:FALSE];
        if ([op runModalForDirectory:nil file:nil]==NSFileHandlingPanelOKButton) {
            NSString *f=[[op filenames] objectAtIndex:0];
            [sharetab_backimage setStringValue:[self shortenedFilename:f maxLength:26]];
            [dsrvrcfg setValue:f forKey:@"backgroundImage"];
            [dsrvrcfg setValue:[NSString stringByColor:[sharetab_backcolor color]] forKey:@"background"];
        }
    }
    else if ([button isEqualToString:@"none"]) {
        [dsrvrcfg setValue:@"" forKey:@"backgroundImage"];
        [dsrvrcfg setValue:[NSString stringByColor:[sharetab_backcolor color]] forKey:@"background"];
        [sharetab_backimage setStringValue:MSG(@"(none)")];
    }
    else if ([button isEqualToString:@"transparent"]) {
        [dsrvrcfg setValue:@"" forKey:@"backgroundImage"];
        [dsrvrcfg setValue:@"transparent" forKey:@"background"];
        [sharetab_backimage setStringValue:MSG(@"(transparent)")];
    }
    
    [sharetab_previewview changeConfig:dsrvrcfg];
}
- (IBAction)sharetab_changeTransparency:(id)sender {
    NSLog(@"%f", [sender intValue]/100.0);
    [sharetab_transparencytag setStringValue:[NSString stringWithFormat:@"%d%%", [sender intValue]]];
    [dsrvrcfg setValue:[NSNumber numberWithFloat:[sender intValue]/100.0] forKey:@"opacity"];
    [sharetab_previewview changeConfig:dsrvrcfg];
}
- (IBAction)sharetab_setFont:(id)sender {
    NSFontPanel *fp=[fontmanager fontPanel:YES];
    [fp orderFront:self];    
}
- (void)changeFont:(id)sender {
    NSFont *newfont=[fontmanager convertFont:[fontmanager selectedFont]];
    [sharetab_fonttag setStringValue:[NSString stringWithFormat:@"%@, %d pt", [newfont fontName], (int)[newfont pointSize]]];

    [dsrvrcfg setValue:[newfont fontName] forKey:@"font"];
    [dsrvrcfg setValue:[NSNumber numberWithFloat:[newfont pointSize]] forKey:@"size"];
    [sharetab_previewview changeConfig:dsrvrcfg];
}
- (IBAction)sharetab_changeSound:(id)sender {
    if (sound) {
        if ([sound isPlaying]) [sound stop];
    }

    [loadercfg setValue:@"" forKey:@"beepSound"];
    [sharetab_soundfile setStringValue:@""];
    
    NSLog(@"change sound!");
    NSString *t=[sender titleOfSelectedItem];
    if ([t isEqualToString:MSG(@"(none)")]) {
        [loadercfg setValue:@"0" forKey:@"shouldBeep"];
        return;
    }

    [loadercfg setValue:@"1" forKey:@"shouldBeep"];

    if ([t isEqualToString:MSG(@"(default)")]) {
        [loadercfg setValue:@"" forKey:@"beepSound"];
        return;
    }

    if ([t isEqualToString:MSG(@"customized...")]) {
        NSOpenPanel *op=[NSOpenPanel openPanel];
        [op setAllowsMultipleSelection:FALSE];
        if ([op runModalForDirectory:nil file:nil]==NSFileHandlingPanelOKButton) {
            NSString *f=[[op filenames] objectAtIndex:0];
            [sharetab_soundfile setStringValue:[self shortenedFilename:f maxLength:30]];
            [loadercfg setValue:f forKey:@"beepSound"];
        }
        else {
            [sender selectItemWithTitle:MSG(@"(default)")];
        }
    }
    else {
        [loadercfg setValue:t forKey:@"beepSound"];
    }
}
- (IBAction)sharetab_changeNotify:(id)sender {
    if ([sender intValue])
        [dsrvrcfg setValue:@"default" forKey:@"notificationStyle"];
    else
        [dsrvrcfg setValue:@"silent" forKey:@"notificationStyle"];
}
- (IBAction)sharetab_testSound:(id)sender {
    if (sound) {
        if ([sound isPlaying]) [sound stop];
    }

    NSString *t=[sharetab_soundlist titleOfSelectedItem];
    if ([t isEqualToString:MSG(@"(none)")]) return;
    if ([t isEqualToString:MSG(@"(default)")]) {
        SysBeep(30);
        return;
    }

    if (sound) {
        [sound release];
        sound=nil;
    }
    if ([t isEqualToString:MSG(@"customized...")]) {
        NSString *f=[sharetab_soundfile stringValue];
        if ([f length]) sound=[[NSSound alloc] initWithContentsOfFile:f byReference:YES];
    }
    else {
        sound=[NSSound soundNamed:t];
        [sound retain];
    }
    
    if (sound) [sound play];    
}
- (IBAction)pref_dumpConfigToConsole:(id)sender {
    NSLog(@"dumping output filter order");
    NSLog([[oflist array] description]);
    
    NSLog(@"dumping module lists and enable/disable settings");
    NSLog([[modlist array] description]);
}
- (IBAction)pref_writeConfig:(id)sender {
    NSLog(@"gathering and writing config");

    // gathr IM configs
    [self gatherIMSettings];

    // write shortcut menu settings (have to erase the entire node first)
    NSMutableArray *ma=[modlist array];
    NSMutableDictionary *md=[[NSMutableDictionary new] autorelease];
    NSEnumerator *e=[ma objectEnumerator];
    CVModuleItem *cvmi;
    while (cvmi=[e nextObject]) {
        NSString *shortcut=[cvmi shortcut];
        if ([shortcut length]) [md setValue:shortcut forKey:[cvmi identifier]];
    }
    
    // ... with fastIMSwitch settings
    if ([fastimswitchkey length]) [md setValue:fastimswitchkey forKey:@"fastIMSwitch"];
    
    // ... and outputFilterOrder, too
    NSMutableArray *ofo=[[NSMutableArray new] autorelease];
    e=[[oflist array] objectEnumerator];
    while (cvmi=[e nextObject]) [ofo addObject:[cvmi identifier]];
    [md setValue:ofo forKey:@"outputFilterOrder"];
    
    // write OVMenuManager node
    [config setValue:md forKey:@"OVMenuManager"];

    // gather the exclude list
    e=[[modlist array] objectEnumerator];
    while (cvmi=[e nextObject]) {
        CVModuleWrapper *w=CVFindModule([loader moduleList], [cvmi identifier]);
        if (!w) continue;
        NSString *fromlib=[[w fromLibrary] lastPathComponent];
        NSDictionary *node=[loadhistory valueForKey:fromlib];
        if (!node) continue;
        [node setValue:[NSNumber numberWithBool:[cvmi loaded]] forKey:[cvmi identifier]];
    }
    
    // now we can write the exclude list
    NSMutableArray *newlibexclude=[[NSMutableArray new] autorelease];
    NSMutableArray *newmodexclude=[[NSMutableArray new] autorelease];
    e=[[loadhistory allKeys] objectEnumerator];
    NSString *libname;
    while (libname=[e nextObject]) {
        NSDictionary *node=[loadhistory valueForKey:libname];
        NSArray *nkeys=[node allKeys];
        NSArray *disabledkeys=[node allKeysForObject:[NSNumber numberWithBool:FALSE]];
        if ([nkeys count]==[disabledkeys count])
            [newlibexclude addObject:libname];
        else {
            if ([disabledkeys count]) [newmodexclude addObjectsFromArray:disabledkeys];
        }
    }
    [loadercfg setValue:newlibexclude forKey:@"excludeLibraryList"];
    [loadercfg setValue:newmodexclude forKey:@"excludeModuleList"];

    NSLog(@"new dictionary=%@", [config description]);

    // we sync loader config first, then overwrite with ours
    [[loader config] sync];
    [[[loader config] dictionary] removeAllObjects];
    [[[loader config] dictionary] addEntriesFromDictionary:config];
    [[loader config] sync];
}
- (NSString*)shortenedFilename:(NSString*)f maxLength:(int)m {
    NSString *display=f;
    if ([f length] > (size_t)m) {
        NSArray *pc=[f pathComponents];
        if ([pc count] > 2) {
            display=[NSString stringWithFormat:@"%@%@/.../%@",
                [pc objectAtIndex:0],
                [pc objectAtIndex:1],
                [pc objectAtIndex:[pc count]-1]];
        }
    }
    return display;
}
- (void)setupSound {
    NSArray *aiff=CVEnumeratePath(@"/System/Library/Sounds", @".aiff");
    
    [sharetab_soundlist removeAllItems];
    [sharetab_soundlist addItemWithTitle:MSG(@"(default)")];
    [sharetab_soundlist addItemWithTitle:MSG(@"(none)")];
    int c=[aiff count];
    for (int i=0; i<c; i++) {
        [sharetab_soundlist addItemWithTitle:[[[[aiff objectAtIndex: i] lastPathComponent] componentsSeparatedByString:@"."] objectAtIndex:0]];
    }
    [sharetab_soundlist addItemWithTitle:MSG(@"customized...")];
    
    NSString *soundfile=[loadercfg valueForKey:@"beepSound" default:@""];
    if ([soundfile length]) {
        [sharetab_soundlist selectItemWithTitle:soundfile];
        [sharetab_soundfile setStringValue:@""];
        if (![sharetab_soundlist selectedItem]) {
            [sharetab_soundlist selectItemWithTitle:MSG(@"customized...")];
            [sharetab_soundfile setStringValue:[self shortenedFilename:soundfile maxLength:30]];
        }
    }
    else {
        [sharetab_soundfile setStringValue:@""];
    }
}
- (void)setupIMSettings {
    NSDictionary *d;
    if ([self identifierExists:@"OVIMPhonetic"]) {
        d=[self getConfigNode:@"OVIMPhonetic"];
        [settab_phoneticslayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout" default:@"0"] intValue]];
        [settab_phoneticslayout setEnabled:YES];
        CVRemoveStringFromArray(@"OVIMPhonetic", propeditmodlist);
    }
    if ([self identifierExists:@"OVIMChewing"]) {
        d=[self getConfigNode:@"OVIMChewing"];
        [settab_chewinglayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout" default:@"0"] intValue]];
        [settab_chewinglayout setEnabled:YES];
        CVRemoveStringFromArray(@"OVIMChewing", propeditmodlist);
    }

    if ([self identifierExists:@"OVIMTibetan"]) {
        d=[self getConfigNode:@"OVIMTibetan"];
        [settab_tibetanlayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout" default:@"0"] intValue]];
        [settab_tibetanlayout setEnabled:YES];
        CVRemoveStringFromArray(@"OVIMTibetan", propeditmodlist);
    }

    if ([self identifierExists:@"OVIMPOJ-Holo"]) {
        d=[self getConfigNode:@"OVIMPOJ-Holo"];
        [settab_pojlayout selectItemAtIndex:[[d valueForKey:@"keyboardLayout" default:@"0"] intValue]];
        [settab_pojlayout setEnabled:YES];

        [settab_pojascii setIntValue:[[d valueForKey:@"ASCIIOutput" default:@"0"] intValue]];
        [settab_pojascii setEnabled:YES];

        [settab_pojpojonly setIntValue:[[d valueForKey:@"fullPOJOutput" default:@"0"] intValue]];
        [settab_pojpojonly setEnabled:YES];
        CVRemoveStringFromArray(@"OVIMPOJ-Holo", propeditmodlist);
    }
}
- (void)setupGenericIMSettings {
    [gim_imlist removeAllItems];
    if (![gimmodlist count]) return;
    
    int c=[gimmodlist count];
    const char *lc=[loader service]->locale();
    for (int i=0; i<c; i++) {
        CVModuleWrapper *w=CVFindModule([loader moduleList], [gimmodlist objectAtIndex:i]);
        OVModule *ovm=[w module];
        [gim_imlist addItemWithTitle:[NSString stringWithUTF8String:ovm->localizedName(lc)]];
    }
    [gim_imlist setEnabled:YES];
    [gim_imlist selectItemAtIndex:0];
    [gim_autocompose setEnabled:YES];
    [gim_errorbeep setEnabled:YES];
    [gim_hitmaxcompose setEnabled:YES];
    [gim_maxkeylen setEnabled:YES];
    [gim_shiftselection setEnabled:YES];
    [self gim_changeIM:gim_imlist];
}
- (IBAction)gim_changeIM:(id)sender {
    int i=[sender indexOfSelectedItem];
    if (i < 0) return;
    
    NSDictionary *d=[self getConfigNode:[gimmodlist objectAtIndex:i]];
    #define NUM(x,y)  [[d valueForKey:x default:y] intValue]
    [gim_autocompose setIntValue:NUM(@"autoCompose", @"0")];
    [gim_hitmaxcompose setIntValue:NUM(@"hitMaxAndCompose", @"0")];
    [gim_shiftselection setIntValue:NUM(@"shiftSelectionKey", @"0")];
    [gim_errorbeep setIntValue:NUM(@"warningBeep", @"1")];
    #undef NUM
    
    NSString *maxkeylen=[d valueForKey:@"maxKeySequenceLength" default:@"5"];
    [gim_maxkeylen selectItemWithTitle:maxkeylen];
    if ([gim_maxkeylen indexOfSelectedItem]==-1) [gim_maxkeylen selectItemWithTitle:@"5"];
}
- (IBAction)gim_settingsTrigger:(id)sender {
    int i=[gim_imlist indexOfSelectedItem];
    NSDictionary *d=[self getConfigNode:[gimmodlist objectAtIndex:i]];    
    #define SET(x,y) [d setValue:[NSString stringWithFormat:@"%d", [y intValue]] forKey:x];
    SET(@"autoCompose", gim_autocompose);
    SET(@"hitMaxAndCompose", gim_hitmaxcompose);
    SET(@"shiftSelectionKey", gim_shiftselection);
    SET(@"warningBeep", gim_errorbeep);
    [d setValue:[gim_maxkeylen titleOfSelectedItem] forKey:@"maxKeySequenceLength"];
    #undef SET
}
- (void)gatherIMSettings {
    #define NUM(x)  [NSString stringWithFormat:@"%d", x] 
    NSDictionary *d;
    if ([self identifierExists:@"OVIMPhonetic"]) {
        d=[self getConfigNode:@"OVIMPhonetic"];
        [d setValue:NUM([settab_phoneticslayout indexOfSelectedItem]) forKey:@"keyboardLayout"];
    }
    if ([self identifierExists:@"OVIMChewing"]) {
        d=[self getConfigNode:@"OVIMChewing"];
        [d setValue:NUM([settab_chewinglayout indexOfSelectedItem]) forKey:@"keyboardLayout"];
    }

    if ([self identifierExists:@"OVIMTibetan"]) {
        d=[self getConfigNode:@"OVIMTibetan"];
        [d setValue:NUM([settab_tibetanlayout indexOfSelectedItem]) forKey:@"keyboardLayout"];
    }

    if ([self identifierExists:@"OVIMPOJ-Holo"]) {
        d=[self getConfigNode:@"OVIMPOJ-Holo"];
        [d setValue:NUM([settab_pojlayout indexOfSelectedItem]) forKey:@"keyboardLayout"];
        [d setValue:NUM([settab_pojascii intValue]) forKey:@"ASCIIOutput"];
        [d setValue:NUM([settab_pojpojonly intValue]) forKey:@"fullPOJOutput"];
        [d setValue:@"1" forKey:@"warningBeep"];
    }
    #undef NUM
}
- (BOOL)identifierExists:(NSString*)mid {
    if (CVFindModule([loader moduleList], mid)) return YES;
    return NO;
}
- (NSDictionary*)getConfigNode:(NSString*)mid {
    return [config valueForKey:mid default:[[NSMutableDictionary new] autorelease]];
}
@end

@implementation CVPrefArray 
- (id)initWithDragDropSetting:(BOOL)d {
    if (self=[super init]) {
        draggable=d;
        array=[NSMutableArray new];
    }
    return self;
}
- (void)dealloc {
    [array release];
    [super dealloc];
}
- (NSMutableArray*)array {
    return array;
}
- (id) tableView:(NSTableView*)t objectValueForTableColumn:(NSTableColumn*)c row:(int)r {
    if ([[c identifier] isEqualToString:@"loaded"])
        return [NSNumber numberWithBool:[[[self array] objectAtIndex:r] loaded]];

    if ([[c identifier] isEqualToString:@"shortcut"]) {
        NSString *sc=[[[self array] objectAtIndex:r] shortcut];
        CVKeyCode kc(sc);
        NSString *ics=kc.getModifierIconString();
        NSString *ks=[NSString stringWithFormat:@"%@ %@", ics, kc.getKeyCodeString()];
        return ks;
    }

    // the other column identifier will always be "modulename"
    return [[[self array] objectAtIndex:r] name];
}
- (void)tableView:(NSTableView *)t setObjectValue:(id)v forTableColumn:(NSTableColumn *)c row:(int)r {
    if ([[c identifier] isEqualToString:@"loaded"])
        [[[self array] objectAtIndex:r] setLoaded:[v boolValue]];
}

- (int)numberOfRowsInTableView:(NSTableView*)t {
	return [[self array] count];  
}
- (BOOL)tableView:(NSTableView *)t writeRows:(NSArray*)r toPasteboard:(NSPasteboard*)pb {
    if (!draggable) return FALSE;
    [pb declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:t];

    NSNumber *n=[r objectAtIndex:0];    // only use the first row number
    [pb setString:[n stringValue] forType:NSStringPboardType];
    return YES;
}
- (NSDragOperation)tableView:(NSTableView*)t validateDrop:(id <NSDraggingInfo>)info proposedRow:(int)r proposedDropOperation:(NSTableViewDropOperation)op
{
    if (!draggable) return NSDragOperationNone;
    return NSDragOperationEvery;
}
- (BOOL)tableView:(NSTableView *)t acceptDrop:(id <NSDraggingInfo>)info row:(int)r dropOperation:(NSTableViewDropOperation)op
{
    id pb=[info draggingPasteboard];
    
    int srcrow=[[pb stringForType:NSStringPboardType] intValue];
    NSLog(@"moving item from row %d to %d", srcrow, r);
    
    id obj=[[self array] objectAtIndex:srcrow];
    [[self array] insertObject:obj atIndex:r];
    
    if (srcrow < r) {
        [[self array] removeObjectAtIndex:srcrow];
    }
    else {
        [[self array] removeObjectAtIndex:srcrow+1];
    }
    [t reloadData];
    
    return YES;
}
@end

@implementation CVModuleItem
- (id)initWithModuleID:(NSString*)i name:(NSString*)n shortcut:(NSString*)s loaded:(BOOL)e {
    if (self=[super init]) {
        modid=[[NSString alloc] initWithString:i];
        modname=[[NSString alloc] initWithString:n];
        shortcut=[[NSString alloc] initWithString:(s ? s : @"")];
        loaded=e;
    }
    return self;
}
- (void)dealloc {
    [modid release];
    [modname release];
    [shortcut release];
    [super dealloc];
}
- (NSString*)description {
    return [NSString stringWithFormat:@"%@ (%@) [%@] %@", modid, modname, shortcut, loaded ? @"ENABLED" : @"DISABLED"];
}
- (NSString*)name {
    return modname;
}
- (NSString*)identifier {
    return modid;
}
- (NSString*)shortcut {
    return shortcut;
}
- (BOOL)loaded {
    return loaded;
}
- (void)setLoaded:(BOOL)e {
    loaded=e;
}
- (void)setShortcut:(NSString*)s {
    [shortcut release];
    shortcut=[[NSString alloc] initWithString:s];
}
@end
