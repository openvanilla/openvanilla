#import "OVPrefDelegate.h"
#import "CVWrappers.h"
#import "CVLoaderUtility.h"
#import "CVDictionary.h"
#import "CVKeyCode.h"
#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"

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

    loader=[[CVEmbeddedLoader alloc] init];
    if (loader) {
        NSLog(@"embedded loader inited");
//        NSLog([[loader config] description]);
//        NSLog([[loader moduleList] description]);
//        NSLog([[loader loadHistory] description]);
    }
    else {
        // what if not...? how to abort?
    }
    
    // copy our own dictionary
    config=[[NSMutableDictionary dictionaryWithDictionary:[[loader config] dictionary]] retain];
    
    // create the lists...
    modlist=[[CVPrefArray alloc] initWithDragDropSetting:FALSE];
    oflist=[[CVPrefArray alloc] initWithDragDropSetting:TRUE];

    // we retain an output filter list
    outputfilters=[NSMutableArray new];

    NSArray *ldmodlist=[loader moduleList];
    NSDictionary *loaderdict=[config valueForKey:@"OVLoader" default:[[NSMutableDictionary new] autorelease]];
    NSDictionary *menudict=[config valueForKey:@"OVMenuManager" default:[[NSMutableDictionary new] autorelease]];
    NSArray *libexclude=[loaderdict valueForKey:@"excludeLibraryList" default:[[NSArray new] autorelease]];
    NSArray *modexclude=[loaderdict valueForKey:@"excludeModuleList" default:[[NSArray new] autorelease]];
    NSDictionary *history=[loader loadHistory];
    
    // shared tab settings
    NSString *shortcut=[menudict valueForKey:@"fastIMSwitch" default:@""];
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
    
    
    // get the comprehensive exclude list
    NSMutableArray *excludelist=[NSMutableArray arrayWithArray:modexclude];
    NSEnumerator *e=[libexclude objectEnumerator];
    NSString *s;
    while (s=[e nextObject]) {
        NSArray *libmods=[history valueForKey:s];
        if (libmods) [excludelist addObjectsFromArray:libmods];
    }
    NSLog(@"exclude list=%@", [excludelist description]);

    NSMutableArray *oforderlist=[[NSMutableArray new] autorelease];

    // we have everything now, let's do the business
    e=[ldmodlist objectEnumerator];
    CVModuleWrapper *w;
    const char *lc=[loader service]->locale();
    while (w=[e nextObject]) {
        NSString *mid=[w identifier];
        BOOL loaded=YES;
        if (CVStringIsInArray(mid, excludelist)) loaded=NO;
        
        // if it's an OF, init it
        if ([[w moduleType] isEqualToString:@"OVOutputFilter"]) {
            NSLog(@"init OF %@", mid);
            NSMutableDictionary *modcfgdict=[config valueForKey:mid default:[[NSMutableDictionary new] autorelease]];
            CVDictionary mcd(modcfgdict);
            [w initializeWithConfig:&mcd service:[loader service]];
            [outputfilters addObject: w];
        }
        
        OVModule *ovm=[w module];
        NSString *name=[NSString stringWithUTF8String:ovm->localizedName(lc)];        
        NSString *shortcut=[menudict valueForKey:mid default:@""];
        
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

    NSArray *cfgoforder=[menudict valueForKey:@"outputFilterOrder" default:[[NSArray new] autorelease]];
    NSLog(@"CFG OF ORDER=%@", [cfgoforder description]);
    e=[cfgoforder objectEnumerator];
    NSString *cs;
    while (cs=[e nextObject]) {
        int c=[oforderlist count];
        NSLog(@"cs=%@, oforderlist count=%d", cs, c);
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

    // register pasteboard for drag-and-drop functionality
    [oftab_oforderlist registerForDraggedTypes:[NSArray arrayWithObject:NSStringPboardType]];
    [oftab_oforderlist setDataSource:oflist];
    [modtab_modlist setDataSource:modlist];
    [modtab_modlist setDelegate:self];
}
- (void)dealloc {
    [outputfilters release];
    [modlist release];
    [oflist release];
    [loader release];
    [config release];
    [super dealloc];
}
- (void)windowWillClose:(NSNotification *)aNotification {
    [self pref_writeConfig:self];
}
- (BOOL)tableView:(NSTableView *)t shouldSelectRow:(int)r {
    if (t==modtab_modlist) {
        NSLog(@"modlist row %d selected, shortcut=%@", r, [[[modlist array] objectAtIndex:r] shortcut]);
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
    if ([key length] && strlen(buf)) {
        [fastimswitchkey release];
        fastimswitchkey=[[NSString stringWithFormat:@"%@ %s", key, buf] retain];
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
    
    OVOutputFilter *of=(OVOutputFilter*)[w module];
    
    NSString *output=[NSString stringWithUTF8String:of->process(
        [[oftab_inputtext string] UTF8String],
        [loader service])];
        
    [oftab_outputtext setString:output];
    [oftab_notifymessage setString:[loader notifyMessage]];
}
- (IBAction)pref_dumpConfigToConsole:(id)sender {
    NSLog(@"dumping output filter order");
    NSLog([[oflist array] description]);
    
    NSLog(@"dumping module lists and enable/disable settings");
    NSLog([[modlist array] description]);
}
- (IBAction)pref_writeConfig:(id)sender {
    NSLog(@"gathering and writing config");

    // write shortcut menu settings
    NSMutableArray *ma=[modlist array];
    NSMutableDictionary *md=[[NSMutableDictionary new] autorelease];
    NSEnumerator *e=[ma objectEnumerator];
    CVModuleItem *cvmi;
    while (cvmi=[e nextObject]) {
        NSString *shortcut=[cvmi shortcut];
        if ([shortcut length]) [md setValue:shortcut forKey:[cvmi identifier]];
    }
    
    // ... with fastIMSwitch settings
    [md setValue:fastimswitchkey forKey:@"fastIMSwitch"];
    
    // ... and outputFilterOrder, too
    NSMutableArray *ofo=[[NSMutableArray new] autorelease];
    e=[[oflist array] objectEnumerator];
    while (cvmi=[e nextObject]) [ofo addObject:[cvmi identifier]];
    [md setValue:ofo forKey:@"outputFilterOrder"];
    
    [config setValue:md forKey:@"OVMenuManager"];

    // we sync loader config first, then overwrite with ours
    [[loader config] sync];
    [[[loader config] dictionary] removeAllObjects];
    [[[loader config] dictionary] addEntriesFromDictionary:config];
    [[loader config] sync];

    [[NSApplication sharedApplication] terminate:self];
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
