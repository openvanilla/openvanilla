/* OVPrefDelegate */

#import <Cocoa/Cocoa.h>
#import "CVEmbeddedLoader.h"

@interface OVPrefDelegate : NSObject
{
    IBOutlet id modtab_box_cmd;
    IBOutlet id modtab_box_control;
    IBOutlet id modtab_box_keylist;
    IBOutlet id modtab_box_option;
    IBOutlet id modtab_box_shift;
    IBOutlet id modtab_loadedfrom;
    IBOutlet id modtab_modlist;
    IBOutlet id modtab_selectedmodule;
    IBOutlet id modtab_shortcutkey;
    IBOutlet id oftab_convertbutton;
    IBOutlet id oftab_convertfilter;
    IBOutlet id oftab_inputtext;
    IBOutlet id oftab_oforderlist;
    IBOutlet id otab_outputtext;
    CVEmbeddedLoader *loader;
}
- (IBAction)modtab_shortcutKeyChange:(id)sender;
- (IBAction)pref_dumpConfigToConsole:(id)sender;
- (IBAction)pref_writeConfig:(id)sender;
@end
