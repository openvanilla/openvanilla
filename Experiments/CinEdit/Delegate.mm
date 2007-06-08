#import "Delegate.h"
#include "OVCIN.h"
// #include 

#define MYPATH "/Library/OpenVanilla/0.7.2/Modules/OVIMGeneric/"
#define CIN "~/Desktop/test.cin"

@implementation Delegate

- (IBAction)keyname_add:(id)sender {
	[k addRow:[list_keyname selectedRow]];
	[list_keyname reloadData];	
}

- (IBAction)keyname_rm:(id)sender {
	if([list_keyname selectedRow] > -1) {
		[k removeRow:[list_keyname selectedRow]];
		[list_keyname reloadData];
	}
}

- (IBAction)chardef_add:(id)sender {
	[c addRow:[list_chardef selectedRow]];
	[list_chardef reloadData];	
}

- (IBAction)chardef_rm:(id)sender {
	if([list_chardef selectedRow] > -1) {
		[c removeRow:[list_chardef selectedRow]];
		[list_chardef reloadData];
	}
}

- (void) saveExec {
	NSMutableString  * cin = [NSMutableString new];
	[cin appendFormat:@"%cgen_inp\n", '%'];
	if([txt_ename stringValue]) {
		[cin appendFormat:@"%cename: %@\n", '%', [txt_ename stringValue]];
	}
	if([txt_cname stringValue]) {
		[cin appendFormat:@"%ccname: %@\n", '%', [txt_cname stringValue]];
	}
	[cin appendFormat:@"%ctcname: %@\n", '%', [txt_tcname stringValue]];
	[cin appendFormat:@"%cscname: %@\n", '%', [txt_scname stringValue]];
	[cin appendFormat:@"%cselkey: %@\n", '%', [txt_selkey stringValue]];
	[cin appendFormat:@"%cendkey: %@\n", '%', [k dumpEndkey]];		
	[cin appendFormat:@"%cencoding UTF-8\n", '%'];	
	[cin appendFormat:@"%ckeyname begin\n", '%'];
	int i;
	for(i = 0; i < [k count]; i++) {
		[cin appendString:[k dumpline:i]];
	}
	[cin appendFormat:@"%ckeyname end\n", '%'];
	[cin appendFormat:@"%cchardef begin\n", '%'];
	for(i = 0; i < [c count]; i++) {
		[cin appendString:[c dumpline:i]];
	}
	[cin appendFormat:@"%cchardef end\n", '%'];	
	[cin writeToFile:[[NSString stringWithUTF8String:CIN] stringByStandardizingPath]
		atomically:TRUE
		encoding:NSUTF8StringEncoding
			   error:NULL
		];
}

- (IBAction) save:(id)sender {
	[self saveExec];
}

- (IBAction) saveAs:(id)sender {
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
	int i;
	k = [keyTable alloc];
	[k init];
	for(i = 0; i < cin->keycount(); i++) {
		Keyname keyname=cin->get_key(i);
		NSMutableDictionary *d=[NSMutableDictionary new];
		if(strstr(cin->get_endkey(), keyname.key.c_str())) {
			[d setObject:[NSNumber numberWithInt:TRUE] forKey:@"endkey"];
		} else {
			[d setObject:[NSNumber numberWithInt:FALSE] forKey:@"endkey"];			
		}
		[d setObject:[NSString stringWithUTF8String:keyname.key.c_str()] forKey:@"key"];
		[d setObject:[NSString stringWithUTF8String:keyname.value.c_str()] forKey:@"value"];
		[k addKey: d];
		[d release];		
	}
	[list_keyname setDataSource:k];

	c = [charTable alloc];
	[c init];
	for(i = 0; i < cin->charcount(); i++) {
		Chardef chardef=cin->get_char(i);
		NSMutableDictionary *d=[NSMutableDictionary new];		
		[d setObject:[NSString stringWithUTF8String:chardef.key.c_str()] forKey:@"key"];
		[d setObject:[NSString stringWithUTF8String:chardef.value.c_str()] forKey:@"value"];
		[c addKey: d];
	}
	[list_chardef setDataSource:c];	

	[win_cin setTitle:shortflename];
	[txt_ename setStringValue:ename];
	[txt_cname setStringValue:cname];
	[txt_tcname setStringValue:tcname];	
	[txt_scname setStringValue:scname];	
	[txt_selkey setStringValue:selkey];
}

- (void)awakeFromNib {
	[self loadCin];

}

@end