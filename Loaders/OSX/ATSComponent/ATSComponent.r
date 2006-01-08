// ATSComponent.r: Apple Text Service Component resource file
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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

#define UseExtendedThingResource 1
#include <Carbon/Carbon.r>
#include "ATSComponent.h"

resource 'thng' (ATSCBASERESOURCEID, "OpenVanilla")
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
    // componentHasMultiplePlatforms+myComponentRegistrationFlags,        
    0x00000008,                 
	0xa600,                     // Resource ID
	{
#if TARGET_REZ_MAC_PPC
        0x0000fe4a,              // kMyComponentFlags
        'dlle',
        0xa600,                  // Resource ID
        // PowerPC-based Macintosh
        platformPowerPCNativeEntryPoint,  
#endif
#if TARGET_REZ_MAC_X86
        0x0000fe4a,              // kMyComponentFlags
        'dlle',
        0xa600,                  // Resource ID
        // Intel-based Macintosh        
        platformIA32NativeEntryPoint,     
#endif
    };
};

/* component entry point */
resource 'dlle' (ATSCBASERESOURCEID)
{
	"ATSCDispatch"
};


/* keyboard icon resource (1-, 4-, 8-bit): obsolete, but is still required */

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
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"	
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"00FF FF00 0000 0000 0000 0000 00FF FF00"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
};

/* carbon bundle data */
data 'cbnm' (0, "OpenVanilla", sysheap, purgeable) {
	ATSCBNLENGTH ATSCBUNDLENAME
};

/* default text service component name */
data 'STR ' (ATSCBASERESOURCEID) {
	ATSCIMNLENGTH ATSCIMNAME
};

/* menu resource: (empty) */
resource 'MENU' (ATSCBASERESOURCEID + 1)
{
    ATSCBASERESOURCEID,
    textMenuProc,
    allEnabled,
    enabled,
    "00000",
    { }
};
