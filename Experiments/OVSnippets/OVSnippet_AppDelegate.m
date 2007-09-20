//
//  OVSnippet_AppDelegate.m
//  OVSnippet
//
//  Created by zonble on 2007/9/20.
//  Copyright __MyCompanyName__ 2007 . All rights reserved.
//

#import "OVSnippet_AppDelegate.h"
#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]

@protocol CVDisplayServerPart
- (void)sendStringToCurrentComposingBuffer:(NSString *)string;
- (void)sendCharacterToCurrentComposingBuffer:(NSString *)string;
@end

@implementation OVSnippet_AppDelegate

- (NSString *)applicationSupportFolder {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : NSTemporaryDirectory();
    return [basePath stringByAppendingPathComponent:@"OVSnippet"];
}

- (NSManagedObjectModel *)managedObjectModel {

    if (managedObjectModel != nil) {
        return managedObjectModel;
    }
	
    NSMutableSet *allBundles = [[NSMutableSet alloc] init];
    [allBundles addObject: [NSBundle mainBundle]];
    [allBundles addObjectsFromArray: [NSBundle allFrameworks]];
    
    managedObjectModel = [[NSManagedObjectModel mergedModelFromBundles: [allBundles allObjects]] retain];
    [allBundles release];
    
    return managedObjectModel;
}

- (NSPersistentStoreCoordinator *) persistentStoreCoordinator {

    if (persistentStoreCoordinator != nil) {
        return persistentStoreCoordinator;
    }

    NSFileManager *fileManager;
    NSString *applicationSupportFolder = nil;
    NSURL *url;
    NSError *error;
    
    fileManager = [NSFileManager defaultManager];
    applicationSupportFolder = [self applicationSupportFolder];
    if ( ![fileManager fileExistsAtPath:applicationSupportFolder isDirectory:NULL] ) {
        [fileManager createDirectoryAtPath:applicationSupportFolder attributes:nil];
    }
    
    url = [NSURL fileURLWithPath: [applicationSupportFolder stringByAppendingPathComponent: @"OVSnippet.xml"]];
    persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel: [self managedObjectModel]];
    if (![persistentStoreCoordinator addPersistentStoreWithType:NSXMLStoreType configuration:nil URL:url options:nil error:&error]){
        [[NSApplication sharedApplication] presentError:error];
    }    

    return persistentStoreCoordinator;
}
 
- (NSManagedObjectContext *) managedObjectContext {

    if (managedObjectContext != nil) {
        return managedObjectContext;
    }

    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (coordinator != nil) {
        managedObjectContext = [[NSManagedObjectContext alloc] init];
        [managedObjectContext setPersistentStoreCoordinator: coordinator];
    }
    
    return managedObjectContext;
}

- (NSUndoManager *)windowWillReturnUndoManager:(NSWindow *)window {
    return [[self managedObjectContext] undoManager];
}
 
- (IBAction) saveAction:(id)sender {

    NSError *error = nil;
    if (![[self managedObjectContext] save:&error]) {
        [[NSApplication sharedApplication] presentError:error];
    }
}
 
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {

    NSError *error;
    int reply = NSTerminateNow;
    
    if (managedObjectContext != nil) {
        if ([managedObjectContext commitEditing]) {
            if ([managedObjectContext hasChanges] && ![managedObjectContext save:&error]) {				

                BOOL errorResult = [[NSApplication sharedApplication] presentError:error];
				
                if (errorResult == YES) {
                    reply = NSTerminateCancel;
                } 
            }
        } 
        else {
            reply = NSTerminateCancel;
        }
    }    
    return reply;
}

- (void) dealloc {
    [managedObjectContext release], managedObjectContext = nil;
    [persistentStoreCoordinator release], persistentStoreCoordinator = nil;
    [managedObjectModel release], managedObjectModel = nil;
    [super dealloc];
}

- (void)windowWillClose:(NSNotification *)notification
{
	NSLog(@"OpenVanilla Snippets terminated.");
	[[NSApplication sharedApplication] terminate:self];	
}

- (void) insertSnippet:(NSString *)string {
	NSLog(@"Add object");
	NSManagedObjectContext *moc = [self managedObjectContext];
	NSManagedObject *stringObject = [NSEntityDescription insertNewObjectForEntityForName:@"Snippet" inManagedObjectContext:moc];
	[stringObject setValue:string forKey:@"snippetItem"];
	NSLog(@"Done");	
}

// Services

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    [NSApp setServicesProvider:self];
}

- (void)insertSnippetViaService:(NSPasteboard *)pboard userData:(NSString *)data error:(NSString **)error {
	
    NSString *pboardString;
    NSArray *types;
	
    types = [pboard types];	
    if (![types containsObject:NSStringPboardType] || !(pboardString = [pboard stringForType:NSStringPboardType])) {
        *error = NSLocalizedString(@"Error: Pasteboard doesn't contain a string.", @"Pasteboard couldn't give string.");
        return;
    }
	
	[self insertSnippet:pboardString];	
	[window orderFront:self]; 
    return;
}

/*
- (NSDragOperation)tableView:(NSTableView*)tv validateDrop:(id <NSDraggingInfo>)info proposedRow:(int)row proposedDropOperation:(NSTableViewDropOperation)op {
    // [tv setDropRow:[tv numberOfRows] dropOperation:NSTableViewDropAbove];
    return NSTableViewDropAbove;
}*/

- (BOOL)tableView:(NSTableView*)tv acceptDrop:(id <NSDraggingInfo>)info row:(int)row dropOperation:(NSTableViewDropOperation)op
{
	NSLog(@"acceptDrop");
    NSPasteboard *myPasteboard=[info draggingPasteboard];
    NSString *string;	
    string=[myPasteboard stringForType:NSStringPboardType];
	[self insertSnippet:string];
    return YES;
}

- (void)awakeFromNib
{
	NSLog(@"Start!");
	_displayServer = [[NSConnection rootProxyForConnectionWithRegisteredName:@"OVNewDisplayServer-0.8.0" host:nil] retain];
	
	if (_displayServer) {
		[_displayServer setProtocolForProxy:@protocol(CVDisplayServerPart)];		
		[window setLevel:NSFloatingWindowLevel];
	}
	else {
		NSLog(@"cannot find display server");
		[[NSApplication sharedApplication] terminate:self];
	}
	// Make it posssible to drag and drop text into snippetListview
    [snippetListview registerForDraggedTypes:
		[NSArray arrayWithObjects:NSStringPboardType,nil]];	
	[snippetListview setDelegate:self];
	[snippetListview setDoubleAction:@selector(tableAction)];
	[snippetListview setDraggingSourceOperationMask:NSDragOperationLink
									 forLocal:NO];
	[snippetListview setDraggingSourceOperationMask:NSDragOperationCopy
									 forLocal:YES];	
	
}


- (void)tableAction {
	[_displayServer sendStringToCurrentComposingBuffer:[sendKey toolTip]];
}

- (IBAction)stringAction:(id)sender
{
	[_displayServer sendStringToCurrentComposingBuffer:[sender toolTip]];
}

@end
