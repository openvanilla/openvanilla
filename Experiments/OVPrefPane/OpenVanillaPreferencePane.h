/* OpenVanillaPreferencePane */

#import <Cocoa/Cocoa.h>
#import <PreferencePanes/NSPreferencePane.h>
#import <sys/syslimits.h>
#import "VXConfig.h"

@interface OpenVanillaPreferencePane : NSPreferencePane
{
    IBOutlet id prefChewingLayout;
    IBOutlet id prefFontSizeSlider;
    IBOutlet id prefFontSizeText;
    IBOutlet id prefPhoneticLayout;
    IBOutlet id prefPOJAsciiOutput;
    IBOutlet id prefPOJLayout;
    IBOutlet id prefWarningBeep;
	
	char userpref[PATH_MAX];
	VXConfig *sysconfig;		// from OVLoader.cpp
}
- (void)willSelect;
- (void)willUnselect;
- (void)createUserConfig;		// from OVLoader.cpp
- (IBAction)advancedButton:(id)sender;
@end
