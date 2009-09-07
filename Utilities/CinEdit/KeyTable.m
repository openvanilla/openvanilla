// KeyTable.m
//
// The routines of the tableview of key-mapping
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#import "KeyTable.h"

@implementation KeyTable

- (void) init {
	keyname_items = [NSMutableArray new];
}

- (NSString *) dumpDataFromARow: (int)row {
	NSString * rtn;
	rtn = [NSString stringWithFormat:@"%@ %@\n",
			[[keyname_items objectAtIndex:row] objectForKey:@"key"],
			[[keyname_items objectAtIndex:row] objectForKey:@"value"]
			];
	return rtn;
}

- (int) find: (NSString *) str {
	int i = 0;	
	NSEnumerator *enumerator = [keyname_items objectEnumerator];
	NSMutableDictionary * d;	
	
	while (d = [enumerator nextObject]) {
		NSString *key = [[d objectForKey:@"key"] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
		NSString *value = [[d objectForKey:@"value"] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
		if([key isEqualToString:@""] || [value isEqualToString:@""]) {
			i++;
			continue;
		}
		if([key compare:str options:NSCaseInsensitiveSearch] == 0 || [value compare:str options:NSCaseInsensitiveSearch] == 0 ) {
			return i;
		}
		i++;
	}
	return 0;
}

- (int) count {
	return [keyname_items count];
}


- (BOOL) isEdited {
	return isEdited;
}

- (void)reset {
	isEdited = NO;
	NSLog(@"KeyTable reset");	
}

- (NSString *) dumpEndkey {
	NSMutableString * rtn = [NSMutableString new];
	int i;
	for(i =0; i< [keyname_items count]; i++) {
		if([[[keyname_items objectAtIndex:i] objectForKey:@"endkey"] intValue]) {
			[rtn appendString:[[keyname_items objectAtIndex:i] objectForKey:@"key"]];
		}
	}
	return rtn;
}

- (void)addKey: (NSMutableDictionary *)d {	
	[keyname_items addObject:d];
}

- (int)addRow: (int)row {
	NSMutableDictionary *d=[NSMutableDictionary new];
	int i;
	[d setObject:@"" forKey:@"key"];
	[d setObject:@"" forKey:@"value"];	
	[d setObject:[NSNumber numberWithInt:FALSE]  forKey:@"endkey"];		
	if(row == -1) { 
		i = [keyname_items count];
	} else {		
		i = row;
	}
	[keyname_items insertObject:d atIndex:i];	
	[d release];	
	return i;
}

- (void)removeRow:(unsigned)row {
	[keyname_items removeObjectAtIndex:row];
}

/* The routines of a tableview */

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
    if ([[aTableColumn identifier] isEqualToString:@"key"]) {
		isEdited = YES;
        return [[keyname_items objectAtIndex:rowIndex] objectForKey:@"key"];
    }
    if ([[aTableColumn identifier] isEqualToString:@"value"]) {
		isEdited = YES;		
        return [[keyname_items objectAtIndex:rowIndex] objectForKey:@"value"];
    }
    if ([[aTableColumn identifier] isEqualToString:@"endkey"]) {
		isEdited = YES;	
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

@end