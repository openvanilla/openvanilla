/* OVPrefDelegate */

#import <Cocoa/Cocoa.h>
#import "CVEmbeddedLoader.h"

@interface CVPrefArray : NSObject {
    BOOL draggable;
    NSMutableArray *array;
}
- (id)initWithDragDropSetting:(BOOL)d;
- (NSMutableArray*)array;
@end

@interface OVPrefDelegate : NSObject
{
    IBOutlet id modtab_box_cmd;
    IBOutlet id modtab_box_control;
    IBOutlet id modtab_box_keylist;
    IBOutlet id modtab_box_option;
    IBOutlet id modtab_box_shift;
    IBOutlet id modtab_loadedfrom;
    IBOutlet id modtab_modlist;
    IBOutlet id oftab_convertbutton;
    IBOutlet id oftab_convertfilter;
    IBOutlet id oftab_inputtext;
    IBOutlet id oftab_notifymessage;
    IBOutlet id oftab_oforderlist;
    IBOutlet id oftab_outputtext;
    IBOutlet id sharetab_cmd;
    IBOutlet id sharetab_control;
    IBOutlet id sharetab_keylist;
    IBOutlet id sharetab_shift;
    IBOutlet id sharetab_option;

    // user interface states
    int modtab_modlist_currentrow;

    CVEmbeddedLoader *loader;
    NSMutableDictionary *config;
    CVPrefArray *modlist;
    CVPrefArray *oflist;
    NSMutableArray *outputfilters;
    NSString *fastimswitchkey;
}
- (IBAction)modtab_shortcutKeyChange:(id)sender;
- (IBAction)oftab_convert:(id)sender;
- (IBAction)pref_dumpConfigToConsole:(id)sender;
- (IBAction)pref_writeConfig:(id)sender;
- (IBAction)sharetab_shortcutKeyChange:(id)sender;
@end
