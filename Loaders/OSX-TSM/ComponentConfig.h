//
// ComponentConfig.h
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

#ifndef ComponentConfig_h
#define ComponentConfig_h

// configurables
#define TSMC_VERSION            0x00090000	// OpenVanilla 0.9.0
#define TSMC_VENDOR             'opvn'
#define TSMC_NAME               "OpenVanilla"
#define TSMC_NAME_LENGTH        $"0b"
#define TSMC_BUNDLE_ID          "org.openvanilla.LoaderTSM"
#define TSMC_BUNDLE_ID_LENGTH   $"19"
#define TSMC_LOADER_APP_NAME			@"OVLoaderServer.app"
#define TSMC_SERVER_CONNECTION_NAME		@"OpenVanilla_090_Connection"
#define TSMC_LOADER_SERVER_BUNDLE_ID	@"org.openvanilla.LoaderServer"

// change this if you want to do different stuff
#define TSMC_SCRIPT				2           // smTradCinese
#define TSMC_LANGUAGE           19          // langtradChinese
#define TSMC_RESOURCE_ID        16896       // (15872+script_id*512) here script_id=smTradChinese

// derived variables
#define TSMC_PENCIL_MENU_ID		TSMC_RESOURCE_ID + 1
#define TSMC_FLAGS              0x8000 + TSMC_SCRIPT * 0x100 + TSMC_LANGUAGE
#define TARGET_REZ_MAC_PPC          1
#define TARGET_REZ_MAC_X86          1

#endif

//
// Free Tibet
//
