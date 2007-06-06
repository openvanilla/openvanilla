/* Delegate */

#import <Cocoa/Cocoa.h>

@interface Delegate : NSObject
{
    IBOutlet id list_chardef;
    IBOutlet id list_keyname;
    IBOutlet id txt_cname;
    IBOutlet id txt_ename;
    IBOutlet id txt_endkey;
    IBOutlet id txt_scname;
    IBOutlet id txt_selkey;
    IBOutlet id txt_tcname;
    IBOutlet id win_cin;
	

	NSMutableArray *chardef_items;	
}
@end
