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
    IBOutlet id fontmanager;
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
    IBOutlet id sharetab_backcolor;
    IBOutlet id sharetab_backimage;
    IBOutlet id sharetab_cmd;
    IBOutlet id sharetab_control;
    IBOutlet id sharetab_fonttag;
    IBOutlet id sharetab_forecolor;
    IBOutlet id sharetab_keylist;
    IBOutlet id sharetab_option;
    IBOutlet id sharetab_previewview;
    IBOutlet id sharetab_shift;
    IBOutlet id sharetab_shownotify;
    IBOutlet id sharetab_soundfile;
    IBOutlet id sharetab_transparencyslider;
    IBOutlet id sharetab_transparencytag;

    // user interface states
    int modtab_modlist_currentrow;

    CVEmbeddedLoader *loader;
    NSMutableDictionary *config;
    CVPrefArray *modlist;
    CVPrefArray *oflist;
    NSMutableArray *outputfilters;
    NSString *fastimswitchkey;
    NSMutableDictionary *loadhistory;
}
- (IBAction)modtab_shortcutKeyChange:(id)sender;
- (IBAction)oftab_convert:(id)sender;
- (IBAction)pref_dumpConfigToConsole:(id)sender;
- (IBAction)pref_writeConfig:(id)sender;
- (IBAction)sharetab_changeColor:(id)sender;
- (IBAction)sharetab_changeImage:(id)sender;
- (IBAction)sharetab_changeNotify:(id)sender;
- (IBAction)sharetab_changeSound:(id)sender;
- (IBAction)sharetab_changeTransparency:(id)sender;
- (IBAction)sharetab_setFont:(id)sender;
- (IBAction)sharetab_shortcutKeyChange:(id)sender;
- (IBAction)sharetab_testSound:(id)sender;
@end
