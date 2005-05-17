#import "OVPrefDelegate.h"
#import "CVWrappers.h"
#import "CVLoaderUtility.h"
#import "CVDictionary.h"
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
@end

@implementation OVPrefDelegate
- (void)awakeFromNib {
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
    return TRUE;
}
- (IBAction)modtab_shortcutKeyChange:(id)sender {
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

    if ([[c identifier] isEqualToString:@"shortcut"])
        return [[[self array] objectAtIndex:r] shortcut];

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
@end
