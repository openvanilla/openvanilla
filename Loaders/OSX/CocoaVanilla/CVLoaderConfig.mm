// CVLoaderConfig.mm: CocoaVanilla Loader configurations
// The functions here return array objects that contains some default settings
// (such as the settings for candidate bar)
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

#include "CVLoaderConfig.h"
#include "CVLoaderUtility.h"
#include "NSDictionaryExtension.h"

void CVPreparePath() {
	NSString *up = [CVLC_USERSPACE_PATH stringByStandardizingPath];
	NSString *cp = [CVLC_USERCONFIG_PATH stringByStandardizingPath];
	
    if (!CVIfPathExists(up)) {
		NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", up]);
		[[NSFileManager defaultManager] createDirectoryAtPath:up attributes:nil];
	}
    if (!CVIfPathExists(cp)) {
		NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", cp]);
		[[NSFileManager defaultManager] createDirectoryAtPath:cp attributes:nil];
	}
}

NSArray *CVGetModuleLoadPath() {
    NSMutableArray *a = [[NSMutableArray new] autorelease];
    
    // put in CVLC_SYSMODULE_PATH (/Library/OpenVanilla/[version_no]/Modules)
    [a addObject:CVLC_SYSMODULE_PATH];

    // put in CVLC_USERMODULE_PATH (~/Library/OpenVanilla/[version_no]/Modules)    
    NSString *um=[CVLC_USERMODULE_PATH stringByStandardizingPath];
    if (!CVIfPathExists(um)) {
		NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", um]);
		[[NSFileManager defaultManager] createDirectoryAtPath:um attributes:nil];		
	}
    [a addObject:um];
    return a;
}

NSString *CVGetUserSpacePath() {
	CVPreparePath();
	// NSLog([CVLC_USERSPACE_PATH stringByStandardizingPath]);
	return [CVLC_USERSPACE_PATH stringByStandardizingPath];
}

NSString *CVGetAtomicInitLockFilename() {
    return [CVGetUserSpacePath() stringByAppendingPathComponent:CVLC_ATOMIC_INIT_FILE];
}

NSString *CVGetUserConfigFilename() {
	CVPreparePath();
	// NSLog([CVLC_USERCONFIG_FILE stringByStandardizingPath]);
	return [CVLC_USERCONFIG_FILE stringByStandardizingPath];
}

NSDictionary *CVGetDisplayServerConfig() {
	NSMutableDictionary *d=[[[NSMutableDictionary alloc] init] autorelease];
	[d setValue:@"" forKey:@"backgroundImage"];
	[d setValue:@"1.0 1.0 1.0" forKey:@"foreground"];
	[d setValue:@"0.0 0.0 0.5" forKey:@"background"];
	[d setValue:@"0.9" forKey:@"opacity"];
	[d setValue:@"Lucida Grande" forKey:@"font"];
	[d setValue:@"24" forKey:@"size"];
    [d setValue:@"default" forKey:@"notificationStyle"];
	return d;
}

NSString *CVGetAtomicInitErrorMessageFilename() {
    return CVLC_ATOIMC_ERROR_FILE;
}
