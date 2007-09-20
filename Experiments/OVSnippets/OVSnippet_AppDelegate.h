//
//  OVSnippet_AppDelegate.h
//  OVSnippet
//
//  Created by zonble on 2007/9/20.
//  Copyright __MyCompanyName__ 2007 . All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface OVSnippet_AppDelegate : NSObject
{
    IBOutlet NSWindow *window;
    IBOutlet id snippetListview;
    IBOutlet id sendKey;	
	id _displayServer;	
    NSPersistentStoreCoordinator *persistentStoreCoordinator;
    NSManagedObjectModel *managedObjectModel;
    NSManagedObjectContext *managedObjectContext;
}

- (NSPersistentStoreCoordinator *)persistentStoreCoordinator;
- (NSManagedObjectModel *)managedObjectModel;
- (NSManagedObjectContext *)managedObjectContext;

- (IBAction)stringAction:(id)sender;
- (IBAction)saveAction:(id)sender;

@end
