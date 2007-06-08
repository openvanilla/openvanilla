//
//  keyTable.m
//  CinEdit
//
//  Created by zonble on 2007/6/8.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "keyTable.h"

@implementation keyTable

- (void) init {
	keyname_items = [NSMutableArray new];
}

- (NSString *) dump {
	NSString * rtn = @"";
	int i;
	for(i =0; i< [keyname_items count]; i++) {
		rtn = [rtn stringByAppendingFormat:@"%@ %@\n",
			[[keyname_items objectAtIndex:i] objectForKey:@"key"],
			[[keyname_items objectAtIndex:i] objectForKey:@"value"]
			];
	}
	return rtn;
}

- (NSString *) dumpEndkey {
	NSString * rtn = @"";
	int i;
	for(i =0; i< [keyname_items count]; i++) {
		if([[[keyname_items objectAtIndex:i] objectForKey:@"endkey"] intValue]) {
			NSLog([[keyname_items objectAtIndex:i] objectForKey:@"key"]);
			rtn = [rtn stringByAppendingString:[[keyname_items objectAtIndex:i] objectForKey:@"key"]];
		}
	}
	return rtn;
}

- (void)addKey: (NSMutableDictionary *)d {	
	[keyname_items addObject:d];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
    if ([[aTableColumn identifier] isEqualToString:@"key"]) {
        return [[keyname_items objectAtIndex:rowIndex] objectForKey:@"key"];
    }
    if ([[aTableColumn identifier] isEqualToString:@"value"]) {
        return [[keyname_items objectAtIndex:rowIndex] objectForKey:@"value"];
    }
    if ([[aTableColumn identifier] isEqualToString:@"endkey"]) {
        return [[keyname_items objectAtIndex:rowIndex] objectForKey:@"endkey"];
    }		
    return nil;
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
    return [keyname_items count];
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
	if ([[aTableColumn identifier] isEqualToString:@"endkey"]) {
		[[keyname_items objectAtIndex:rowIndex] setObject:anObject forKey:@"endkey"];
	}
	if ([[aTableColumn identifier] isEqualToString:@"key"]) {
		NSString *KeyString = anObject;
		KeyString = [KeyString stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
		if(KeyString) {
			unichar c = [KeyString characterAtIndex:0];
			if(isprint((int)c) && c != ' ') {
				c = tolower(c);
				NSString *a = [[NSString alloc] initWithCharacters:&c length:1];
				[[keyname_items objectAtIndex:rowIndex] setObject:a forKey:@"key"];
			}
		}
	}
	if ([[aTableColumn identifier] isEqualToString:@"value"]) {
		if([anObject isEqualToString:@""]) return;
		NSString * a = anObject;
		a = [a stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
		[[keyname_items objectAtIndex:rowIndex] setObject:a forKey:@"value"];
	}
}

- (void)addRow: (int)row {
	NSMutableDictionary *d=[NSMutableDictionary new];
	[d setObject:@"" forKey:@"key"];
	[d setObject:@"" forKey:@"value"];	
	[d setObject:[NSNumber numberWithInt:FALSE]  forKey:@"endkey"];		
	if(row == -1) { 
		[keyname_items insertObject:d atIndex:[keyname_items count]];
	} else {		
		[keyname_items insertObject:d atIndex:row];		
	}
	[d release];	
}

- (void)removeRow:(unsigned)row {
	[keyname_items removeObjectAtIndex:row];
}

@end