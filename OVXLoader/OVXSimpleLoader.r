/* OVXLoader.r */

#define UseExtendedThingResource 1
#include <Carbon/Carbon.r>
#include "CIMconst.h"

resource 'thng' (128)
{
    'tsvc',
    'inpm',
	cimVendorCode,
    0x8000+cimScript*0x100+cimLanguage,
    kAnyComponentFlagsMask,
    'dlle', cimBaseResourceID,
    'STR ', cimBaseResourceID,
    'STR ', cimBaseResourceID,
    'ICON', cimBaseResourceID,
    0x00010000,
    componentHasMultiplePlatforms,
    15872+cimScript*0x200,
    {
        0x8000+cimScript*0x100+cimLanguage,
        'dlle', cimBaseResourceID, 1000
    }
};

resource 'dlle' (cimBaseResourceID)
{
	"CIMComponentDispatch"
};


/* keyboard icon resources: these make an "icon suite" (1-bit, 4-bit, 8-bit)
   of 16x16 icons; used by OS X in the keyboard menu, menu bar, and sys. pref. */

data 'kcs#' (cimBaseResourceID)
{
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

data 'kcs4' (cimBaseResourceID)
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

data 'kcs8' (cimBaseResourceID)
{
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
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

resource 'MENU' (cimBaseResourceID + 1)
{
    cimBaseResourceID,
    textMenuProc,
    allEnabled,
    enabled,
    "00000",
	{}
};

resource 'STR ' (cimBaseResourceID)
{
    "OpenVanilla IM Loader"
};

