#import "OpenVanillaPreferencePane.h"
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

const char *defaultplistfile  = "/Library/OpenVanilla/0.6.3/OVLoader.plist";

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
	
	[self createUserConfig];
	
	OVDictionary *configroot=sysconfig->getDictionaryRoot();
	if (!configroot->keyExist("OVLoader")) configroot->newDictionary("OVLoader");
	if (!configroot->keyExist("IM-OVIMPhonetic")) configroot->newDictionary("IM-OVIMPhonetic");
	if (!configroot->keyExist("IM-OVIMPOJHolo")) configroot->newDictionary("IM-OVIMPOJHolo");
	if (!configroot->keyExist("IM-OVIMChewing")) configroot->newDictionary("IM-OVIMChewing");
	if (!configroot->keyExist("IM-OVIMTibetan")) configroot->newDictionary("IM-OVIMTibetan");
	
	OVDictionary *loader=(OVDictionary*)OVSafe(configroot->getDictionary("OVLoader"));
	OVDictionary *phonetic=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPhonetic"));
	OVDictionary *pojholo=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPOJHolo"));
	OVDictionary *chewing=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMChewing"));
	OVDictionary *tibetan=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMTibetan"));

	[prefWarningBeep setState: loader->getIntDefault("warningBeep", 1)];
	[prefPhoneticLayout selectItemAtIndex: phonetic->getIntDefault("keyboardLayout", 0)];
	[prefChewingLayout selectItemAtIndex: chewing->getIntDefault("keyboardLayout", 0)];
	[prefTibetanLayout selectItemAtIndex: tibetan->getIntDefault("keyboardLayout", 0)];
	[prefPOJLayout selectItemAtIndex: pojholo->getIntDefault("keyboardLayout", 0)];
	[prefPOJFull setState: pojholo->getIntDefault("fullPOJOutput", 0)];
	[prefPOJAsciiOutput setState: pojholo->getIntDefault("ASCIIOutput", 0)];
	[prefFontSizeSlider setIntValue: loader->getIntDefault("textSize", 20)];
	[prefFontSizeText setIntValue: loader->getIntDefault("textSize", 20)];
	[prefOpacitySlider setIntValue: loader->getIntDefault("opacity", 80)];
	[prefOpacityText setIntValue: loader->getIntDefault("opacity", 80)];
	
	int keyMask=loader->getIntDefault("menuKey", kMenuOptionModifier | kMenuShiftModifier | kMenuNoCommandModifier);
	
	[prefKeyOpt setState: 0];
	[prefKeyShift setState: 0];
	[prefKeyCtrl setState: 0];
	[prefKeyCommand setState: 0];
	if (keyMask != -1)
	{
		if (keyMask & kMenuOptionModifier) [prefKeyOpt setState: 1];
		if (keyMask & kMenuShiftModifier) [prefKeyShift setState: 1];
		if (keyMask & kMenuControlModifier) [prefKeyCtrl setState: 1];
		if (!(keyMask & kMenuNoCommandModifier)) [prefKeyCommand setState: 1];
	}
	
	float r = 0.0f, g = 0.0f, b = 0.0f;
	char rc[80], gc[80], bc[80];
	
	if(loader->keyExist("forer")) { loader->getString("forer", rc); r=atof(rc); }
	if(loader->keyExist("foreg")) { loader->getString("foreg", gc); g=atof(gc); }
	if(loader->keyExist("foreb")) { loader->getString("foreb", bc); b=atof(bc); }

	[prefColorForeground setColor: [NSColor colorWithCalibratedRed: r 
		green: g blue: b alpha: 1.0f]];
	r = g = b = 1.0f;
	
	if(loader->keyExist("backr")){ loader->getString("backr", rc); r=atof(rc); }
	if(loader->keyExist("backg")){ loader->getString("backg", gc); g=atof(gc); }
	if(loader->keyExist("backb")){ loader->getString("backb", bc); b=atof(bc); }
	[prefColorBackground setColor: [NSColor colorWithCalibratedRed: r 
		green: g blue: b alpha: 1.0f]];
}

- (void)willUnselect
{
	OVAUTODELETE;
	
	OVDictionary *configroot=sysconfig->getDictionaryRoot();
	OVDictionary *loader=(OVDictionary*)OVSafe(configroot->getDictionary("OVLoader"));
	OVDictionary *phonetic=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPhonetic"));
	OVDictionary *pojholo=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMPOJHolo"));
	OVDictionary *chewing=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMChewing"));
	OVDictionary *tibetan=(OVDictionary*)OVSafe(configroot->getDictionary("IM-OVIMTibetan"));

	loader->setInt("warningBeep", [prefWarningBeep state]);
	loader->setInt("textSize", [prefFontSizeText intValue]);
	loader->setInt("opacity", [prefOpacityText intValue]);
	phonetic->setInt("keyboardLayout", [prefPhoneticLayout indexOfSelectedItem]);
	chewing->setInt("keyboardLayout", [prefChewingLayout indexOfSelectedItem]);
	tibetan->setInt("keyboardLayout", [prefTibetanLayout indexOfSelectedItem]);
	pojholo->setInt("keyboardLayout", [prefPOJLayout indexOfSelectedItem]);
	pojholo->setInt("ASCIIOutput", [prefPOJAsciiOutput state]);
	pojholo->setInt("fullPOJOutput", [prefPOJFull state]);
	
	
	int keyMask=0;
	if ([prefKeyOpt state]) keyMask |= kMenuOptionModifier;
	if ([prefKeyShift state]) keyMask |= kMenuShiftModifier;
	if ([prefKeyCtrl state]) keyMask |= kMenuControlModifier;
	if (![prefKeyCommand state]) keyMask |= kMenuNoCommandModifier;

	// we deny the "only shift" case
	if (
	     (keyMask & kMenuShiftModifier) && 
		 (keyMask & kMenuNoCommandModifier) &&
		 !(keyMask & (kMenuOptionModifier | kMenuControlModifier))
	   )
	{
		keyMask = kMenuShiftModifier | kMenuOptionModifier | kMenuNoCommandModifier;
	}
	else 
	{
		if (((keyMask & kMenuNoCommandModifier) && 
		 !(keyMask & (kMenuOptionModifier | kMenuControlModifier))))
		{
			keyMask = -1;
		}
	}

	loader->setInt("menuKey", keyMask);

	float r, g, b, a;
	char rc[3], gc[3], bc[3];
	
	NSColor *forecolor=[prefColorForeground color];
	[forecolor getRed: &r green: &g blue: &b alpha: &a];
	sprintf(rc, "%2f", r);
	sprintf(gc, "%2f", g);
	sprintf(bc, "%2f", b);
	loader->setString("forer", rc);
	loader->setString("foreg", gc);
	loader->setString("foreb", bc);	
	
	NSColor *backcolor=[prefColorBackground color];
	[backcolor getRed: &r green: &g blue: &b alpha: &a];
	sprintf(rc, "%2f", r);
	sprintf(gc, "%2f", g);
	sprintf(bc, "%2f", b);
	loader->setString("backr", rc);
	loader->setString("backg", gc);
	loader->setString("backb", bc);	
	
	sysconfig->write();
}

-(void)createUserConfig
{	
	// first we determine if ~/Library/OpenVanilla/0.6.3/ already exists
	
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
    strcat(userpref, "/0.6.3");
    mkdir(userpref, S_IRWXU) ;

	strcat(userpref, "/OVLoader.plist");
    sysconfig=new VXConfig(userpref);
	
	struct stat filestat;
	
	// if ~/Lib/OV/0.6.3/OVLoader.plist does not exist, copy from default
	if (stat(userpref, &filestat))         // any error
	{
		VXConfig def(defaultplistfile);
		def.read();
		VXDictionary *userdict=new VXDictionary(def.getDictionaryRoot()->getDictRef(), 1);
		sysconfig->changeDictionary(userdict);
		sysconfig->write();
	}

	fprintf (stderr, "OVPrefPane: user preference file at %s\n", userpref);
	sysconfig->read();
}

@end
