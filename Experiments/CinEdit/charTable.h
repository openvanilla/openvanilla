//
//  charTable.h
//  CinEdit
//
//  Created by zonble on 2007/6/8.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface charTable : NSObject
{
	NSMutableArray *chardef_items;	
}
- (NSString *)dump;
- (void)addKey: (NSMutableDictionary *)d;
- (void)addRow: (int)row;
- (void)removeRow:(unsigned)row;
@end