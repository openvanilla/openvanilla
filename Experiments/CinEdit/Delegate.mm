#import "Delegate.h"
#include "OVCIN.h"

#define MYPATH "/Library/OpenVanilla/0.7.2/Modules/OVIMGeneric/"
#define MYCIN "/Library/OpenVanilla/0.7.2/Modules/OVIMGeneric/cj.cin"


@interface keyTable : NSObject
{
	NSMutableArray *keyname_items;	
}
@end

@implementation keyTable

- (void) init {
	keyname_items = [NSMutableArray new];
}

- (void)addKey: (Keyname)keyname {
	NSMutableDictionary *d=[NSMutableDictionary new];
	[d setObject:[NSString stringWithUTF8String:keyname.key.c_str()] forKey:@"key"];
	[d setObject:[NSString stringWithUTF8String:keyname.value.c_str()] forKey:@"value"];
	[keyname_items addObject:d];
	[d release];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(int)rowIndex {
    if ([[aTableColumn identifier] isEqualToString:@"key"]) {
        return [[keyname_items objectAtIndex:rowIndex] objectForKey:@"key"];
    }
    if ([[aTableColumn identifier] isEqualToString:@"value"]) {
        return [[keyname_items objectAtIndex:rowIndex] objectForKey:@"value"];
    }	
    return nil;
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
    return [keyname_items count];
}

- (void) test {
	NSLog([NSString stringWithFormat:@"%d", [keyname_items count]]);
}

@end
//----

@interface charTable : NSObject
{
	NSMutableArray *chardef_items;	
}
@end

@implementation charTable

- (void) init {
	chardef_items = [NSMutableArray new];
}

- (void)addKey: (Chardef)chardef {
	NSMutableDictionary *d=[NSMutableDictionary new];
	[d setObject:[NSString stringWithUTF8String:chardef.key.c_str()] forKey:@"key"];
	[d setObject:[NSString stringWithUTF8String:chardef.value.c_str()] forKey:@"value"];
	[chardef_items addObject:d];
	[d release];
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

@end

//-----

@implementation Delegate



- (void)addChar: (Chardef)chardef{
}

- (void)loadCin {
	[win_cin center];
	OVCIN *cin = new OVCIN();
	cin->load(MYPATH, "cj.cin");
	NSString *shortflename = [NSString stringWithUTF8String: cin->get_filename()];	
	NSString *ename = [NSString stringWithUTF8String: cin->get_ename()];
	NSString *cname = [NSString stringWithUTF8String: cin->get_cname()];
	NSString *tcname = [NSString stringWithUTF8String: cin->get_tcname()];
	NSString *scname = [NSString stringWithUTF8String: cin->get_scname()];	
	NSString *selkey = [NSString stringWithUTF8String: cin->get_selkey()];
	NSString *endkey = [NSString stringWithUTF8String: cin->get_endkey()];
	
	NSLog([NSString stringWithFormat:@"%d", cin->keycount()]);
	NSLog([NSString stringWithFormat:@"%d", cin->charcount()]);	
	int i;
	keyTable * k = [keyTable alloc];
	[k init];
	for(i = 0; i < cin->keycount(); i++) {
		[k addKey: cin->get_key(i)];
	}
	[list_keyname setDataSource:k];

	charTable * c = [charTable alloc];
	[c init];
	for(i = 0; i < cin->charcount(); i++) {
		[c addKey: cin->get_char(i)];
	}
	[list_chardef setDataSource:c];	

	[win_cin setTitle:shortflename];
	[txt_ename setStringValue:ename];
	[txt_cname setStringValue:cname];
	[txt_tcname setStringValue:tcname];	
	[txt_scname setStringValue:scname];	
	[txt_selkey setStringValue:selkey];
	[txt_endkey setStringValue:endkey];	
}

- (void)awakeFromNib {
	[self loadCin];

}

@end