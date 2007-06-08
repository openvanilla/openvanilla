/* Delegate */

#import <Cocoa/Cocoa.h>
#import "keyTable.h"
#import "charTable.h"


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
	
	keyTable * k;
	charTable * c;
}
- (IBAction)chardef_add:(id)sender;
- (IBAction)chardef_rm:(id)sender;
- (IBAction)keyname_add:(id)sender;
- (IBAction)keyname_rm:(id)sender;
- (IBAction)save:(id)sender;
- (IBAction)saveAs:(id)sender;
@end
