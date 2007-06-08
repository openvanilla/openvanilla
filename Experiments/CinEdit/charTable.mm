//
//  charTable.m
//  CinEdit
//
//  Created by zonble on 2007/6/8.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "charTable.h"

@implementation charTable

- (void) init {
	chardef_items = [NSMutableArray new];
}

- (NSMutableDictionary *d) getTable {
	return chardef_items;
}

- (void)addKey: (NSMutableDictionary *)d {
	[chardef_items addObject:d];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
    if ([[aTableColumn identifier] isEqualToString:@"key"]) {
        return [[chardef_items objectAtIndex:rowIndex] objectForKey:@"key"];
    }
    if ([[aTableColumn identifier] isEqualToString:@"value"]) {
        return [[chardef_items objectAtIndex:rowIndex] objectForKey:@"value"];
    }	
    return nil;
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
    return [chardef_items count];
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
	if ([[aTableColumn identifier] isEqualToString:@"key"]) {
		NSString *KeyString = anObject;
		KeyString = [KeyString stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
		if(KeyString) {					
			[[chardef_items objectAtIndex:rowIndex] setObject:KeyString forKey:@"key"];
		}
	}
	if ([[aTableColumn identifier] isEqualToString:@"value"]) {
		if([anObject isEqualToString:@""]) return;
		NSString * a = anObject;
		a = [a stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
		NSLog(a);
		[[chardef_items objectAtIndex:rowIndex] setObject:a forKey:@"value"];
	}
}

- (void)addRow: (int)row {
	NSMutableDictionary *d=[NSMutableDictionary new];
	[d setObject:@"" forKey:@"key"];
	[d setObject:@"" forKey:@"value"];	
	if(row == -1) { 
		[chardef_items insertObject:d atIndex:[chardef_items count]];
	} else {		
		[chardef_items insertObject:d atIndex:row];		
	}
	[d release];	
}

- (void)removeRow:(unsigned)row {
	[chardef_items removeObjectAtIndex:row];
}

@end