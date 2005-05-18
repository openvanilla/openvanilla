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
    IBOutlet id gim_autocompose;
    IBOutlet id gim_errorbeep;
    IBOutlet id gim_hitmaxcompose;
    IBOutlet id gim_imlist;
    IBOutlet id gim_maxkeylen;
    IBOutlet id gim_shiftselection;
    IBOutlet id imset_modlist;
    IBOutlet id imset_propedit;
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
    IBOutlet id settab_chewinglayout;
    IBOutlet id settab_phoneticslayout;
    IBOutlet id settab_pojascii;
    IBOutlet id settab_pojlayout;
    IBOutlet id settab_pojpojonly;
    IBOutlet id settab_tibetanlayout;
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
    IBOutlet id sharetab_soundlist;
    IBOutlet id sharetab_transparencyslider;
    IBOutlet id sharetab_transparencytag;

    // user interface states
    int modtab_modlist_currentrow;
    NSSound *sound;
    NSMutableArray *gimmodlist;        // module list for Generic IM sub-panel
    int gimcurrentindex;               // currently "used" Generic IM index
    NSMutableArray *propeditmodlist;
    int propeditcurrentindex;

    CVEmbeddedLoader *loader;
    NSMutableDictionary *config;
    NSMutableDictionary *loadercfg;
    NSMutableDictionary *dsrvrcfg;
    NSMutableDictionary *menucfg;
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
- (NSString*)shortenedFilename:(NSString*)f maxLength:(int)m;
- (void)setupSound;
// methods for IM settings
- (void)setupGenericIMSettings;
- (void)setupIMSettings;
- (void)gatherIMSettings;
- (BOOL)identifierExists:(NSString*)mid;
- (NSDictionary*)getConfigNode:(NSString*)mid;
@end
