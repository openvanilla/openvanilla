// CVLoaderConfig.h: CocoaVanilla Loader configurations
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

#ifndef __CVLoaderConfig_h
#define __CVLoaderConfig_h

#include <Cocoa/Cocoa.h>

#define CVLC_SVN                "$Rev$"
#define CVLC_SYSMODULE_PATH     @"/Library/OpenVanilla/0.8/Modules/"
#define CVLC_USERMODULE_PATH    @"~/Library/OpenVanilla/0.8/Modules/"
#define CVLC_USERSPACE_PATH		@"~/Library/OpenVanilla/0.8/UserSpace/"
#define CVLC_USERCONFIG_PATH	@"~/Library/OpenVanilla/0.8/Preferences/"
#define CVLC_USERCONFIG_FILE	@"~/Library/OpenVanilla/0.8/Preferences/org.openvanilla.080.plist"
#define CVLC_DISPLAYSERVER		@"/Library/OpenVanilla/0.8/Loader/OVDisplayServer.app"
#define CVLC_ATOMIC_INIT_FILE   @"atomic-init-lock"
#define CVLC_ATOIMC_ERROR_FILE  @"/tmp/_openvanilla-atomicinit-error.html"
#define CVLC_FACTORY_DEFAULTS   @"/Library/OpenVanilla/0.8/Loader/OVLoader.bundle/Contents/Resources/org.openvanilla.OVLoader.DefaultSettings.plist"
#define CVLC_PREFERENCEUTIL     @"/Library/OpenVanilla/0.8/Loader/OVPreferences.app"
#define CVLC_PREFERENCEBUNDLE   @"org.openvanilla.ovpreferences.080"
#define CVLC_DOCUMENT_EN		"open http://openvanilla.org/help/"
#define CVLC_DOCUMENT_ZH_TW		"open http://openvanilla.org/help/"
#define CVLC_DOCUMENT_ZH_CN		"open http://openvanilla.org/help/"

NSArray *CVGetModuleLoadPath();
NSString *CVGetUserSpacePath();
NSString *CVGetUserConfigFilename();
NSString *CVGetAtomicInitLockFilename();
NSString *CVGetAtomicInitErrorMessageFilename();
NSDictionary *CVGetDisplayServerConfig();

// void CVCheckDefaultConfiguration(NSMutableDictionary *d);
// void CVCreateIntermediateDirectory(NSString *p);

#endif
