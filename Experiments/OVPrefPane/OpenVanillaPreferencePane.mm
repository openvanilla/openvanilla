#import "OpenVanillaPreferencePane.h"
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

const char *defaultplistfile  = "/Library/OpenVanilla/0.6.2/OVLoader.plist";

@implementation OpenVanillaPreferencePane

- (IBAction)advancedButton:(id)sender
{
	// will cause some syncing problem as method willUnselect will be called
	// once again *after* XML editor is called, but anyway we pretend there
	// is no problem in that
	[self willUnselect];	
	
    char sbuf[PATH_MAX];
    sprintf (sbuf, "open %s", userpref);
    system(sbuf);

	// this is the easiest way to quite System Preferences.app, 
	// no Apple Event, no hustle-bustle, babe
	NSAppleScript *script=[[NSAppleScript alloc] initWithSource: 
		@"tell application \"System Preferences\" to quit"];
	NSDictionary *dict;
	[script executeAndReturnError: &dict];
}

- (void)willSelect
{
	OVAUTODELETE;
	
	fprintf (stderr, "OVPrefPane: preference panel started\n");	
	[self createUserConfig];
	
	OVDictionary *configroot=sysconfig->getDictionaryRoot();
	if (!configroot->keyExist("OVLoader")) configroot->newDictionary("OVLoader");
	if (!configroot->keyExist("IM-OVIMPhonetic")) configroot->newDictionary("IM-OVIMPhonetic");
	if (!configroot->keyExist("IM-OVIMPOJ")) configroot->newDictionary("IM-OVIMPOJ");
	if (!configroot->keyExist("IM-OVIMChewing")) configroot->newDictionary("IM-OVIMChewing");
	if (!configroot->keyExist("IM-OVIMTibetan")) configroot->newDictionary("IM-OVIMTibetan");
	
	OVDictionary *loader=(OVDictionary*)OVSafe(configroot->getDictionary("OVLoader"));
	OVDictionary *phonetic=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPhonetic"));
	OVDictionary *poj=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPOJ"));
	OVDictionary *chewing=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMChewing"));
	OVDictionary *tibetan=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMTibetan"));

	[prefWarningBeep setState: loader->getIntDefault("warningBeep", 1)];
	[prefPhoneticLayout selectItemAtIndex: phonetic->getIntDefault("keyboardLayout", 0)];
	[prefChewingLayout selectItemAtIndex: chewing->getIntDefault("keyboardLayout", 0)];
	[prefTibetanLayout selectItemAtIndex: tibetan->getIntDefault("keyboardLayout", 0)];
	[prefPOJLayout selectItemAtIndex: poj->getIntDefault("keyboardLayout", 0)];
	[prefPOJFull setState: poj->getIntDefault("fullPOJOutput", 0)];
	[prefPOJAsciiOutput setState: poj->getIntDefault("ASCIIOutput", 0)];
	[prefFontSizeSlider setIntValue: loader->getIntDefault("textSize", 20)];
	[prefFontSizeText setIntValue: loader->getIntDefault("textSize", 20)];
}

- (void)willUnselect
{
	OVAUTODELETE;
	
	fprintf (stderr, "OVPrefPane: saving preference data\n");	
	OVDictionary *configroot=sysconfig->getDictionaryRoot();
	OVDictionary *loader=(OVDictionary*)OVSafe(configroot->getDictionary("OVLoader"));
	OVDictionary *phonetic=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPhonetic"));
	OVDictionary *poj=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPOJ"));
	OVDictionary *chewing=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMChewing"));
	OVDictionary *tibetan=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMTibetan"));

	loader->setInt("warningBeep", [prefWarningBeep state]);
	loader->setInt("textSize", [prefFontSizeText intValue]);
	phonetic->setInt("keyboardLayout", [prefPhoneticLayout indexOfSelectedItem]);
	chewing->setInt("keyboardLayout", [prefChewingLayout indexOfSelectedItem]);
	tibetan->setInt("keyboardLayout", [prefTibetanLayout indexOfSelectedItem]);
	poj->setInt("keyboardLayout", [prefPOJLayout indexOfSelectedItem]);
	poj->setInt("ASCIIOutput", [prefPOJAsciiOutput state]);
	poj->setInt("fullPOJOutput", [prefPOJFull state]);
	
	
	sysconfig->write();
}

-(void)createUserConfig
{	
	// first we determine if ~/Library/OpenVanilla/0.6.2/ already exists
	
	char *ptr;    
    if (!(ptr=getenv("HOME")))
    {
        struct passwd *pw = getpwuid(getuid());
        if (pw == NULL) strcpy(userpref, "/tmp");
        else strcpy(userpref, pw->pw_dir);
    }
    else strcpy(userpref, ptr);

    strcat(userpref, "/Library/OpenVanilla") ;
    mkdir(userpref, S_IRWXU) ;
    strcat(userpref, "/0.6.2");
    mkdir(userpref, S_IRWXU) ;

	strcat(userpref, "/OVLoader.plist");
    sysconfig=new VXConfig(userpref);
	
	struct stat filestat;
	
	// if ~/Lib/OV/0.6.2/OVLoader.plist does not exist, copy from default
	if (stat(userpref, &filestat))         // any error
	{
		VXConfig def(defaultplistfile);
		def.read();
		VXDictionary *userdict=new VXDictionary(def.getDictionaryRoot()->getDictRef(), 1);
		sysconfig->changeDictionary(userdict);
		sysconfig->write();
	}

	fprintf (stderr, "OVLoader: user preference file at %s\n", userpref);
	sysconfig->read();
}

@end
