// OVPrefDelegate.h: OVPreference.app delegated object
//
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#import <Cocoa/Cocoa.h>
#import "CVEmbeddedLoader.h"
#import "CVWrappers.h"

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
    IBOutlet id settab_chewingaddphrase;
    IBOutlet id settab_phoneticslayout;
    IBOutlet id settab_pojascii;
    IBOutlet id settab_pojlayout;
    IBOutlet id settab_pojpojonly;
    IBOutlet id settab_tibetanlayout;
    IBOutlet id sharetab_backcolor;
//  IBOutlet id sharetab_backimage;
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
	IBOutlet id pref_tab;

    // user interface states
    int modtab_modlist_currentrow;
    NSSound *sound;
    NSMutableArray *gimmodlist;        // module list for Generic IM sub-panel
    int gimcurrentindex;               // currently "used" Generic IM index
    NSMutableArray *propeditmodlist;
    int propeditcurrentindex;
    NSMutableArray *propeditproplist;
    NSMutableDictionary *propeditdict;

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
- (IBAction)gim_changeIM:(id)sender;
- (IBAction)gim_settingsTrigger:(id)sender;
- (IBAction)modtab_clearShortcut:(id)sender;
- (IBAction)modtab_shortcutKeyChange:(id)sender;
- (IBAction)oftab_convert:(id)sender;
- (IBAction)pedit_changeModule:(id)sender;
- (IBAction)pref_dumpConfigToConsole:(id)sender;
- (IBAction)pref_writeConfig:(id)sender;
- (IBAction)sharetab_changeColor:(id)sender;
// - (IBAction)sharetab_changeImage:(id)sender;
- (IBAction)sharetab_changeNotify:(id)sender;
- (IBAction)sharetab_changeSound:(id)sender;
- (IBAction)sharetab_changeTransparency:(id)sender;
- (IBAction)sharetab_clearShortcutKey:(id)sender;
- (IBAction)sharetab_setFont:(id)sender;
- (IBAction)sharetab_shortcutKeyChange:(id)sender;
- (IBAction)sharetab_testSound:(id)sender;
- (IBAction)onlineHelp:(id)sender;
- (NSString*)shortenedFilename:(NSString*)f maxLength:(int)m;
- (void)setupSound;
// methods for IM settings
- (void)setupGenericIMSettings;
- (void)setupIMSettings;
- (void)gatherIMSettings;
- (void)setupPropEditSettings;
- (CVModuleWrapper*)initModule:(NSString*)mid;
- (BOOL)identifierExists:(NSString*)mid;
- (NSMutableDictionary*)getConfigNode:(NSString*)mid;
@end
