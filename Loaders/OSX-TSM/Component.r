//
// Component.r
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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
#include "ComponentConfig.h"

#ifndef ATSC_OSX_10_3_9
	resource 'thng' (TSMC_RESOURCE_ID, "OpenVanilla")
	{
		'tsvc',                     // Type
		'inpm',                     // Subtype
		TSMC_VENDOR,                // Manufacturer
		0x00000000,                 // use componentHasMultiplePlatorms
		0x00000000,
		0x00000000,
		0x00000000,
		'STR ',                     // Name Type
		TSMC_RESOURCE_ID,           // Name ID
		'STR ',                     // Info ID
		TSMC_RESOURCE_ID + 1,       // Info ID
		'ICON',                     // Icon Type
		TSMC_RESOURCE_ID,           // Icon ID
		0x00040000,                 // Version
		0x00000008,                 
		TSMC_RESOURCE_ID,           // Resource ID
		{
	#if TARGET_REZ_MAC_PPC
			TSMC_FLAGS,             // kMyComponentFlags
			'dlle',
			TSMC_RESOURCE_ID,       // Resource ID
			platformPowerPCNativeEntryPoint,  
	#endif
	#if TARGET_REZ_MAC_X86
			TSMC_FLAGS,             // kMyComponentFlags
			'dlle',
			TSMC_RESOURCE_ID,       // Resource ID
			platformIA32NativeEntryPoint,     
	#endif
		};
	};
#endif


/* component entry point */
resource 'dlle' (TSMC_RESOURCE_ID)
{
	"TSMCDispatch"
};

/* carbon bundle data */
data 'cbnm' (0, "Lithoglyph", sysheap, purgeable) {
	TSMC_BUNDLE_ID_LENGTH TSMC_BUNDLE_ID
};

/* default text service component name */
data 'STR ' (TSMC_RESOURCE_ID) {
	TSMC_NAME_LENGTH TSMC_NAME
};

/* menu resource: (empty) */
resource 'MENU' (TSMC_PENCIL_MENU_ID)
{
    TSMC_RESOURCE_ID,
    textMenuProc,
    allEnabled,
    enabled,
    "00000",
    { }
};

/* keyboard icon resource (1-, 4-, 8-bit): obsolete, but is still required */
data 'kcs#' (TSMC_RESOURCE_ID)
{
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

data 'kcs4' (TSMC_RESOURCE_ID)
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

data 'kcs8' (TSMC_RESOURCE_ID)
{
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"	
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FF00 0000 0000 0000 0000 00FF 0000"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
};
