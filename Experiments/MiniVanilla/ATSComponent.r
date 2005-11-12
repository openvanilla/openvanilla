// ATSComponent.r: resource file for ATSComponent
// Copyright (c) 2004-2005 The OpenVanilla Project

#define UseExtendedThingResource 1
#include <Carbon/Carbon.r>
#include "ATSComponent.h"

resource 'thng' (ATSCBASERESOURCEID, "MiniVanilla")
{
    'tsvc',                     // Type
    'inpm',                     // Subtype
    ATSCVENDORCODE,             // Manufacturer
	0x00000000,                 // use componentHasMultiplePlatorms
    0x00000000,
    0x00000000,
    0x00000000,
    'STR ',                     // Name Type
    ATSCBASERESOURCEID,         // Name ID
    'STR ',                     // Info ID
    ATSCBASERESOURCEID+1,       // Info ID
    'ICON',                     // Icon Type
    ATSCBASERESOURCEID,         // Icon ID
    0x00040000,                 // Version
    0x00000008,                 // componentHasMultiplePlatforms+myComponentRegistrationFlags,    
	0xa600,                     // Resource ID
	{
	   0x0000fe4a,              // kMyComponentFlags
	   'dlle',
	   0xa600,                  // Resource ID
	   /* platformIA32NativeEntryPoint */
	   0x0005,
    0x0000fe4a,
    'dlle', ATSCBASERESOURCEID,
    0x0006
    };
};

resource 'dlle' (ATSCBASERESOURCEID)
{
	"ATSCDispatch"
};


/* keyboard icon resources: these make an "icon suite" (1-bit, 4-bit, 8-bit)
   of 16x16 icons; used by OS X in the keyboard menu, menu bar, and sys. pref. */

data 'kcs#' (ATSCBASERESOURCEID)
{
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

data 'kcs4' (ATSCBASERESOURCEID)
{
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"	
};


data 'kcs8' (ATSCBASERESOURCEID)
{
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 0000 0000 0000 0000 0000 00FF FF00"
	$"0000 0000 0000 0000 0000 0000 00FF FF00"
	$"0000 0000 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 00FF 0000 FF00 00FF FF00"
	$"00FF FF00 0000 00FF 0000 FF00 00FF FF00"
	$"00FF FF00 0000 00FF 0000 FF00 00FF FF00"
	$"00FF FF00 0000 00FF 0000 FF00 00FF FF00"	
	$"00FF FF00 0000 00FF 0000 FF00 00FF FF00"
	$"00FF FF00 0000 0000 FF00 FF00 00FF FF00"
	$"00FF FF00 0000 0000 00FF 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
};

data 'cbnm' (0, "MiniVanilla", sysheap, purgeable) {
	ATSCBNLENGTH ATSCBUNDLENAME
};

data 'STR ' (ATSCBASERESOURCEID) {
	ATSCIMNLENGTH ATSCIMNAME
};

resource 'MENU' (ATSCBASERESOURCEID + 1)
{
    ATSCBASERESOURCEID,
    textMenuProc,
    allEnabled,
    enabled,
    "00000",
    { }
};
