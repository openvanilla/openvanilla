//
//  OVTableModuleController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVTableModuleController.h"


@implementation OVTableModuleController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVTableModuleController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}

- (void)awakeFromNib
{
	[u_tableView setDelegate:self];
	[u_tableView setDataSource:self];
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex
{
	if (![[aTableColumn identifier] isEqualToString:@"value"])
		return;
	
	NSString *key = [[[self dictionary] allKeys] objectAtIndex:rowIndex];
	[self setValue:anObject forKey:key];
	[self updateAndWrite];
	[u_tableView reloadData];
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return [[self dictionary] count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex
{
	if ([[aTableColumn identifier] isEqualToString:@"key"]) {
		return [[[self dictionary] allKeys] objectAtIndex:rowIndex];
	}
	else if ([[aTableColumn identifier] isEqualToString:@"value"]) {
		return [[[self dictionary] allValues] objectAtIndex:rowIndex];		
	}
	return nil;
}

@end
